#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/proc_fs.h>
#include <linux/string.h> /* strlen() */
#include <linux/uaccess.h> /* copy_to_user(), copy_from_user() */
#include <linux/kernel.h> // kstrtoint
#include <linux/vmalloc.h>  // kmalloc, kfree

struct proc_dir_entry *oslab_fp_dir; /* for "proc_2020202031" proc directory */
struct proc_dir_entry *oslab_fp_B; /* for "proc_2020202031/B" proc file */

char oslab_buffer[500] = {0, }; /* for user input */

pid_t target_pid = -1;

int oslab_is_processed = 0;

// mapping state to string
const char *state_to_str(const struct task_struct *task)
{
    long state = task->state;
    long exit_state = task->exit_state;
    switch(state)
    {
        case TASK_RUNNING:
            return "R (running)";
        case TASK_INTERRUPTIBLE:
            return "S (sleeping)";
        case TASK_UNINTERRUPTIBLE:
            return "D (disk sleep)";
        case TASK_STOPPED:
            return "T (stopped)";
        case __TASK_TRACED:
            return "t (tracing stop)";
        case TASK_PARKED:
            return "P (parked)";
        case TASK_IDLE:
            return "I (idle)";
    }

    switch(exit_state)
    {
        case EXIT_ZOMBIE:
            return "Z (zombie)";
        case EXIT_DEAD:
            return "X (dead)";
    }

    return "etc.";
}

int oslab_open(struct inode *inode, struct file *file)
{
    oslab_is_processed = 0; // Operation End Indicator
    return 0;
}

ssize_t oslab_read(struct file *f, char __user *data_usr, size_t len, loff_t *off)
{
    char *buf; // buffer that will be copied to user buffer
    ssize_t len_written; // buf size
    int j; // used for snprintf
    int found = 0; // check if target_pid process exists

    // target_process information
    struct task_struct*task;
    pid_t pid;
    pid_t ppid;
    uid_t uid;
    gid_t gid;
    u64 utime;
    u64 stime;
    char *state;
    char *name;

    if(oslab_is_processed) return 0;

    // kernel memory allocation for very big memory
    buf = vmalloc(80 * 1024);
    if (!buf) { // fail to allocate memory
        printk(KERN_ERR "vmalloc failed\n");
        return 0;
    }
    

    j = 0;
    j += snprintf(buf + j, 81920 - j, "%-5s %-5s %-5s %-5s %-15s %-15s %-15s %-15s\n",
                "Pid", "PPid", "Uid", "Gid", "utime", "stime", "State", "Name");
    j += snprintf(buf + j, 81920 - j, "---------------------------------------------------------------\n");
    // visit all processes
    for_each_process(task) {
        pid = task->pid;
        ppid = task->real_parent->pid;
        uid = task->cred->uid.val;
        gid = task->cred->gid.val;
        utime = task->utime;
        stime = task->stime;
        state = state_to_str(task);
        name = task->comm;

        // if target_pid is positive, only print information of target_pid
        if(target_pid != -1 && pid != target_pid)
            continue;

        j += snprintf(buf + j, 81920 - j, "%-5d %-5d %-5d %-5d %-15lld %-15lld %-15s %-s\n",
                    pid, ppid, uid, gid, utime, stime, state, name);
        found = 1;
    }

    // if there's not target_pid process
    if(found == 0)
        j += snprintf(buf + j, 81920 - j, "No process found with PID %d\n", target_pid);

    // copy to user side
    len_written = strlen(buf);
    copy_to_user(data_usr, buf, len_written);   

    oslab_is_processed++;

    // free memory
    vfree(buf);

    return len_written;
}   

ssize_t oslab_write(struct file *f, const char __user * data_usr, size_t len, loff_t *off)
{
    ssize_t len_copied;
    int value;


    len_copied = len;
    copy_from_user(oslab_buffer, data_usr, len_copied);

    // oslab_buffer represents integer
    if (kstrtoint(oslab_buffer, 10, &value) == 0) {
        target_pid = value;
    // oslab_buffer doesn't represent integer
    } else {
        target_pid = -1;
    }
    printk(KERN_INFO "target_pid: %d\n", target_pid);

    return len_copied;
}

const struct file_operations oslab_ops = {
    .owner = THIS_MODULE,
    .open = oslab_open,
    .read = oslab_read,
    .write = oslab_write,
};

int __init oslab_init(void)
{
    // 모듈적재시, /proc/proc_2020202031 디렉토리생성
    oslab_fp_dir = proc_mkdir("proc_2020202031", NULL);

    // 모듈적재시, /proc/proc_2020202031/processInfo 파일생성
    oslab_fp_B = proc_create("processInfo", 0666, oslab_fp_dir, &oslab_ops);

    return 0;
}

void __exit oslab_exit(void)
{
    // 모듈해재시, /proc/proc_2020202031/processInfo 파일 삭제
    remove_proc_entry("processInfo", oslab_fp_dir);

    // 모듈해재시, /proc/proc_2020202031 디렉토리 삭제
    remove_proc_entry("proc_2020202031", NULL);
}

module_init(oslab_init);
module_exit(oslab_exit);
MODULE_LICENSE("GPL");