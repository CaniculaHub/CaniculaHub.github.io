  .text
  b     reset           @0x00  reset
  ldr   pc,_udef        @0x04  udef
  ldr   pc,_swi_handler @0x08  swi   软中断程序的入口   突破32M空间的限制
  ldr   pc,_prefech     @0x0c  prefetch abort
  ldr   pc,_data_abort  @0x10  data  abort
  nop                   @0x14 reserved
  ldr   pc,_irq       @0x18  irq
  ldr   pc,_fiq        @0x1c  fiq
  
_udef:
  .word   _udef
_swi_handler:
  .word   swi_handler  
_prefech:
  .word   _prefech
_data_abort:
  .word   _data_abort  
_irq:
  .word   _irq  
_fiq:
  .word   _fiq  


  
swi_handler:   @ 0xffff00ff
  stmfd  sp!,{r0,lr}    @进栈保护现场
  sub  r0,lr,#4
  ldr  r0,[r0]
  bic  r0,#0xff000000
  bl    fun
  ldmfd  sp!,{r0,pc}^    @出栈恢复现场
  
fun:
	cmp   r0,#6
	moveq  r1,#3
	moveq  r2,#4 
	moveq  r3,#5 
	moveq  r4,#6
	ADD	 r5,r1,r2
	ADD	 r6,r3,r4
	ADD	 r8,r5,r6
	mov   pc,lr

reset:
   ldr  sp,=stack_base			@切换到应用程序
   msr  cpsr,#0x10
   mov  r0,#3					@保护现场测试
   swi  6    @跳转到异常向量表中软中断的入口位置
             @保存返回地址到LR 
			 @切换到SVC工作模式
   mov  r1,r0
   b    reset

   .data
buf:
   .space   32
stack_base:
   .end
   
   