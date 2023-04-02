#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_v2paddr(void){
	uint *phy_addr, *vir_addr;
	// in arguments : phy addr is first and vir addr is second
	// checking for invalid phy addr < zero
	if (argptr(0, (void*) &phy_addr, sizeof(*phy_addr))<0){
		cprintf("Physical Address pointer is Invalid.\n"); return-1;
	}
	// checking for invalid vir addr < zero
	if (argptr(1, (void*) &vir_addr, sizeof(*vir_addr))<0){
		cprintf("Virtual Address pointer is Invalid.\n"); return -1;
	}
	
	// process which called this syscall
	struct proc *cur_proc = myproc();
	// page directory of process & getting pg dir form va
	uint *pg_dir = cur_proc->pgdir;
	uint *pg_dir_entry = &pg_dir[PDX(vir_addr)];
	// pg table of process & pg table entry form va
	uint *pg_table = (uint *)P2V(PTE_ADDR(*pg_dir_entry));
	uint *pg_table_entry = &pg_table[PTX(vir_addr)];
	
	//Checking
	// page dir not present or in user mode
	if (!(*pg_dir_entry & PTE_P)){
		cprintf("Not present entry of page directory.\n"); return -1;
	}
	if (!(*pg_dir_entry & PTE_U)){
		cprintf("In USER mode, thus page directory can't be accessed.\n"); return -1;
	}

	// now after pg dir, for page table and pg table entry
	if (!(*pg_table_entry & PTE_P)){
		cprintf("Not present entry of page table.\n"); return -1;
	}
	if (!(*pg_table_entry & PTE_U)){
		cprintf("In USER mode, thus page table can't be accessed.\n"); return -1;
	}

	// (20 bits + 12 bits) = 32 bits => Physical address
	*phy_addr = PTE_ADDR(*pg_table_entry) | PTE_FLAGS(vir_addr);
	cprintf("Vir addr -> Phy addr successfull.\n");
	return 0;
	// used cprintf and !(bool) because no standard library in present
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
