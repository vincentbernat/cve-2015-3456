CFLAGS = -O0 -g -Wall -std=c99 -fpic
LDFLAGS = -Wl,-z,relro -Wl,-z,now
TARGETS = not-qemu not-qemu-pie
OBJECTS = not-qemu.o fdc.o

all: $(TARGETS) $(TARGETS:%=%-stripped)
clean:
	rm -f $(OBJECTS) $(TARGETS) $(TARGETS:%=%-stripped)

# The regular version and the PIE version
not-qemu: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^
not-qemu-pie: $(OBJECTS)
	$(CC) $(LDFLAGS) -pie -fpie -o $@ $^

# How to build a stripped version
%-stripped: %
	cp $< $@
	strip -s $@

.PHONY: all clean
