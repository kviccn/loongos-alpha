.globl alltraps
alltraps:
  # 构建 trap frame
  pushl %ds
  pushl %es
  pushl %fs
  pushl %gs
  pushal
  
  # 设置数据段为内核数据段
  movw $(2 << 3), %ax
  movw %ax, %ds
  movw %ax, %es

  # 以 trap frame 为参数调用 trap_dispatcher，此时 %esp 指向 trap frame
  pushl %esp
  call trap_dispatcher
  addl $4, %esp

.globl trapret
trapret:
  popal
  popl %gs
  popl %fs
  popl %es
  popl %ds
  addl $0x8, %esp  # 弹出 trapno 和 errcode。关于错误码：处理器不会弹出错误码，需要手动弹出
  iret
