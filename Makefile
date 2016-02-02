DEBUG = TRUE
GCC = nspire-gcc
AS = nspire-as
GXX=nspire-g++
LD = nspire-ld-bflt
GCCFLAGS = -Wall -W -marm -Wno-braces
LDFLAGS =
ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -O3
else
	GCCFLAGS += -O0 -g
	LDFLAGS += --debug
endif
EXE = nspire-z80.tns
OBJS = $(patsubst %.c,%.o,$(wildcard *.c))
DISTDIR = build
vpath %.tns $(DISTDIR)

all: $(EXE)

$(DISTDIR)/%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@
$(DISTDIR)/drz80.o:
	$(AS) -c drz80.s -o $(DISTDIR)/drz80.o
$(EXE): $(addprefix $(DISTDIR)/,$(OBJS)) $(DISTDIR)/drz80.o
	mkdir -p $(DISTDIR)
	$(LD) $(LDFLAGS) $^ -o $(DISTDIR)/$@

clean:
	rm -f $(DISTDIR)/*.o $(DISTDIR)/*.elf $(DISTDIR)/*.gdb
	rm -f $(DISTDIR)/$(EXE)
