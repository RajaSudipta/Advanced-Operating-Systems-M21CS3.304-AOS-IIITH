#include<stdio.h>
#include<string.h>

int main()
{
	int res;
	res = syscall(548);
    printf("SYSCALL1 : %d\n", res);
    res = syscall(549, "HI SUDIPTA HALDER!!");
    printf("SYSCALL2 : %d\n", res);
    res = syscall(550);
    printf("SYACALL3 : %d\n", res);
    res = syscall(551);
    printf("SYSCALL4 : %d\n", res);
    return 0;
}
