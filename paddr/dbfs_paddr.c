#include <linux/debugfs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/uaccess.h>
//#include <asm/pgtable.h>
#include <linux/pgtable.h>
#include <asm/io.h>

MODULE_LICENSE("GPL");

struct packet {
        pid_t pid;
        unsigned long vaddr;
        unsigned long paddr;
};

static struct dentry *dir, *output;
static struct task_struct *task;

static ssize_t read_output(struct file *fp,
                        char __user *user_buffer,
                        size_t length,
                        loff_t *position)
{
        // Implement read file operation
        
        struct task_struct *task;
        struct packet* input_packet = (void*)user_buffer;
        struct page* page = NULL;
        unsigned long vaddr = input_packet->vaddr;
        printk("pid: %d\n",input_packet->pid);
        task = pid_task(find_vpid(input_packet->pid), PIDTYPE_PID); 
        
        struct mm_struct *mm = task->mm;
        pgd_t* pgd;
        pte_t* ptep, *pte;
        pud_t* pud;
        pmd_t* pmd;
        
        pgd = pgd_offset(mm, vaddr);
        pud = pud_offset((p4d_t*)pgd, vaddr);
        pmd = pmd_offset(pud, vaddr);
        pte = pte_offset_map(pmd, vaddr);
        page = pte_page(*pte);
        pte_unmap(pte);

        input_packet->paddr = page_to_phys(page);

        return length;
}
static ssize_t write_to_output(struct file *fp, 
                                const char __user *user_buffer, 
                                size_t length, 
                                loff_t *position)
{
        return length;
}
static const struct file_operations dbfs_fops = {
        // Mapping file operations with your functions
        .read = read_output,
        .write = write_to_output,
};

static int __init dbfs_module_init(void)
{
        dir = debugfs_create_dir("paddr", NULL);
        
        if (!dir) {
                printk("Cannot create paddr dir\n");
                return -1;
        }
        output = debugfs_create_file("output", 0777, dir, NULL , &dbfs_fops);
	printk("dbfs_paddr module initialize done\n");

        return 0;
}

static void __exit dbfs_module_exit(void)
{
        // Implement exit module

	printk("dbfs_paddr module exit\n");
}

module_init(dbfs_module_init);
module_exit(dbfs_module_exit);
