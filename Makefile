KNAME := uOS
CC := gcc
LD := ld
ASM := nasm
OBJCOPY := objcopy
QEMU := qemu-system-i386
E := @
ROOT := $(PWD)
OBJ := obj
BIN := bin
OBJ_DIR := $(ROOT)/$(OBJ)
BIN_DIR := $(ROOT)/$(BIN)

CFLAGS := -c -std=c99 -m32 -ggdb -gstabs -nostdinc -fno-builtin -fno-stack-protector -Os -Iinclude
LDFLAGS := -m elf_i386 -nostdlib 
ASMFLAGS := -f elf -g -F stabs

C_SOURCES = $(shell find -name *.c)
S_SOURCES = $(shell find -name *.s)
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))
D_OBJECTS = $(patsubst %.c, %.d, $(C_SOURCES))

all: prepare $(C_OBJECTS) $(S_OBJECTS) $(D_OBJECTS) image

prepare:
	$(E)mkdir -p $(OBJ_DIR) $(BIN_DIR)

.c.o:
	$(E)echo 'compile' $@{$^}...
	$(E)$(CC) $(CFLAGS) $< -o $(OBJ_DIR)/$(notdir $@)

.s.o:
	$(E)echo 'compile' $@{$^}...
	$(E)$(ASM) $(ASMFLAGS) $<
	$(E)mv $@ $(OBJ_DIR)/$(notdir $@)

%.d:%.c
	$(E)echo 'generate' $@{$^}...
	$(E)$(CC) $(CFLAGS) -MM $< |sed -e 1's,^,$(OBJ)/,' > $(OBJ_DIR)/$(notdir $@)


bootloader: $(OBJ)/bootasm.o $(OBJ)/bootmain.o
	$(E)echo 'generate' $@{$^}...
	$(E)$(LD) $(LDFLAGS) -N -e start -Ttext 0x7c00 $^ -o $@
	$(E)$(OBJCOPY) -S -O binary $@
	$(E)dd if=/dev/zero of=$@.o bs=510 count=1
	$(E)dd if=$@ of=$@.o conv=notrunc
	$(E)mv $@.o $@
	$(E)echo -ne '\x55\xaa' >> $@
	$(E)mv $@ $(BIN_DIR) 

//todo: auto generate core dep
		 
core: $(OBJ)/init.o \
$(OBJ)/printk.o $(OBJ)/string.o $(OBJ)/console.o \
$(OBJ)/debug.o \
$(OBJ)/gdt.o $(OBJ)/gdt_s.o \
$(OBJ)/idt.o $(OBJ)/idt_s.o \
$(OBJ)/timer.o

	$(E)echo 'generate' $@{$^}...
	$(E)$(LD) $(LDFLAGS) -T scripts/kernel.lds $^ -o $(BIN_DIR)/$@

image: bootloader core
	$(E)echo 'generate' $@{$^}...
	$(E)dd if=/dev/zero of=$(KNAME).img count=10000
	$(E)dd if=$(BIN)/$(word 1, $^) of=$(KNAME).img conv=notrunc
	$(E)dd if=$(BIN)/$(word 2, $^) of=$(KNAME).img seek=1 conv=notrunc

qemu:
	$(QEMU) -hda $(KNAME).img -no-reboot

debug:
	$(QEMU) -S -s -hda $(KNAME).img -no-reboot

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(KNAME).img
