KNAME := uOS
CC := gcc
LD := ld
AR := ar
ASM := nasm
OBJCOPY := objcopy
QEMU := qemu-system-i386
E := @
ROOT := $(PWD)
OBJ := obj
BIN := bin
LIB := lib
BOOT := boot
KERN := kernel
OBJ_DIR := $(ROOT)/$(OBJ)
BIN_DIR := $(ROOT)/$(BIN)
LIB_DIR := $(ROOT)/$(LIB)
BOOT_DIR := $(ROOT)/$(BOOT)
KERN_DIR := $(ROOT)/$(KERN)

CFLAGS := -c -std=c99 -m32 -ggdb -gstabs -nostdinc -fno-builtin -fno-stack-protector -fno-pie -Os -Iinclude
LDFLAGS := -m elf_i386 -nostdlib 
ASMFLAGS := -f elf -g -F stabs
ARFLAGS := rcs

C_SOURCES = $(shell find -name *.c)
S_SOURCES = $(shell find -name *.s)
C_OBJECTS = $(patsubst %.c, %.o, $(C_SOURCES))
S_OBJECTS = $(patsubst %.s, %.o, $(S_SOURCES))
get_cobjects = $(addprefix $(OBJ)/, $(patsubst %.c, %.o, $(shell find $(1) -name *.c)))
get_sobjects = $(addprefix $(OBJ)/, $(patsubst %.s, %.o, $(shell find $(1) -name *.s)))
# get_aobjects = $(addprefix $(OBJ)/, $(patsubst %.c, %.a, $(shell find $(1) -name *.c)))
get_objects = $(call get_cobjects, $(1)) $(call get_sobjects, $(1))

# D_OBJECTS = $(patsubst %.c, %.d, $(C_SOURCES))

all: prepare $(C_OBJECTS) $(S_OBJECTS) lib image

prepare:
	$(E)mkdir -p $(OBJ_DIR) $(BIN_DIR) $(OBJ_DIR)/$(LIB) $(OBJ_DIR)/$(KERN) $(OBJ_DIR)/$(BOOT)

.c.o:
	$(E)echo 'compile' $@{$^}...
	$(E)$(CC) $(CFLAGS) $^ -o $(OBJ)/$@

.s.o:
	$(E)echo 'compile' $@{$^}...
	$(E)$(ASM) $(ASMFLAGS) $^
	$(E)mv $@ $(OBJ)/$@

# %.d:%.c
# 	$(E)echo 'generate' $@{$^}...
# 	$(E)$(CC) $(CFLAGS) -MM $< |sed -e 1's,^,$(OBJ)/,' > $(OBJ_DIR)/$(notdir $@)
lib_objects = $(call get_objects, $(LIB))
lib: $(lib_objects)
	$(E)echo 'generate' $@{$^}
	$(E)$(AR) $(ARFLAGS) $(OBJ)/$@.a $^

boot_objects = $(call get_objects, $(BOOT))
bootloader: $(boot_objects)
	$(E)echo 'generate' $@{$^}...
	$(E)$(LD) $(LDFLAGS) -N -T scripts/boot.lds $^ -o $@
	$(E)$(OBJCOPY) -S -O binary $@
	$(E)dd if=/dev/zero of=$@.o bs=510 count=1
	$(E)dd if=$@ of=$@.o conv=notrunc
	$(E)mv $@.o $@
	$(E)echo -ne '\x55\xaa' >> $@
	$(E)mv $@ $(BIN_DIR) 


kernel_objects = $(call get_objects, $(KERN))
core: $(kernel_objects) $(OBJ)/lib.a
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
