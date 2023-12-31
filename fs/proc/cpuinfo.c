// SPDX-License-Identifier: GPL-2.0
#include <linux/cpufreq.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <soc/qcom/socinfo.h>

__weak void arch_freq_prepare_all(void)
{
}

extern const struct seq_operations cpuinfo_op;
static int cpuinfo_open(struct inode *inode, struct file *file)
{
	arch_freq_prepare_all();
	return seq_open(file, &cpuinfo_op);
}

static const struct file_operations proc_cpuinfo_operations = {
	.open		= cpuinfo_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static int cpumaxfreq_show(struct seq_file *m, void *v)
{
	uint32_t hw_type = get_hw_version_platform();
	if(HARDWARE_PLATFORM_COURBET   == hw_type
	  || HARDWARE_PLATFORM_SWEET   == hw_type)
		seq_printf(m, "2.30\n");
	else
		seq_printf(m, "2.20\n");
	return 0;
}

static int cpumaxfreq_open(struct inode *inode, struct file *file)
{
	return single_open(file, &cpumaxfreq_show, NULL);
}

static const struct file_operations proc_cpumaxfreq_operations = {
	.open		= cpumaxfreq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static int __init proc_cpuinfo_init(void)
{
	proc_create("cpuinfo", 0, NULL, &proc_cpuinfo_operations);
	proc_create("cpumaxfreq", 0444, NULL, &proc_cpumaxfreq_operations);
	return 0;
}
fs_initcall(proc_cpuinfo_init);
