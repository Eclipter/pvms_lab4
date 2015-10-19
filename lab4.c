#include <linux/module.h>         // Core header for loading LKMs into the kernel
#include <linux/string.h>
#include <linux/kernel.h>         // Contains types, macros, functions for the kernel
#include <linux/fs.h>             // Header for the Linux file system support
#include <asm/uaccess.h>          // Required for the copy to user function
 
MODULE_LICENSE("GPL");            ///< The license type -- this affects available functionality
MODULE_AUTHOR("Vladislav Dektiarev, Eugene Ratkevich");    ///< The author -- visible when you use modinfo
MODULE_DESCRIPTION("A linux kernel module for calculator.");  ///< The description -- see modinfo
MODULE_VERSION("0.1");            ///< A version number to inform users

static char first[100] = {0};
static char second[100] = {0};
static char sign[5] = {0};
static int major = 91;
static short times = 0;

static int     dev_open(struct inode *, struct file *);
static int     dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_read_result(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
static ssize_t dev_write_first(struct file *, const char *, size_t, loff_t *);
static ssize_t dev_write_second(struct file *, const char *, size_t, loff_t *);
static ssize_t dev_write_sign(struct file *, const char *, size_t, loff_t *);

static struct file_operations fops_first =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write_first,
   .release = dev_release,
};

static struct file_operations fops_second =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write_second,
   .release = dev_release,
};

static struct file_operations fops_sign =
{
   .open = dev_open,
   .read = dev_read,
   .write = dev_write_sign,
   .release = dev_release,
};

static struct file_operations fops_result =
{
   .open = dev_open,
   .read = dev_read_result,
   .write = dev_write,
   .release = dev_release,
};

static int __init calc_init(void)
{
	int t = register_chrdev(major, "first", &fops_first);

	if(t < 0) {
		printk(KERN_ALERT "First device registration failed.\n");
		return t;
	}
	else {
		printk(KERN_ALERT "First device registered.\n");
	}

	t = register_chrdev(major + 1, "second", &fops_second);

	if(t < 0) {
		unregister_chrdev(major, "first");
		printk(KERN_ALERT "Second device registration failed.\n");
		return t;
	}
	else {
		printk(KERN_ALERT "Second device registered.\n");
	}

	t = register_chrdev(major + 2, "sign", &fops_sign);

	if(t < 0) {
		unregister_chrdev(major, "first");
		unregister_chrdev(major + 1, "second");
		printk(KERN_ALERT "Sign device registration failed.\n");
		return t;
	}
	else {
		printk(KERN_ALERT "Sign device registered.\n");
	}

	t = register_chrdev(major + 3, "result", &fops_result);

	if(t < 0) {
		unregister_chrdev(major, "first");
		unregister_chrdev(major + 1, "second");
		unregister_chrdev(major + 2, "sign");
		printk(KERN_ALERT "Result device registration failed.\n");
		return t;
	}
	else {
		printk(KERN_ALERT "Result device registered.\n");
	}

	return t;
}

static void __exit calc_exit(void)
{
	unregister_chrdev(major, "first");
	unregister_chrdev(major + 1, "second");
	unregister_chrdev(major + 2, "sign");
	unregister_chrdev(major + 3, "result");
}

static int dev_open(struct inode *inodep, struct file *filep)
{
   printk(KERN_ALERT "Device has been opened.\n");
   return 0;
}

static int dev_release(struct inode *inodep, struct file *filep)
{
   printk(KERN_ALERT "Device successfully closed.\n");
   return 0;
}

static ssize_t dev_write_first(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
   short count = 0;
	memset(first, 0, 100);
	while (len > 0) {
		first[count] = buffer[count++];
		len--;
	}
	return count;
}

static ssize_t dev_write_second(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
   short count = 0;
	memset(second, 0, 100);
	while (len > 0) {
		second[count] = buffer[count++];
		len--;
	}
	return count;
}

static ssize_t dev_write_sign(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
   short count = 0;
	memset(sign, 0, 5);
	while (len > 0) {
		sign[count] = buffer[count++];
		len--;
	}
	return count;
}

static ssize_t dev_read_result(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	char result[100] = {0};
	long first_number;
	long second_number;
	if(times != 0) {
		return 0;
	}
	if(kstrtol(first, 10, &first_number) == 0) {
		printk(KERN_INFO "Result: first number converted.\n");
	}
	else {
		printk(KERN_INFO "Result: first number failed to be converted.\n");
	}
	if(kstrtol(second, 10, &second_number) == 0) {
		printk(KERN_INFO "Result: second number converted.\n");
	}
	else {
		printk(KERN_INFO "Result: second number failed to be converted.\n");
	}

	if(sign[0] == '+') {
		long result_number = first_number + second_number;
		sprintf(result, "%d\n", (int)result_number);
	}
	else if(sign[0] == '-') {
		long result_number = first_number - second_number;
		sprintf(result, "%d\n", (int)result_number);
	}
	else if(sign[0] == '*') {
		long result_number = first_number * second_number;
		sprintf(result, "%d\n", (int)result_number);
	}
	else if(sign[0] == '/') {
		if(second_number == 0) {
			sprintf(result, "%s\n", "Infinity");
		}
		else {
			long result_number = first_number / second_number;
			sprintf(result, "%d\n", (int)result_number);
		}
	}
	else {
		printk(KERN_INFO "Failed to parse the sign %s.\n", sign);
	}

      times++;
    copy_to_user(buffer, result, strlen(result));
    return strlen(result);
}

static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
   printk(KERN_ALERT "Unsupported operation.\n");
   return 0;
}

static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
	printk(KERN_ALERT "Unsupported operation.\n");
    return 0;
}

module_init(calc_init);
module_exit(calc_exit);