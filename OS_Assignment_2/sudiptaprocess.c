#include<linux/syscalls.h>
#include<linux/kernel.h>
#include<linux/cred.h>
#include<linux/sched.h>

SYSCALL_DEFINE0(sudiptaprocess)
{
  printk("Parent Process ID: %d", current->parent->pid);
  printk("Current Process ID: %d", current->pid);
  return 0;
}
