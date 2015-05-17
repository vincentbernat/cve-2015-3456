CFLAGS   = -O0 -g -Wall -std=c99
CPPFLAGS = -D_GNU_SOURCE
TARGETS  = patch
OBJECTS  = patch.o helpers.o

all: $(TARGETS)
clean:
	rm -f $(OBJECTS) $(TARGETS)
patch: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: all clean
