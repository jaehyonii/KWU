#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(os_ftrace, pid_t, pid)
{
	printk("ORIGINAL ftrace() called! PID is [%d]\n", pid);

	return pid;
}
