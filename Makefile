DEBUG = TRUE
GCC = nspire-gcc
AS = nspire-as
GXX=nspire-g++
LD = nspire-ld-bflt
GCCFLAGS = -Wall -W -marm
LDFLAGS =
ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -O3
else
	GCCFLAGS += -O0 -g
	LDFLAGS += --debug
endif
EXE = nspire-z80.tns
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
DISTDIR = .
vpath %.tns $(DISTDIR)

all: $(EXE)

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $<
drz80.o:
	$(AS) -c drz80.s
$(EXE): $(OBJS) drz80.o
	mkdir -p $(DISTDIR)
	$(LD) $(LDFLAGS) $^ -o $(DISTDIR)/$@

clean:
	rm -f *.o *.elf *.gdb
	rm -f $(DISTDIR)/$(EXE)
