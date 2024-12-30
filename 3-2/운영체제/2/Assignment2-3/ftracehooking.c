#include "ftracehooking.h"

static void **syscall_table;
asmlinkage long (*real_os_ftrace)(const struct pt_regs *);

pid_t pid = 0; // pid traced
char file_name[256]; // name of opened file

// the number of times that system calls was called
int n_openat = 0;
int n_read = 0;
int n_write = 0;
int n_lseek = 0;
int n_close = 0;

int read_byte = 0; // read bytes
int write_byte = 0; // write bytes

static asmlinkage int my_ftrace(const struct pt_regs *regs)
{
        // trace start
        if (regs-> di != 0)
        {
                pid = regs->di;
                n_openat = n_read = n_write = n_lseek = n_close = read_byte = write_byte = 0;
                memset(file_name, 0, 256);
                printk(KERN_INFO "OS Assignment 2 ftrace [%d] Start\n", pid);
        }
        // trace termination contidion
        if (regs->di == 0)
        {
                // show trace result
                printk(KERN_INFO "[2020202031] %s file[%s] stats [x] read - %ld / written - %ld\n", current->comm, file_name, read_byte, write_byte);
                printk(KERN_INFO "open[%d] close[%d] read[%d] write[%d] lseek[%d]\n", n_openat, n_close, n_read, n_write, n_lseek);
                printk(KERN_INFO "OS Assignment2 ftrace [%d] End\n", pid);
        }

	return pid;
}

EXPORT_SYMBOL(pid);
EXPORT_SYMBOL(file_name);
EXPORT_SYMBOL(n_openat);
EXPORT_SYMBOL(n_read);
EXPORT_SYMBOL(n_write);
EXPORT_SYMBOL(n_lseek);
EXPORT_SYMBOL(n_close);
EXPORT_SYMBOL(read_byte);
EXPORT_SYMBOL(write_byte);

/* grant pte located at addr RW permission */
static void make_rw(void *addr)
{
        unsigned int level;
        pte_t *pte = lookup_address((u64)addr, &level);

        if(pte->pte &~ _PAGE_RW)
                pte->pte |= _PAGE_RW;
}

/* take away RW permission */
static void make_ro(void *addr)
{
        unsigned int level;
        pte_t *pte = lookup_address((u64)addr, &level);

        pte->pte = pte->pte &~ _PAGE_RW;
}


static int __init hooking_init(void)
{
 	/* Find system call table */
        syscall_table = (void**) kallsyms_lookup_name("sys_call_table");

        /*
         * Change permission of the page of system call table
         * to both readable and writable
         */
        make_rw(syscall_table);

        real_os_ftrace = syscall_table[__NR_os_ftrace];
        syscall_table[__NR_os_ftrace] = my_ftrace;

	make_ro(syscall_table);
        return 0;
}

static void __exit hooking_exit(void)
{
	make_rw(syscall_table);
        syscall_table[__NR_os_ftrace] = real_os_ftrace;

        /* Recover the page's permission (i.e. read-only) */
        make_ro(syscall_table);
}

module_init(hooking_init);
module_exit(hooking_exit);
MODULE_LICENSE("GPL");
