CC       = ccache gcc
TEX      = pdflatex
CFLAGS   = -Wall -Wextra -pedantic -pthread -std=c99 -O2
SOURCES  = $(filter-out exo4.c,$(wildcard *.c))
EXECS    = $(SOURCES:%.c=%.bin)
X11LIB   = `pkg-config --libs x11`
X11FLAGS = `pkg-config --cflags x11`
MATH     = -lm

all: $(EXECS) exo4.bin

$(%.c:%.c=%.bin): %.c
	$(CC) $(CFLAGS) $@.c -o $@.bin

exo4.bin: exo4.c
	$(CC) $(CFLAGS) $< -o $@.bin $(X11LIB) $(X11FLAGS) $(MATH)

report: rapport.tex
	$(TEX) rapport.tex

clean:
	rm *.bin rapport.pdf rapport.aux rapport.log

tarball:
	tar zcvf $(notdir $(shell pwd)).tar.gz exo* out* *.pdf *.tex Makefile

.PHONY: all clean tarball
