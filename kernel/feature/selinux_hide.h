#ifndef __KSU_H_SELINUX_HIDE
#define __KSU_H_SELINUX_HIDE
#include <linux/types.h>
#include <linux/version.h>
#include <linux/sched.h>

void ksu_selinux_hide_init();
void ksu_selinux_hide_exit();
void ksu_selinux_hide_drop_backup_if_unused();
void ksu_selinux_hide_handle_second_stage();
void ksu_selinux_hide_handle_post_fs_data();

struct policydb;
struct page;
struct policydb *ksu_selinux_hide_policydb(struct policydb *policydb);
struct page *ksu_selinux_hide_status_page(void);

/* Synchronous process-context hook; data stays owned by the load caller. */
void ksu_selinux_hide_policy_load_after(const void *data, size_t len, int rc);

// https://github.com/torvalds/linux/commit/b21507e272627c434e8dd74e8d51fd8245281b59
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0) || defined(KSU_COMPAT_SETPROCATTR_USE_NEW_PROTOTYPE)
typedef int (*setprocattr_fn)(const char *name, void *value, size_t size);

int ksu_handle_selinux_setprocattr(const char *name, void *value, size_t size);
#else
typedef int (*setprocattr_fn)(struct task_struct *p, char *name, void *value, size_t size);

int ksu_handle_selinux_setprocattr(struct task_struct *p, char *name, void *value, size_t size);
#endif

#endif
