#include <linux/module.h>
#include <linux/highmem.h>
#include <linux/kallsyms.h> /* kallsyms_lookup_name() */
#include <linux/syscalls.h> /* __SYSCALL_DEFINEx() */
#include <linux/sched.h>
#include <asm/syscall_wrapper.h> /* __SYSCALL_DEFINEx() */
#include <asm/uaccess.h> /* copy from user() */

static const char* state_to_str(const struct task_struct* target)
{
        long state = target->state;
        long exit_state = target->exit_state;

        switch(state) {
        case TASK_RUNNING:
            return "Running or ready";
        case TASK_INTERRUPTIBLE:
            return "Wait";
        case TASK_UNINTERRUPTIBLE:
            return "Wait with ignoring all signals";
        case TASK_STOPPED:
            return "Stopped";
        }

        switch(exit_state) {
        case EXIT_ZOMBIE:
            return "Zombie process";
        case EXIT_DEAD:
            return "Dead";
        }


        return "etc.";
}


static void **syscall_table;
void *real_os_ftrace;


asmlinkage pid_t process_tracer(const struct pt_regs *regs)
{
        pid_t pid = regs->di;
        struct task_struct* target_task = &init_task; // target
        struct task_struct* real_parent_task; // parent of target
        struct task_struct* group_leader_task; // process group leader of target
        struct task_struct* children_task; // child process of target
        struct task_struct* sibling_task; // sibling process of target
        int sibling_cnt = 0;
        int child_cnt = 0;

        // finding target
        do
        {
                if(target_task->pid == pid)
                        break;
                target_task = next_task(target_task);
        }
        while(target_task->pid != init_task.pid);
        // finding fails
        if(target_task->pid != pid)
                return -1;

        real_parent_task = target_task->parent;
        group_leader_task = target_task->group_leader;
        
        

        printk(KERN_INFO "##### TASK INFORMATION of ''[%d] %s'' #####\n", target_task->pid, target_task->comm);
        printk(KERN_INFO "- task state : %s\n", state_to_str(target_task));
        printk(KERN_INFO "- Process Group Leader : [%d] %s\n", group_leader_task->pid, group_leader_task->comm);
        printk(KERN_INFO "- Number of context switches : %ld\n", target_task->nvcsw + target_task->nivcsw);
        printk(KERN_INFO "- Number of calling forks : %d\n", target_task->fork_count);
        printk(KERN_INFO "- its parent process : [%d] %s\n", real_parent_task->pid, real_parent_task->comm);
        
        printk(KERN_INFO "- its sibling process(es)\n");
        // finding all siblings of target
        list_for_each_entry(sibling_task, &target_task->sibling, sibling)
        {
                if(sibling_task->pid == 0)
                        continue;
                
                /* task now points to one of current's siblings */
                printk(KERN_INFO "\t> [%d] %s\n", sibling_task->pid, sibling_task->comm);
                sibling_cnt++;
        }
        if(sibling_cnt > 0)
                printk(KERN_INFO "\t> This process has %d sibling process(es)\n", sibling_cnt);
        else
                printk(KERN_INFO "It has no sibiling.\n", sibling_cnt);
        
        printk(KERN_INFO "- its child process(es)\n");
        // finding all children of target
        list_for_each_entry(children_task, &target_task->children, sibling)
        {
                /* task now points to one of current's children */
                printk(KERN_INFO "\t> [%d] %s\n", children_task->pid, children_task->comm);
                child_cnt++;
        }
        if(child_cnt > 0)
                printk(KERN_INFO "\t> This process has %d child process(es)\n", child_cnt);
        else
                printk(KERN_INFO "It has no child.\n", child_cnt);
        printk(KERN_INFO "##### END OF INFORMATION #####\n");

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
        syscall_table[__NR_os_ftrace] = process_tracer;

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
