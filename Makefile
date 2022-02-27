CC=clang
CFLAGS=-Wall -c -g -m32 -ffreestanding -Iinclude
ASFLAGS=-Wall -g -m32
LDFLAGS=-T scripts/kernel.ld

DIST_DIR=dist
ISO_TARGET=$(DIST_DIR)/loongos-alpha.iso
TARGET=$(DIST_DIR)/kernel.elf
ENTRY=kernel/entry.o
VECTORS=kernel/vectors.o

c_srcs=$(shell find kernel -name *.c)
c_objs=$(patsubst %.c, %.o, $(c_srcs))
asm_srcs=$(shell find kernel -name *.S)
asm_objs=$(patsubst %.S, %.o, $(asm_srcs))
objects=$(c_objs) $(asm_objs) $(VECTORS)

all: $(objects)

$(TARGET): $(ENTRY) $(objects)
	-mkdir $(DIST_DIR)
	ld $(LDFLAGS) $^ -o $@
	objdump -S -Msuffix $@ > $(patsubst %.elf, %.asm, $@)

BUILD_VECTORS_TOOL=scripts/build-vectors
VECTORS_ASM=kernel/vectors.S

$(VECTORS_ASM): $(BUILD_VECTORS_TOOL)
	$< > $@

$(BUILD_VECTORS_TOOL):
	cc scripts/build-vectors.c -o $@

.PHONY: $(ISO_TARGET) simulate boot-iso clean

$(ISO_TARGET): $(TARGET)
	sh scripts/mkiso.sh $< $@

simulate: $(TARGET)
	DISPLAY=:0 qemu-system-x86_64 -m 128M -kernel $< -monitor stdio

boot-iso: $(ISO_TARGET)
	DISPLAY=:0 qemu-system-x86_64 -m 128M -boot d -cdrom $< -monitor stdio

clean:
	rm -rf  $(DIST_DIR) $(TARGET) $(objects) $(BUILD_VECTORS_TOOL) $(VECTORS_ASM)
