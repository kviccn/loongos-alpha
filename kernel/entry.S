/* Align all boot modules on i386 page (4KB) boundaries. */
#define MULTIBOOT_PAGE_ALIGN        0x00000001

/* Must pass memory information to OS. */
#define MULTIBOOT_MEMORY_INFO       0x00000002

#define MULTIBOOT_HEADER_MAGIC      0x1BADB002
#define MULTIBOOT_HEADER_FLAGS      MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
#define MULTIBOOT_HEADER_CHECKSUM   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

#define INIT_STACK_SIZE                  0x1000  // 4KB

.section .init.text
.globl _start
.long MULTIBOOT_HEADER_MAGIC
.long MULTIBOOT_HEADER_FLAGS
.long MULTIBOOT_HEADER_CHECKSUM

_start:
cli

// 初始化栈指针
movl $(stack + INIT_STACK_SIZE), %esp

pushl %eax
pushl %ebx
call kernel_init
addl $8, %esp

hlt

.section .init.data
stack:
  .fill INIT_STACK_SIZE, 1
