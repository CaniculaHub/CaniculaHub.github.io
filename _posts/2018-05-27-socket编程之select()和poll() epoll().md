同步IO和异步IO，阻塞IO和非阻塞IO分别是什么，到底有什么区别？不同的人在不同的上下文下给出的答案是不同的。所以先限定一下本文的上下文。

### 一 概念说明

在进行解释之前，首先要说明几个概念：
\- 用户空间和内核空间
\- 进程切换
\- 进程的阻塞
\- 文件描述符
\- 缓存 I/O

#### 用户空间与内核空间

​	现在操作系统都是采用虚拟存储器，那么对32位操作系统而言，它的寻址空间（虚拟存储空间）为4G（2的32次方）。操作系统的核心是内核，独立于普通的应用程序，可以访问受保护的内存空间，也有访问底层硬件设备的所有权限。为了保证用户进程不能直接操作内核（kernel），保证内核的安全，操心系统将虚拟空间划分为两部分，一部分为内核空间，一部分为用户空间。针对linux操作系统而言，将最高的1G字节（从虚拟地址0xC0000000到0xFFFFFFFF），供内核使用，称为内核空间，而将较低的3G字节（从虚拟地址0x00000000到0xBFFFFFFF），供各个进程使用，称为用户空间。

#### 进程切换

​	为了控制进程的执行，内核必须有能力挂起正在CPU上运行的进程，并恢复以前挂起的某个进程的执行。这种行为被称为进程切换。因此可以说，任何进程都是在操作系统内核的支持下运行的，是与内核紧密相关的。

从一个进程的运行转到另一个进程上运行，这个过程中经过下面这些变化：

1. 保存处理机上下文，包括程序计数器和其他寄存器。
2. 更新PCB信息。
3. 把进程的PCB移入相应的队列，如就绪、在某事件阻塞等队列。
4. 选择另一个进程执行，并更新其PCB。
5. 更新内存管理的数据结构。
6. 恢复处理机上下文。

注：**总而言之就是很耗资源**，具体的可以参考这篇文章：[进程切换](http://guojing.me/linux-kernel-architecture/posts/process-switch/)

#### 进程的阻塞

​	正在执行的进程，由于期待的某些事件未发生，如请求系统资源失败、等待某种操作的完成、新数据尚未到达或无新工作做等，则由系统自动执行阻塞原语(Block)，使自己由运行状态变为阻塞状态。可见，进程的阻塞是进程自身的一种主动行为，也因此只有处于运行态的进程（获得CPU），才可能将其转为阻塞状态。`当进程进入阻塞状态，是不占用CPU资源的`。

#### 文件描述符fd

​	文件描述符（File descriptor）是计算机科学中的一个术语，是一个用于表述指向文件的引用的抽象化概念。

文件描述符在形式上是一个非负整数。实际上，它是一个索引值，指向内核为每一个进程所维护的该进程打开文件的记录表。当程序打开一个现有文件或者创建一个新文件时，内核向进程返回一个文件描述符。在程序设计中，一些涉及底层的程序编写往往会围绕着文件描述符展开。但是文件描述符这一概念往往只适用于UNIX、Linux这样的操作系统。

#### 缓存 I/O

​	缓存 I/O 又被称作标准 I/O，大多数文件系统的默认 I/O 操作都是缓存 I/O。在 Linux 的缓存 I/O 机制中，操作系统会将 I/O 的数据缓存在文件系统的页缓存（ page cache ）中，也就是说，数据会先被拷贝到操作系统内核的缓冲区中，然后才会从操作系统内核的缓冲区拷贝到应用程序的地址空间。

**缓存 I/O 的缺点：**
数据在传输过程中需要在应用程序地址空间和内核进行多次数据拷贝操作，这些数据拷贝操作所带来的 CPU 以及内存开销是非常大的。

### 二 IO模式

​	刚才说了，对于一次IO访问（以read举例），数据会先被拷贝到操作系统内核的缓冲区中，然后才会从操作系统内核的缓冲区拷贝到应用程序的地址空间。所以说，当一个read操作发生时，它会经历两个阶段：

1. 等待数据准备 (Waiting for the data to be ready)
2. 将数据从内核拷贝到进程中 (Copying the data from the kernel to the process)

正式因为这两个阶段，linux系统产生了下面五种网络模式的方案。
\- 阻塞 I/O（blocking IO）
\- 非阻塞 I/O（nonblocking IO）
\- I/O 多路复用（ IO multiplexing）
\- 信号驱动 I/O（ signal driven IO）
\- 异步 I/O（asynchronous IO）

注：由于signal driven IO在实际中并不常用，所以我这只提及剩下的四种IO Model。

#### 阻塞 I/O（blocking IO）

在linux中，默认情况下所有的socket都是blocking，一个典型的读操作流程大概是这样： 

![](http://wx4.sinaimg.cn/mw690/b00a7483gy1frb8rm6lprj20fc097abb.jpg)

当用户进程调用了recvfrom这个系统调用，kernel就开始了IO的第一个阶段：准备数据（对于网络IO来说，很多时候数据在一开始还没有到达。比如，还没有收到一个完整的UDP包。这个时候kernel就要等待足够的数据到来）。这个过程需要等待，也就是说数据被拷贝到操作系统内核的缓冲区中是需要一个过程的。而在用户进程这边，整个进程会被阻塞（当然，是进程自己选择的阻塞）。当kernel一直等到数据准备好了，它就会将数据从kernel中拷贝到用户内存，然后kernel返回结果，用户进程才解除block的状态，重新运行起来。

> 所以，blocking IO的特点就是在IO执行的两个阶段都被block了。

#### 非阻塞 I/O（nonblocking IO）

linux下，可以通过设置socket使其变为non-blocking。当对一个non-blocking socket执行读操作时，流程是这个样子：

![](http://wx2.sinaimg.cn/mw690/b00a7483gy1frb8rjkertj20gr099abv.jpg)

当用户进程发出read操作时，如果kernel中的数据还没有准备好，那么它并不会block用户进程，而是立刻返回一个error。从用户进程角度讲 ，它发起一个read操作后，并不需要等待，而是马上就得到了一个结果。用户进程判断结果是一个error时，它就知道数据还没有准备好，于是它可以再次发送read操作。一旦kernel中的数据准备好了，并且又再次收到了用户进程的system call，那么它马上就将数据拷贝到了用户内存，然后返回。

> 所以，nonblocking IO的特点是用户进程需要**不断的主动询问**kernel数据好了没有。

#### I/O 多路复用（ IO multiplexing）

​	IO multiplexing就是我们说的select，poll，epoll，有些地方也称这种IO方式为event driven IO。select/epoll的好处就在于单个process就可以同时处理多个网络连接的IO。它的基本原理就是select，poll，epoll这个function会不断的轮询所负责的所有socket，当某个socket有数据到达了，就通知用户进程。

![](http://wx2.sinaimg.cn/mw690/b00a7483gy1frb8roxg4xj20gx092jt1.jpg)

`当用户进程调用了select，那么整个进程会被block`，而同时，kernel会“监视”所有select负责的socket，当任何一个socket中的数据准备好了，select就会返回。这个时候用户进程再调用read操作，将数据从kernel拷贝到用户进程。

> 所以，I/O 多路复用的特点是通过一种机制一个进程能同时等待多个文件描述符，而这些文件描述符（套接字描述符）其中的任意一个进入读就绪状态，select()函数就可以返回。

这个图和blocking IO的图其实并没有太大的不同，事实上，还更差一些。因为这里需要使用两个system call (select 和 recvfrom)，而blocking IO只调用了一个system call (recvfrom)。但是，用select的优势在于它可以同时处理多个connection。

所以，如果处理的连接数不是很高的话，使用select/epoll的web server不一定比使用multi-threading + blocking IO的web server性能更好，可能延迟还更大。select/epoll的优势并不是对于单个连接能处理得更快，而是在于能处理更多的连接。）

在IO multiplexing Model中，实际中，对于每一个socket，一般都设置成为non-blocking，但是，如上图所示，整个用户的process其实是一直被block的。只不过process是被select这个函数block，而不是被socket IO给block。

## 总结

### blocking和non-blocking的区别

调用blocking IO会一直block住对应的进程直到操作完成，而non-blocking IO在kernel还准备数据的情况下会立刻返回。

### synchronous IO和asynchronous IO的区别

在说明synchronous IO和asynchronous IO的区别之前，需要先给出两者的定义。POSIX的定义是这样子的：
\- A synchronous I/O operation causes the requesting process to be blocked until that I/O operation completes;
\- An asynchronous I/O operation does not cause the requesting process to be blocked;

两者的区别就在于synchronous IO做”IO operation”的时候会将process阻塞。按照这个定义，之前所述的blocking IO，non-blocking IO，IO multiplexing都属于synchronous IO。

有人会说，non-blocking IO并没有被block啊。这里有个非常“狡猾”的地方，定义中所指的”IO operation”是指真实的IO操作，就是例子中的recvfrom这个system call。non-blocking IO在执行recvfrom这个system call的时候，如果kernel的数据没有准备好，这时候不会block进程。但是，当kernel中数据准备好的时候，recvfrom会将数据从kernel拷贝到用户内存中，这个时候进程是被block了，在这段时间内，进程是被block的。

而asynchronous IO则不一样，当进程发起IO 操作之后，就直接返回再也不理睬了，直到kernel发送一个信号，告诉进程说IO完成。在这整个过程中，进程完全没有被block。

![](http://wx2.sinaimg.cn/mw690/b00a7483gy1frb8rsd1w7j20h209376c.jpg)

通过上面的图片，可以发现non-blocking IO和asynchronous IO的区别还是很明显的。在non-blocking IO中，虽然进程大部分时间都不会被block，但是它仍然要求进程去主动的check，并且当数据准备完成以后，也需要进程主动的再次调用recvfrom来将数据拷贝到用户内存。而asynchronous IO则完全不同。它就像是用户进程将整个IO操作交给了他人（kernel）完成，然后他人做完后发信号通知。在此期间，用户进程不需要去检查IO操作的状态，也不需要主动的去拷贝数据。 

### [详述socket编程之select()和poll()函数]

select()函数和poll()函数均是主要用来处理多路I/O复用的情况。比如一个服务器既想等待输入终端到来，又想等待若干个套接字有客户请求到达，这时候就需要借助select或者poll函数了。
（一）select()函数
原型如下：

```
 int select(int fdsp1, fd_set *readfds, fd_set *writefds, fd_set *errorfds, const struct timeval *timeout);
```

各个参数含义如下：

- int fdsp1:最大描述符值 + 1
- fd_set *readfds:对可读感兴趣的描述符集
- fd_set *writefds:对可写感兴趣的描述符集
- fd_set *errorfds:对出错感兴趣的描述符集
- struct timeval *timeout:超时时间（注意：对于linux系统，此参数没有const限制，每次select调用完毕timeout的值都被修改为剩余时间，而unix系统则不会改变timeout值）

select函数会在发生以下情况时返回：

- readfds集合中有描述符可读
- writefds集合中有描述符可写
- errorfds集合中有描述符遇到错误条件
- 指定的超时时间timeout到了

当select返回时，描述符集合将被修改以指示哪些个描述符正处于可读、可写或有错误状态。可以用FD_ISSET宏对描述符进行测试以找到状态变化的描述符。如果select因为超时而返回的话，所有的描述符集合都将被清空。
select函数返回状态发生变化的描述符总数。返回0意味着超时。失败则返回-1并设置errno。可能出现的错误有：EBADF（无效描述符）、EINTR（因终端而返回）、EINVAL（nfds或timeout取值错误）。
设置描述符集合通常用如下几个宏定义：

```
FD_ZERO(fd_set fdset);                / clear all bits in fdset           */
FD_SET(int fd, fd_set fdset);         / turn on the bit for fd in fd_set  */
FD_CLR(int fd, fd_set fdset);         / turn off the bit for fd in fd_set */
int FD_ISSET(int fd, fd_set fdset);   / is the bit for fd on in fdset?    */
```

如:

```
fd_set rset;
FD_ZERO(&rset);                        /* initialize the set: all bits off  */
FD_SET(1, &rset);                      /* turn on bit for fd 1              */
FD_SET(4, &rset);                      /* turn on bit for fd 4              */
FD_SET(5, &rset);                      /* turn on bit for fd 5              */
```

当select返回的时候，rset位都将被置0，除了那些有变化的fd位。
当发生如下情况时认为是可读的：

1. socket的receive buffer中的字节数大于socket的receive buffer的low-water mark属性值。（low-water mark值类似于分水岭，当receive buffer中的字节数小于low-water mark值的时候，认为socket还不可读，只有当receive buffer中的字节数达到一定量的时候才认为socket可读）
2. 连接半关闭（读关闭，即收到对端发来的FIN包）
3. 发生变化的描述符是被动套接字，而连接的三路握手完成的数量大于0，即有新的TCP连接建立
4. 描述符发生错误，如果调用read系统调用读套接字的话会返回-1。

当发生如下情况时认为是可写的：
1. socket的send buffer中的字节数大于socket的send buffer的low-water mark属性值以及socket已经连接或者不需要连接（如UDP）。
2. 写半连接关闭，调用write函数将产生SIGPIPE
3. 描述符发生错误，如果调用write系统调用写套接字的话会返回-1。

注意：
select默认能处理的描述符数量是有上限的，为FD_SETSIZE的大小。
对于timeout参数，如果置为NULL，则表示wait forever；若timeout->tv_sec = timeout->tv_usec = 0，则表示do not wait at all；否则指定等待时间。
如果使用select处理多个套接字，那么需要使用一个数组（也可以是其他结构）来记录各个描述符的状态。而使用poll则不需要，下面看poll函数。
（二）poll()函数
原型如下：
```
 int poll(struct pollfd *fdarray, unsigned long nfds, int timeout);
```

各参数含义如下：

- struct pollfd *fdarray:一个结构体，用来保存各个描述符的相关状态。
- unsigned long nfds:fdarray数组的大小，即里面包含有效成员的数量。
- int timeout:设定的超时时间。（以毫秒为单位）

poll函数返回值及含义如下：

- -1：有错误产生
- 0：超时时间到，而且没有描述符有状态变化
- \>0：有状态变化的描述符个数

着重讲fdarray数组，因为这是它和select()函数主要的不同的地方：
pollfd的结构如下：

```
struct pollfd 
{
   int fd;                  /* descriptor to check */
   short events;      /* events of interest on fd */
   short revents;     /* events that occured on fd */
};
```

其实poll()和select()函数要处理的问题是相同的，只不过是不同组织在几乎相同时刻同时推出的，因此才同时保留了下来。select()函数把可读描述符、可写描述符、错误描述符分在了三个集合里，这三个集合都是用bit位来标记一个描述符，一旦有若干个描述符状态发生变化，那么它将被置位，而其他没有发生变化的描述符的bit位将被clear，也就是说select()的readset、writeset、errorset是一个value-result类型，通过它们传值，而也通过它们返回结果。这样的一个坏处是每次重新select 的时候对集合必须重新赋值。而poll()函数则与select()采用的方式不同，它通过一个结构数组保存各个描述符的状态，每个结构体第一项fd代表描述符，第二项代表要监听的事件，也就是感兴趣的事件，而第三项代表poll()返回时描述符的返回状态。合法状态如下：

- POLLIN:                有普通数据或者优先数据可读
- POLLRDNORM:    有普通数据可读
- POLLRDBAND:    有优先数据可读
- POLLPRI:              有紧急数据可读
- POLLOUT:            有普通数据可写
- POLLWRNORM:   有普通数据可写
- POLLWRBAND:    有紧急数据可写
- POLLERR:            有错误发生
- POLLHUP:            有描述符挂起事件发生
- POLLNVAL:          描述符非法

对于POLLIN | POLLPRI等价与select()的可读事件；POLLOUT | POLLWRBAND等价与select()的可写事件；POLLIN 等价与POLLRDNORM | POLLRDBAND，而POLLOUT等价于POLLWRBAND。如果你对一个描述符的可读事件和可写事件以及错误等事件均感兴趣那么你应该都进行相应的设置。
对于timeout的设置如下：

- - INFTIM:   wait forever
  - 0:            return immediately, do not block
  - \>0:         wait specified number of milliseconds

### epoll总结

**1、基本知识**

　　epoll是在2.6内核中提出的，是之前的select和poll的增强版本。相对于select和poll来说，epoll更加灵活，没有描述符限制。epoll使用一个文件描述符管理多个描述符，将用户关系的文件描述符的事件存放到内核的一个事件表中，这样在用户空间和内核空间的copy只需一次。

**2、epoll接口**

　　epoll操作过程需要三个接口，分别如下：

```
#include <sys/epoll.h>
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
```

**（1） int epoll_create(int size);**
　　创建一个epoll的句柄，size用来告诉内核这个监听的数目一共有多大。这个参数不同于select()中的第一个参数，给出最大监听的fd+1的值。需要注意的是，当创建好epoll句柄后，它就是会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，必须调用close()关闭，否则可能导致fd被耗尽。

**（2）int epoll_ctl(int epfd, int op, int fd, struct epoll_event \*event);**
　　epoll的事件注册函数，它不同与select()是在监听事件时告诉内核要监听什么类型的事件epoll的事件注册函数，它不同与select()是在监听事件时告诉内核要监听什么类型的事件，而是在这里先注册要监听的事件类型。第一个参数是epoll_create()的返回值，第二个参数表示动作，用三个宏来表示：
EPOLL_CTL_ADD：注册新的fd到epfd中；
EPOLL_CTL_MOD：修改已经注册的fd的监听事件；
EPOLL_CTL_DEL：从epfd中删除一个fd；
第三个参数是需要监听的fd，第四个参数是告诉内核需要监听什么事，struct epoll_event结构如下：

```
struct epoll_event {
  __uint32_t events;  /* Epoll events */
  epoll_data_t data;  /* User data variable */
};
```

events可以是以下几个宏的集合：
EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
EPOLLOUT：表示对应的文件描述符可以写；
EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
EPOLLERR：表示对应的文件描述符发生错误；
EPOLLHUP：表示对应的文件描述符被挂断；
EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里

**（3） int epoll_wait(int epfd, struct epoll_event \* events, int maxevents, int timeout);**
　　等待事件的产生，类似于select()调用。参数events用来从内核得到事件的集合，maxevents告之内核这个events有多大，这个maxevents的值不能大于创建epoll_create()时的size，参数timeout是超时时间（毫秒，0会立即返回，-1将不确定，也有说法说是永久阻塞）。该函数返回需要处理的事件数目，如返回0表示已超时。

**3、工作模式**

　　epoll对文件描述符的操作有两种模式：LT（level trigger）和ET（edge trigger）。LT模式是默认模式，LT模式与ET模式的区别如下：

　　LT模式：**当epoll_wait检测到描述符事件发生并将此事件通知应用程序，应用程序可以不立即处理该事件。下次调用epoll_wait时，会再次响应应用程序并通知此事件。**

　　ET模式：**当epoll_wait检测到描述符事件发生并将此事件通知应用程序，应用程序必须立即处理该事件。如果不处理，下次调用epoll_wait时，不会再次响应应用程序并通知此事件。**

　　**ET模式在很大程度上减少了epoll事件被重复触发的次数，因此效率要比LT模式高。epoll工作在ET模式的时候，必须使用非阻塞套接口，以避免由于一个文件句柄的阻塞读/阻塞写操作把处理多个文件描述符的任务饿死。**
