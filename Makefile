
STD_FLAGS ?= --std=c11
WARN_FLAGS ?= -Wall -Wextra

CHECK_FLAGS ?= -I/opt/local/include -L/opt/local/lib -lcheck

ifndef OPT_FLAGS
  OPT_FLAGS := -g -fno-omit-frame-pointer

  ifdef OPT
    ifneq ($(OPT),0)
      OPT_FLAGS := -O3
    endif
  endif
endif

ifdef SAN
  # SAN=address, SAN=undefined, SAN=leak
  WARN_FLAGS := $(WARN_FLAGS) -fsanitize=$(SAN)
endif

CFLAGS := $(STD_FLAGS) $(OPT_FLAGS) $(WARN_FLAGS) $(CFLAGS)

all: main test

main: main.c vector.h
	$(CC) $(CFLAGS) $< -o $@

test: tests/check.c vector.h
	$(CC) $(CFLAGS) $(CHECK_FLAGS) $< -o $@

clean:
	-rm -f main
	-rm -f test

.PHONY: clean all
.DEFAULT: all
