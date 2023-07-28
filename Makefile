
AS=/usr/bin/as
ASFLAGS=-target i386
CINCLUDES=-Iinclude
CFLAGS=-target i386 -g -O0 $(CINCLUDES)
CFLAGS+= -Werror -Wno-unused-function -Wno-cpp -Wno-unused-parameter -Wall
CFLAGS+= -fomit-frame-pointer -fno-exceptions -fno-asynchronous-unwind-tables -fno-unwind-tables
CFLAGS+= -mllvm -align-all-functions=4 -mllvm -align-all-nofallthru-blocks=4
CFLAGS+= -nostdlib -nodefaultlibs
LD=$(HOME)/.brew/Cellar/llvm/16.0.6/bin/ld.lld
LDFLAGS=-m elf_i386
OBJCOPY=$(HOME)/.brew/Cellar/llvm/16.0.6/bin/llvm-objcopy

KERNEL_SRC=kernel_main.c term.c idt.c intr.c string.c heap.c paging.c disk.c fs.c
KERNEL_ASM=idt.s intr.s io.s paging.s
KERNEL_OBJ=$(patsubst %.c,out/%.o,$(KERNEL_SRC)) $(patsubst %.s,out/%.s.o,$(KERNEL_ASM))

all: out/disk.img out/kernel.dbg

clean:
	rm -rf out

out/disk.img: out/boot.bin out/kernel.bin
	cat $^ /dev/zero | head -c 131072 > $@

out/kernel.dbg: out/kernel
	$(OBJCOPY) --only-keep-debug $^ $@

out/boot.bin: out/boot.s.o
	$(OBJCOPY) -O binary $^ $@

out/kernel.bin: out/kernel
	$(OBJCOPY) -O binary $^ $@

out/kernel: src/kernel.ld $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) -static --image-base=1048576 -o $@ $^

out/%.o: src/%.c
	mkdir -p out
	$(CC) -c $(CFLAGS) -o $@ $^

out/%.s.o: src/%.s
	mkdir -p out
	$(AS) -c $(ASFLAGS) -o $@ $^
