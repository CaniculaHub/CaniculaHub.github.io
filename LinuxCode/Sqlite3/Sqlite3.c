#include <stdio.h>
#include <sqlite3.h>
#include <string.h>
#include <stdlib.h>
#define DATABASE "FRUIT.db"
int fruit_insert(sqlite3 *DB)
{
    
    char Name_of_Product[32];
    int Unit_Price;
    int Quantity;
    char handle[64];
    char *errmsg;
    printf("Input  Name_of_Product: ");
    scanf("%s",Name_of_Product);

    printf("Input  Unit_Price: ");
    scanf("%d",&Unit_Price);

    printf("Input  Quantity: ");
    scanf("%d",&Quantity);
    sprintf(handle,"insert into FRUIT values('%s',%d,%d)",Name_of_Product,Unit_Price,Quantity);
    if(sqlite3_exec(DB,handle,NULL,NULL,&errmsg)!=SQLITE_OK)
    {
        printf("%s\n",errmsg);
    }
    else 
    printf("Insert done.\n");
	return 0;
}

int fruit_delete(sqlite3 *DB)
{
    int  number;
	int  Quantity;
	char handle[64];
    char name[32];
	char *errmsg;

	printf("Input Name_of_Product:");
	scanf("%s", name);

    printf("Input your buy number:");
    scanf("%d",&number);

	sprintf(handle, "update FRUIT set Quantity = Quantity-%d where Name_of_Product ='%s';",number, name);

	if(sqlite3_exec(DB, handle, NULL, NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s\n", errmsg);
	}
	else
	{
		printf("Delete done.\n");
	}

	return 0;
}

int callback(void *arg, int f_num, char ** f_value, char ** f_name)
{
	int i = 0;
	for(i = 0; i < f_num; i++)
	{
//		printf("%-8s: ", f_name[i]);
		printf("       %-15s", f_value[i]);
	}

//	printf("++++++++++++++++++++++");
	putchar(10);
	return 0;
}
int do_query(sqlite3 *DB)
{
	char *errmsg;
	char handle[64] = "select * from FRUIT;";

	if(sqlite3_exec(DB, handle, callback,NULL, &errmsg) != SQLITE_OK)
	{
		printf("%s", errmsg);
	}
	else
	{
		printf("select done.\n");
	}

	return 0;
}


int main(int argc, char const *argv[])
{
    /* code */
    sqlite3 *fruit_db;
    char *errmsg;
    char handle[64];
    int  select_flag; 

    if (sqlite3_open(DATABASE,&fruit_db)!=SQLITE_OK)
    {
        printf("%s\n",sqlite3_errmsg(fruit_db));
        return -1;
    }
    else
    {
        printf("OPEN DATABASE Success!\n");
    }

    sprintf(handle,"create table FRUIT(Name_of_Product char,Unit_Price int,Quantity int);");
    if(sqlite3_exec(fruit_db,handle,NULL,NULL,&errmsg)!=SQLITE_OK)
    {
        printf("%s\n",errmsg);
    }
    else
    {
        printf("Create FRUIT_DATABASE success!\n");
    }
    while(1)
    {
        printf(" ---------------------------------------------\n");
        printf("|1:进货     2：卖出     3：退出      4：查询 |\n");
        printf(" ---------------------------------------------\n");
        printf("Please select:");
        scanf("%d",&select_flag);
        switch(select_flag)
        {
            case 1:
                    fruit_insert(fruit_db);
            		 break;
            case 2:
                    fruit_delete(fruit_db);
           			 break;
            case 3:
					sqlite3_close(fruit_db);
					exit(0);
           			 break;
            case 4:
					printf("|---------------------------------------------------------|\n");
					printf("|Name_of_Product-----------Unit_Price----------Quantity---|\n");
					printf("|---------------------------------------------------------|\n");
					do_query(fruit_db);
           			 break;
            default:
            printf("Invalid data \n");
        }
    }

    
    return 0;
}
