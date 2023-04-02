# Approach for Implementation:

1. Core allocation
    - we will choose for eg. core 7 for kernel level thread
    - changes in sheduler of `proc.c`(sheduler function)
    - for this core we will make a sheduler  
    - this sheduler will execute a function to poll for any changes in `syscall page` and if it finds any then exeutes it

2. Implementing system call - `flexSC_register`
    - first initialise a system call `flexSC_register` with it's corresponding system call id as done in previous lab in required files
    - write the system call function in `sysfile.c`
    - in that function as we are now priviledge we have an access to priviledge fuctions :
        - call kalloc function which returns a virtual address of a newly created page (free entry) in the physical memory
        - we add uva2ka(address) to a array of pointers which will be used in kernel thread to access the system call pages
        - now this function will return same address as kalloc to the user process

3. Implementing function to write to `syscall page` entry
    - here we write to the address returned form flexSC_register (in user process)

4. Implementing system call - `flexSC_wait`
    - it checks for status of entries in `syscall page` and make them wait until execution completes
    - wherever `flexSC_register` was defined for system call similarly we will define `flexSC_wait`

5. Implementing priviledge functions for Kernel Thread (on different CORE)
    - it keeps on polling for changes form `syscall page` if it finds any entries which are to be executed
    - in core we allocated, it will run the process in it and execute corresponding system calls
    - update return value in syscall page entry
    - file `proc.c` to be updated
    
##### Files to be changed:
- `sysfile.c`, `vm.c`, `proc.c`, `proc.h`
- for system call : `syscall.h`, `user.h`, `usys.S`, `Makefile`
- changes in it are mentioned above

##### Note: 
- `uva2ka` -> user virutal address to kernel address (pre-defined function in XV6)
- `cpuid` -> for cpu id
- `kalloc` -> return a address of a newly allocated page
