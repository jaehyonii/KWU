#include <linux/module.h>
#include <linux/highmem.h>
#include <linux/kallsyms.h> /* kallsyms_lookup_name() */
#include <linux/syscalls.h> /* __SYSCALL_DEFINEx() */
#include <asm/syscall_wrapper.h> /* __SYSCALL_DEFINEx() */
#include <asm/uaccess.h> /* copy from user() */
#include <linux/string.h>