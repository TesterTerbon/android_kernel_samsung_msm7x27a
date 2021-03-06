#ifndef _ENTRY_H
#define _ENTRY_H

#include <linux/types.h>
#include <linux/signal.h>
#include <asm/ptrace.h>
<<<<<<< HEAD
<<<<<<< HEAD

typedef void pgm_check_handler_t(struct pt_regs *, long, unsigned long);
extern pgm_check_handler_t *pgm_check_table[128];
pgm_check_handler_t do_protection_exception;
pgm_check_handler_t do_dat_exception;

extern int sysctl_userprocess_debug;
=======
#include <asm/cputime.h>
#include <asm/timer.h>

extern void (*pgm_check_table[128])(struct pt_regs *);
extern void *restart_stack;
=======
#include <asm/cputime.h>

extern void *restart_stack;
extern unsigned long suspend_zero_pages;
>>>>>>> refs/remotes/origin/master

void system_call(void);
void pgm_check_handler(void);
void ext_int_handler(void);
void io_int_handler(void);
void mcck_int_handler(void);
void restart_int_handler(void);
void restart_call_handler(void);
<<<<<<< HEAD
void psw_idle(struct s390_idle_data *, struct vtimer_queue *,
	      unsigned long, int);
=======
void psw_idle(struct s390_idle_data *, unsigned long);
>>>>>>> refs/remotes/origin/master

asmlinkage long do_syscall_trace_enter(struct pt_regs *regs);
asmlinkage void do_syscall_trace_exit(struct pt_regs *regs);

void do_protection_exception(struct pt_regs *regs);
void do_dat_exception(struct pt_regs *regs);
<<<<<<< HEAD
void do_asce_exception(struct pt_regs *regs);
>>>>>>> refs/remotes/origin/cm-10.0
=======

void addressing_exception(struct pt_regs *regs);
void data_exception(struct pt_regs *regs);
void default_trap_handler(struct pt_regs *regs);
void divide_exception(struct pt_regs *regs);
void execute_exception(struct pt_regs *regs);
void hfp_divide_exception(struct pt_regs *regs);
void hfp_overflow_exception(struct pt_regs *regs);
void hfp_significance_exception(struct pt_regs *regs);
void hfp_sqrt_exception(struct pt_regs *regs);
void hfp_underflow_exception(struct pt_regs *regs);
void illegal_op(struct pt_regs *regs);
void operand_exception(struct pt_regs *regs);
void overflow_exception(struct pt_regs *regs);
void privileged_op(struct pt_regs *regs);
void space_switch_exception(struct pt_regs *regs);
void special_op_exception(struct pt_regs *regs);
void specification_exception(struct pt_regs *regs);
void transaction_exception(struct pt_regs *regs);
void translation_exception(struct pt_regs *regs);
>>>>>>> refs/remotes/origin/master

void do_per_trap(struct pt_regs *regs);
void syscall_trace(struct pt_regs *regs, int entryexit);
void kernel_stack_overflow(struct pt_regs * regs);
void do_signal(struct pt_regs *regs);
<<<<<<< HEAD
int handle_signal32(unsigned long sig, struct k_sigaction *ka,
		    siginfo_t *info, sigset_t *oldset, struct pt_regs *regs);
<<<<<<< HEAD

void do_extint(struct pt_regs *regs, unsigned int, unsigned int, unsigned long);
int __cpuinit start_secondary(void *cpuvoid);
void __init startup_init(void);
void die(const char * str, struct pt_regs * regs, long err);
=======
void do_notify_resume(struct pt_regs *regs);

struct ext_code;
void do_extint(struct pt_regs *regs, struct ext_code, unsigned int, unsigned long);
void do_restart(void);
void __init startup_init(void);
void die(struct pt_regs *regs, const char *str);

void __init time_init(void);
>>>>>>> refs/remotes/origin/cm-10.0
=======
void handle_signal32(unsigned long sig, struct k_sigaction *ka,
		    siginfo_t *info, sigset_t *oldset, struct pt_regs *regs);
void do_notify_resume(struct pt_regs *regs);

void __init init_IRQ(void);
void do_IRQ(struct pt_regs *regs, int irq);
void do_restart(void);
void __init startup_init(void);
void die(struct pt_regs *regs, const char *str);
int setup_profiling_timer(unsigned int multiplier);
void __init time_init(void);
int pfn_is_nosave(unsigned long);
void s390_early_resume(void);
unsigned long prepare_ftrace_return(unsigned long parent, unsigned long ip);
>>>>>>> refs/remotes/origin/master

struct s390_mmap_arg_struct;
struct fadvise64_64_args;
struct old_sigaction;

<<<<<<< HEAD
long sys_mmap2(struct s390_mmap_arg_struct __user  *arg);
long sys_s390_ipc(uint call, int first, unsigned long second,
	     unsigned long third, void __user *ptr);
long sys_s390_personality(unsigned int personality);
long sys_s390_fadvise64(int fd, u32 offset_high, u32 offset_low,
		    size_t len, int advice);
long sys_s390_fadvise64_64(struct fadvise64_64_args __user *args);
long sys_s390_fallocate(int fd, int mode, loff_t offset, u32 len_high,
			u32 len_low);
long sys_fork(void);
long sys_clone(unsigned long newsp, unsigned long clone_flags,
	       int __user *parent_tidptr, int __user *child_tidptr);
long sys_vfork(void);
void execve_tail(void);
long sys_execve(const char __user *name, const char __user *const __user *argv,
		const char __user *const __user *envp);
long sys_sigsuspend(int history0, int history1, old_sigset_t mask);
long sys_sigaction(int sig, const struct old_sigaction __user *act,
		   struct old_sigaction __user *oact);
long sys_sigaltstack(const stack_t __user *uss, stack_t __user *uoss);
=======
>>>>>>> refs/remotes/origin/master
long sys_sigreturn(void);
long sys_rt_sigreturn(void);
long sys32_sigreturn(void);
long sys32_rt_sigreturn(void);

#endif /* _ENTRY_H */
