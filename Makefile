
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

KERNEL_FILES=kernel_main.c term.c idt.c intr.c heap.c
KERNEL_ASFILES=idt.s intr.s io.s

all: out/disk.img

clean:
	rm -rf out

out/disk.img: out/boot.bin out/kernel.bin
	cat $^ /dev/zero | head -c 131072 > $@

out/boot.bin: out/boot.s.o
	$(OBJCOPY) -O binary $^ $@

out/kernel.bin: src/kernel.ld out/kernel.all.o
	$(LD) -static --oformat=binary -T src/kernel.ld -o $@ out/kernel.all.o

out/kernel.all.o: $(patsubst %.c,out/%.o,$(KERNEL_FILES)) $(patsubst %.s,out/%.s.o,$(KERNEL_ASFILES))
	$(LD) -g -relocatable -o $@ $^

out/%.o: src/%.c
	mkdir -p out
	$(CC) -c $(CFLAGS) -o $@ $^

out/%.s.o: src/%.s
	mkdir -p out
	$(AS) -c $(ASFLAGS) -o $@ $^
