#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>


#define PROC_NAME "seconds"
#define BUFFER_SIZE 128

ssize_t read_seconds(struct file* file, char __user *user_buf, size_t count, loff_t *pos);

ssize_t read_seconds(struct file* file, char __user *user_buf, size_t count, loff_t *pos)
{
	char buffer[BUFFER_SIZE];
	int rv = 0;
	unsigned long sec = jiffies/HZ;
	
	{
		static int completed = 0;
		if(completed)
		{
			completed = 0;
			return 0;
		}
		completed = 1;
	}
	rv = sprintf(buffer, "%lu", sec);
	copy_to_user(user_buf, buffer, rv);
	return rv;
}

static const struct proc_ops proc_ops_seconds = {

	.proc_read = read_seconds,
};

int proc_init(void)
{
	proc_create(PROC_NAME, 0666, NULL, &proc_ops_seconds);
	printk(KERN_INFO "Module loaded\n");
	return 0;
}

void proc_exit(void)
{
	remove_proc_entry(PROC_NAME, NULL);
	printk(KERN_INFO "Module unloaded\n");
}


module_init(proc_init);
module_exit(proc_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Creating a process entry jiffies");
MODULE_AUTHOR("AHUNO");


