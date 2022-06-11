#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/slab.h>


MODULE_LICENSE("GPL");

static struct dentry *dir, *inputdir, *ptreedir;
static struct task_struct *curr;
char * malloc_buff;
char* result_str;

static ssize_t write_pid_to_input(struct file *fp, 
                                const char __user *user_buffer, 
                                size_t length, 
                                loff_t *position)
{
        long* input_pid;
        //pid_t input_pid;

        //copy_from_user(fp->private_data+*position, user_buffer, length);
        //strncpy(malloc_buff,user_buffer, length);
        //printk("%s\n", malloc_buff);
        //printk("%s\n", fp->private_data+*position);

        char buf[32];
	size_t buf_size;
	unsigned long value;
	buf_size = min(length, (sizeof(buf)-1));
        
	if (copy_from_user(buf, user_buffer, buf_size))
		return -EFAULT;
        buf[buf_size] = '\0';
        printk("%s\n", buf);
        //printk("%d\n", strlen(buf));
	
        kstrtol(buf, 10, input_pid);
        printk("pid: %ld\n", *input_pid);

        return buf_size;


        struct task_struct *temp;
        char *temp_str;

        /*
        curr = pid_task(find_vpid(input_pid), PIDTYPE_PID);
        strcpy(result_str, "");
        temp = curr;
        while(1){
                strcpy(temp_str, "");
                strcat(temp_str, temp->comm);
                strcat(temp_str, " (");
                strcat(temp_str, temp->pid);
                strcat(temp_str, ")\n");
                strcat(temp_str, result_str);
                strcpy(result_str, temp_str);
        }*/
	return buf_size;
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
        //malloc_buff = kmalloc(16, GFP_KERNEL);
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
	//kfree(malloc_buff);
	printk("dbfs_ptree module exit\n");
}

module_init(dbfs_module_init);
module_exit(dbfs_module_exit);
