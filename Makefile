DEBUG ?= TRUE

GCC = nspire-gcc
AS  = arm-none-eabi-as -mcpu=arm926ej-s # nspire-as
GXX = nspire-g++
LD  = nspire-ld
GENZEHN = genzehn

NSPIREIO ?= FALSE
NAVNETIO ?= FALSE

GCCFLAGS ?= 
GCCFLAGS += -Wall -W -marm # -include _nn_insert.h# -DKEYS_H # -mfloat-abi=softfp -mfpu=vfpv3 -nostdlib
O1FLAGS = 
LDFLAGS = # -Wl,--nspireio # -Wl,-wrap,printf -Wl,-wrap,puts # -Wl,-wrap,printf -Wl,-wrap,puts#-Wl,-nostdlib -lndls -lsyscalls
ZEHNFLAGS = --name "nspire-z80" --uses-lcd-blit false --240x320-support true

SRC_DIR = src
DEPLOY_DIR =

ifeq ($(NSPIREIO),TRUE)
	LDFLAGS += -Wl,--nspireio
	GCCFLAGS += -DNO_LCD
endif

ifeq ($(NAVNETIO),TRUE)
	LDFLAGS += -Wl,-wrap,printf -Wl,-wrap,puts
	GCCFLAGS += -DUSE_NAVNETIO
endif

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
	O1FLAGS += -O1
endif

OBJS = $(patsubst %.c, %.o, $(shell find $(SRC_DIR) -name \*.c))
OBJS += $(patsubst %.cpp, %.o, $(shell find $(SRC_DIR) -name \*.cpp))
OBJS += $(patsubst %.s, %.o, $(shell find $(SRC_DIR) -name \*.s))
EXE = nspire-z80
DISTDIR = build
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all: $(EXE).tns

$(DISTDIR)/%.o: $(SRC_DIR)/%.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

$(DISTDIR)/%.o: $(SRC_DIR)/%.cpp
	$(GXX) $(GCCFLAGS) -c $< -o $@
	
$(DISTDIR)/%.o: $(SRC_DIR)/%.s
	$(AS) -c $< -o $@

$(DISTDIR)/%_o1.o: $(SRC_DIR)/%_o1.c
	$(GCC) $(GCCFLAGS) $(O1FLAGS) -c $< -o $@


$(EXE).elf: $(patsubst $(SRC_DIR)/%,$(DISTDIR)/%, $(OBJS))
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $@ $(LDFLAGS)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $^ --output $@.zehn $(ZEHNFLAGS)
	make-prg $@.zehn $@
	rm $@.zehn

.PHONY: deploy

deploy: $(EXE).tns
	NavNet_launcher.exe NavNet_upload.exe "$(shell wslpath -w $(EXE).tns)" $(EXE).tns

clean:
	rm -f $(patsubst $(SRC_DIR)/%,$(DISTDIR)/%, $(OBJS)) $(EXE).tns $(EXE).elf $(EXE).tns.zehn
