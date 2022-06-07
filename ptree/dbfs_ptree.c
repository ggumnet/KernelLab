#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>

MODULE_LICENSE("GPL");

static struct dentry *dir, *inputdir, *ptreedir;
static struct task_struct *curr;

static ssize_t write_pid_to_input(struct file *fp, 
                                const char __user *user_buffer, 
                                size_t length, 
                                loff_t *position)
{
        pid_t input_pid;

        copy_from_user(fp->private_data+*position, user_buffer, length);
        
        printk("%s", fp->private_data+*position);

        //curr = pid_task(find_vpid(input_pid), PIDTYPE_PID);
        return length;
}

static ssize_t read_pid_from_ptree(struct file *fp, 
                                char __user *user_buffer, 
                                size_t length, 
                                loff_t *position)
{
       
        return length;
}

static const struct file_operations dbfs_fops = {
        .write = write_pid_to_input,
        .read = read_pid_from_ptree,
};

static int __init dbfs_module_init(void)
{
        dir = debugfs_create_dir("ptree", NULL);
        
        if (!dir) {
                printk("Cannot create ptree dir\n");
                return -1;
        }

        inputdir = debugfs_create_file("input", 0777, dir , NULL , &dbfs_fops);
        ptreedir = debugfs_create_file("ptree", 0777, dir , NULL, &dbfs_fops); // Find suitable debugfs API
	
	printk("dbfs_ptree module initialize done\n");

        return 0;
}

static void __exit dbfs_module_exit(void)
{
        // Implement exit module code
	
	printk("dbfs_ptree module exit\n");
}

module_init(dbfs_module_init);
module_exit(dbfs_module_exit);
