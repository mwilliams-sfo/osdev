
AS=/usr/bin/as
ASOPTS=-target i386
LD=$(HOME)/.brew/Cellar/llvm/16.0.6/bin/ld.lld
OBJCOPY=$(HOME)/.brew/Cellar/llvm/16.0.6/bin/llvm-objcopy

all: out/disk.img

clean:
	rm -rf out

out/disk.img: out/boot.bin out/kernel.bin
	cat $^ > $@

out/boot.bin: out/boot.o
	$(OBJCOPY) -O binary $^ $@

out/kernel.bin: src/kernel.ld out/kernel.all.o
	$(LD) --oformat=binary -T src/kernel.ld -o $@ out/kernel.all.o

out/kernel.all.o: out/kernel.o
	$(LD) -g -relocatable -o $@ $^

out/boot.o: src/boot/boot.s
	mkdir -p out
	$(AS) $(ASOPTS) -o $@ $^

out/%.o: src/%.s
	mkdir -p out
	$(AS) $(ASOPTS) -o $@ $^
