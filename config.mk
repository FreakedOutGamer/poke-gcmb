# Shared by every multiboot decompilation under this directory.
#
# The tools are built from source inside this directory by `make tools` in
# decomp/ (see README.md): agbcc from the tools/agbcc-src submodule
# (pret/agbcc), installed into tools/agbcc, and pret's gbagfx and preproc in
# tools/gbagfx and tools/preproc. Nothing outside decomp/ is needed.

# decomp/ itself; projects include this file as ../config.mk.
DECOMP_DIR := $(patsubst %/,%,$(dir $(abspath $(lastword $(MAKEFILE_LIST)))))

AGBCC_DIR ?= $(DECOMP_DIR)/tools/agbcc
GBAGFX ?= $(DECOMP_DIR)/tools/gbagfx/gbagfx
# pret's preproc expands INCBIN_* and _("...") in C after the preprocessor,
# as pokeemerald does.
PREPROC ?= $(DECOMP_DIR)/tools/preproc/preproc

CC1     := $(AGBCC_DIR)/bin/agbcc
OLD_CC1 := $(AGBCC_DIR)/bin/old_agbcc
CC1_ARM := $(AGBCC_DIR)/bin/agbcc_arm

# Common pret-style compiler defaults. Individual objects may override these
# when matching shows that a different compiler revision or optimization was
# used.
CPP      := cpp
# ../common/include holds headers shared by every project (pret's gba/ set);
# a project's own include/ is searched first. ../common/src holds the data
# tables (src/data/) shared sources include, as pret's src/*.c do.
CPPFLAGS += -I include -I ../common/include -I $(AGBCC_DIR)/include -iquote include -iquote ../common/src \
            -nostdinc -undef -Wno-trigraphs -std=gnu89
CFLAGS   ?= -mthumb-interwork -O2 -fhex-asm -Wimplicit -Wparentheses

# Stop early, with the fix, if the tools have not been built yet.
BUILT_TOOLS := $(AGBCC_DIR)/bin/agbcc $(GBAGFX) $(PREPROC)
ifeq ($(filter clean,$(MAKECMDGOALS)),)
ifneq ($(words $(wildcard $(BUILT_TOOLS))),$(words $(BUILT_TOOLS)))
$(error The tools are not built: run `make tools` in $(DECOMP_DIR))
endif
endif
