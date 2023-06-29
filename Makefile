
AS=/usr/bin/as
ASFLAGS=-target i386
CINCLUDES=-Iinclude
CFLAGS=-target i386 -g -O0 $(CINCLUDES)
CFLAGS+= -Werror -Wno-unused-function -Wno-cpp -Wno-unused-parameter -Wall
CFLAGS+= -fomit-frame-pointer -fno-exceptions -fno-asynchronous-unwind-tables -fno-unwind-tables
CFLAGS+= -mllvm -align-all-functions=4 -mllvm -align-all-nofallthru-blocks=4
CFLAGS+= -nostdlib -nodefaultlibs
LD=$(HOME)/.brew/Cellar/llvm/16.0.6/bin/ld.lld
OBJCOPY=$(HOME)/.brew/Cellar/llvm/16.0.6/bin/llvm-objcopy

KERNEL_FILES=kernel_start.c kernel_main.c idt.c
KERNEL_ASFILES=idt.s

all: out/disk.img

clean:
	rm -rf out

out/disk.img: out/boot.bin out/kernel.bin
	cat $^ /dev/zero | head -c 8704 > $@

out/boot.bin: out/boot.o
	$(OBJCOPY) -O binary $^ $@

out/kernel.bin: src/kernel.ld out/kernel.all.o
	$(LD) -static --oformat=binary -T src/kernel.ld -o $@ out/kernel.all.o

out/kernel.all.o: $(patsubst %.c,out/%.o,$(KERNEL_FILES)) $(patsubst %.s,out/%.asm.o,$(KERNEL_ASFILES))
	$(LD) -g -relocatable -o $@ $^

out/boot.o: src/boot/boot.s
	mkdir -p out
	$(AS) $(ASFLAGS) -o $@ $^

out/%.o: src/%.c
	mkdir -p out
	$(CC) -c $(CFLAGS) -o $@ $^

out/%.asm.o: src/%.s
	mkdir -p out
	$(AS) -c $(ASFLAGS) -o $@ $^
