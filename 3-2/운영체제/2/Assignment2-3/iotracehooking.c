#include "ftracehooking.h"

static void **syscall_table;

asmlinkage long (*real_openat)(const struct pt_regs *);
asmlinkage long (*real_read)(const struct pt_regs *);
asmlinkage long (*real_write)(const struct pt_regs *);
asmlinkage long (*real_lseek)(const struct pt_regs *);
asmlinkage long (*real_close)(const struct pt_regs *);

// the number of times that system calls was called
extern int pid;
extern char file_name[256];

// the number of times that system calls was called
extern int n_openat;
extern int n_read;
extern int n_write;
extern int n_lseek;
extern int n_close;

extern int read_byte; // read bytes
extern int write_byte; // write bytes

static asmlinkage long ftrace_openat(const struct pt_regs *regs)
{
        if(pid == current->pid)
        {
                n_openat++;

                // filename copy
                copy_from_user(file_name, (char*)regs->si, sizeof(regs->si) - 1);
                file_name[sizeof(regs->si)] = NULL;
        }

	return real_openat(regs);
}

static asmlinkage long ftrace_read(const struct pt_regs *regs)
{
        long size_read = real_read(regs);
        if(pid == current->pid)
        {
	        n_read++;
                read_byte += size_read;
        }
       
	return size_read;
}

static asmlinkage long ftrace_write(const struct pt_regs *regs)
{
        long size_write = real_write(regs);
	if(pid == current->pid)
        {
	        n_write++;
                write_byte += size_write;
        }
       
	return size_write;
}

static asmlinkage long ftrace_lseek(const struct pt_regs *regs)
{
	if(pid == current->pid)
	        n_lseek++;
       
	return real_lseek(regs);
}

static asmlinkage long ftrace_close(const struct pt_regs *regs)
{
	if(pid == current->pid)
	        n_close++;
       
	return real_close(regs);
}


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
	
        real_openat = syscall_table[__NR_openat];
        syscall_table[__NR_openat] = ftrace_openat;

	real_read = syscall_table[__NR_read];
	syscall_table[__NR_read] = ftrace_read;

	real_write = syscall_table[__NR_write];
	syscall_table[__NR_write] = ftrace_write;

	real_lseek = syscall_table[__NR_lseek];
	syscall_table[__NR_lseek] = ftrace_lseek;

	real_close = syscall_table[__NR_close];
	syscall_table[__NR_close] = ftrace_close;
	
	make_ro(syscall_table);

        return 0;
}

static void __exit hooking_exit(void)
{
	make_rw(syscall_table);
	
        syscall_table[__NR_openat] = real_openat;
        syscall_table[__NR_read] = real_read;
        syscall_table[__NR_write] = real_write;
        syscall_table[__NR_lseek] = real_lseek;
        syscall_table[__NR_close] = real_close;

        /* Recover the page's permission (i.e. read-only) */
        make_ro(syscall_table);
}

module_init(hooking_init);
module_exit(hooking_exit);
MODULE_LICENSE("GPL");
