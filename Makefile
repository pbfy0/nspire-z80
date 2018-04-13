DEBUG = TRUE

GCC = nspire-gcc
AS  = arm-none-eabi-as # nspire-as
GXX = nspire-g++
LD  = nspire-ld
GENZEHN = genzehn

GCCFLAGS = -Wall -W -marm
LDFLAGS =
ZEHNFLAGS = --name "nspire-z80"

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
endif

OBJS = $(patsubst %.c, %.o, $(shell find . -name \*.c))
OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))
OBJS += $(patsubst %.s, %.o, $(shell find . -name \*.s))
EXE = nspire-z80
DISTDIR = build
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all: $(EXE).tns

$(DISTDIR)/%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

$(DISTDIR)/%.o: %.cpp
	$(GXX) $(GCCFLAGS) -c $< -o $@
	
$(DISTDIR)/%.o: %.s
	$(AS) -c $< -o $@

$(DISTDIR)/%_malloc.o: %_malloc.c
	$(GCC) $(GCCFLAGS) -O1 -c $< -o $@



$(EXE).elf: $(addprefix $(DISTDIR)/,$(OBJS))
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $@ $(LDFLAGS)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $^ --output $@.zehn $(ZEHNFLAGS)
	make-prg $@.zehn $@
	rm $@.zehn

clean:
	rm -f $(addprefix $(DISTDIR)/,$(OBJS)) $(DISTDIR)/$(EXE).tns $(DISTDIR)/$(EXE).elf $(DISTDIR)/$(EXE).zehn
