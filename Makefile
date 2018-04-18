DEBUG ?= TRUE

GCC = nspire-gcc
AS  = arm-none-eabi-as -mcpu=arm926ej-s # nspire-as
GXX = nspire-g++
LD  = nspire-ld
GENZEHN = genzehn

GCCFLAGS ?= 
GCCFLAGS += -Wall -W -marm # -DKEYS_H # -mfloat-abi=softfp -mfpu=vfpv3 -nostdlib
O1FLAGS = 
LDFLAGS = #-Wl,-nostdlib -lndls -lsyscalls
ZEHNFLAGS = --name "nspire-z80" --uses-lcd-blit false --240x320-support true

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
	O1FLAGS += -O1
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

$(DISTDIR)/%_o1.o: %_o1.c
	$(GCC) $(GCCFLAGS) $(O1FLAGS) -c $< -o $@



$(EXE).elf: $(addprefix $(DISTDIR)/,$(OBJS))
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $@ $(LDFLAGS)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $^ --output $@.zehn $(ZEHNFLAGS)
	make-prg $@.zehn $@
	rm $@.zehn

clean:
	rm -f $(addprefix $(DISTDIR)/,$(OBJS)) $(DISTDIR)/$(EXE).tns $(DISTDIR)/$(EXE).elf $(DISTDIR)/$(EXE).zehn
