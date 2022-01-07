#include<linux/syscalls.h>
#include<linux/kernel.h>
#include<linux/cred.h>
#include<linux/sched.h>

SYSCALL_DEFINE1(sudiptaprint, char *, str)
{
  char buf[256];
    long copied = strncpy_from_user(buf, str, sizeof(buf));
    if (copied < 0 || copied == sizeof(buf))
    {    
      return -EFAULT;
    }
    printk("sudiptaprint called with \"%s\"\n", buf);
    return 0;
}
