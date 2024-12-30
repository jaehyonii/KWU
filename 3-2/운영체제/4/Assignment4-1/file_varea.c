#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/pid.h>
#include <linux/limits.h>
#include <linux/dcache.h>
#include <linux/highmem.h>
#include <linux/kallsyms.h> /* kallsyms_lookup_name() */
#include <linux/syscalls.h> /* __SYSCALL_DEFINEx() */
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/mm_types.h>
#include <linux/init_task.h>
#include <asm/syscall_wrapper.h> /* __SYSCALL_DEFINEx() */
#include <asm/uaccess.h> /* copy from user() */
#include <asm/ptrace.h>

static void **syscall_table;
void *real_os_ftrace;


asmlinkage pid_t file_varea(const struct pt_regs *regs)
{
    pid_t pid = regs->di;
    struct task_struct *task;
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    struct file *file;
    char *path;
    char buf[100];

    // find process
    task = pid_task(find_vpid(pid), PIDTYPE_PID);
    if (task == NULL) {
        printk(KERN_INFO "Process with PID %d not found.\n", pid);
        return -1;
    }

    mm = get_task_mm(task);
    if (mm == NULL) {
        printk(KERN_INFO "Process %d has no memory structure.\n", pid);
        return -1;
    }

    printk(KERN_INFO "######## Loaded files of a process '%s(%d)' in VM ########\n", task->comm, pid);

    // go around every vma
    for (vma = mm->mmap; vma; vma = vma->vm_next) {
        // get file direction
        file = vma->vm_file;
        if (file == NULL)
            continue;
            
        path = d_path(&file->f_path, buf, sizeof(buf));
        printk(KERN_INFO "mem(%lx-%lx) code(%lx-%lx) data(%lx-%lx) heap(%lx-%lx) %s\n",
            vma->vm_start, vma->vm_end,
            vma->vm_mm->start_code, vma->vm_mm->end_code,
            vma->vm_mm->start_data, vma->vm_mm->end_data,
            vma->vm_mm->start_brk, vma->vm_mm->brk,
            path);
    }
    printk(KERN_INFO "###############################################\n");

    mmput(mm);

	return pid;
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

    real_os_ftrace = syscall_table[__NR_os_ftrace];
    syscall_table[__NR_os_ftrace] = file_varea;

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
