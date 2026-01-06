# Shared Memory
- Shared memory is a powerful technique for inter-process communication (IPC) that allows multiple processes to 
access the same region of memory.
- It's one of the fastest forms of IPC because data doesn't need to be copied between processes; instead,
they directly access the shared memory region.

### shm_open(): Creates or opens a shared memory object.
#include <sys/mman.h>
#include <fcntl.h>

int shm_open(const char *name, int oflag, mode_t mode);

name  : A string representing the name of the shared memory object. Think of it as a file name, but for shared memory.
oflag : Flags that control how the object is opened. Common flags include:
        O_RDONLY: Open for reading only.
        O_RDWR: Open for reading and writing.
        O_CREAT: Create the shared memory object if it doesn't exist.
        O_EXCL: Used with O_CREAT. If the shared memory object already exists, shm_open() will fail.
        O_TRUNC: Truncate the shared memory object to zero length.
mode: Permissions to set on the shared memory object if O_CREAT is specified (e.g., 0666 for read/write access for owner, group, and others).

Return Type:
On success, returns a non-negative integer, which is a file descriptor associated with the shared memory object.
On failure, returns -1 and sets errno to indicate the error. 

### shm_unlink(): Removes a shared memory object.

#include <sys/mman.h>

int shm_unlink(const char *name);

name: The name of the shared memory object (the same name you used in shm_open()).

Return Type:
On success, returns 0.
On failure, returns -1 and sets errno.

### ftruncate(): Sets the size of the shared memory object.

#include <unistd.h>
int ftruncate(int fd, off_t length);

Parameters:

fd: The file descriptor returned by shm_open().
length: The desired size of the shared memory object in bytes.
Return Type:

On success, returns 0.
On failure, returns -1 and sets errno

### mmap(): Maps the shared memory object into the address space of a process.

#include <sys/mman.h>
void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

Parameters:
addr: A suggested starting address for the mapping (usually NULL, letting the kernel choose).
length: The length of the mapping (typically the size you set with ftruncate()).
prot: Memory protection flags (e.g., PROT_READ, PROT_WRITE, PROT_READ | PROT_WRITE).
flags: Mapping flags (e.g., MAP_SHARED, MAP_PRIVATE). MAP_SHARED is what you want for shared memory, so changes are visible to other processes.
fd: The file descriptor from shm_open().
offset: The offset within the shared memory object to start the mapping (usually 0).

Return Type:
On success, returns a pointer to the mapped region in the process's address space.
On failure, returns MAP_FAILED (which is usually (void *) -1) and sets errno.

### munmap(): Unmaps the shared memory object.

#include <sys/mman.h>
int munmap(void *addr, size_t length);

Parameters:
addr: The address returned by mmap().
length: The length of the mapping.

Return Type:
On success, returns 0. 
On failure, returns -1 and sets errno
