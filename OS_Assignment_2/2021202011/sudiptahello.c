#include<linux/syscalls.h>
#include<linux/kernel.h>
#include<linux/cred.h>
#include<linux/sched.h>

SYSCALL_DEFINE0(sudiptahello)
{
  printk("Hello World Sudipta..");
  return 0;
}
