################################################################################
## CONFIG                                                                     ##
################################################################################

NAME    := vibe
VERSION := 0.1.0

CONFIG ?= debug
STRICT ?= no

################################################################################
## PROGRAMS                                                                   ##
################################################################################

RM_RF   ?= rm -rf
MKDIR_P ?= mkdir -p

OS := $(shell uname -s)

FORMAT := clang-format -i

################################################################################
## FILES & FOLDERS                                                            ##
################################################################################

SRCDIR := $(CURDIR)/src
APPDIR := $(CURDIR)/app
CFGDIR := $(CURDIR)/etc
INCDIR := $(SRCDIR)
HDRDIR := $(INCDIR)
OUTDIR := $(CURDIR)/bin
OBJDIR := $(CURDIR)/obj

CFG_SRCS := $(wildcard $(CFGDIR)/*.c)
CFG_HDRS := $(wildcard $(CFGDIR)/*.h)
CFG_OBJS := $(CFG_SRCS:$(CURDIR)/%.c=$(OBJDIR)/$(CONFIG)/%.o)
CFG_DEPS := $(CFG_OBJS:%.o=%.d)

LIB_SRCS := $(wildcard $(SRCDIR)/*.c)
LIB_HDRS := $(wildcard $(HDRDIR)/*.h)
LIB_OBJS := $(LIB_SRCS:$(CURDIR)/%.c=$(OBJDIR)/$(CONFIG)/%.o)
LIB_DEPS := $(LIB_OBJS:%.o=%.d)

APP_SRCS := $(wildcard $(APPDIR)/*.c)
APP_HDRS := $(wildcard $(APPDIR)/*.h)
APP_OBJS := $(APP_SRCS:$(CURDIR)/%.c=$(OBJDIR)/$(CONFIG)/%.o)
APP_DEPS := $(APP_OBJS:%.o=%.d)

BIN := $(OUTDIR)/$(NAME)-$(VERSION)-$(CONFIG)

################################################################################
## COMPILER                                                                   ##
################################################################################

CC ?= cc
LD ?= ld

CFLAGS  :=
LDFLAGS :=

CFLAGS += -std=c17 -c -MMD
CFLAGS += -I$(INCDIR) -I$(CFGDIR)
CFLAGS += -Werror

ifeq ($(CONFIG), release)
	CFLAGS += -O3 -DNDEBUG
else
	CFLAGS += -O0 -g
endif

ifeq ($(STRICT), yes)
	CFLAGS += -Wall -Wextra -pedantic
endif

ifeq ($(OS), Darwin)
	LDFLAGS += -L/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/lib
	LDFLAGS += -lSystem
endif

################################################################################
## TARGETS                                                                    ##
################################################################################

.PHONY: all clean format run 

all: $(BIN)

run: $(BIN)
	%<

$(BIN): $(APP_OBJS) $(LIB_OBJS) $(CFG_OBJS)
	@$(MKDIR_P) $(@D)
	$(LD) -o $@ $(LDFLAGS) $^

$(OBJDIR)/$(CONFIG)/%.o: $(CURDIR)/%.c
	@$(MKDIR_P) $(@D)
	$(CC) -o $@ $(CFLAGS) $<

clean:
	$(RM_RF) $(OUTDIR)
	$(RM_RF) $(OBJDIR)

format:
	$(FORMAT) $(APP_SRCS)
	$(FORMAT) $(APP_HDRS)
	$(FORMAT) $(LIB_SRCS)
	$(FORMAT) $(LIB_HDRS)
	$(FORMAT) $(CFG_SRCS)
	$(FORMAT) $(CFG_HDRS)

-include $(APP_DEPS)
-include $(LIB_DEPS)
-include $(CFG_DEPS)
