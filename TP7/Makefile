CC       = ccache gcc
TEX      = pdflatex
CFLAGS   = -Wall -Wextra -pedantic -pthread -std=c99 -O2
SOURCES  = $(filter-out exo4.c,$(wildcard *.c))
EXECS    = $(SOURCES:%.c=%.bin)
X11LIB   = `pkg-config --libs x11`
X11FLAGS = `pkg-config --cflags x11`

all: $(EXECS) exo4.bin

doc: outs report

exo1.bin: exo1.c
	$(CC) $(CFLAGS) $< -o $@

exo2.bin: exo2.c
	$(CC) $(CFLAGS) $< -o $@

exo3.bin: exo3.c
	$(CC) $(CFLAGS) $< -o $@

exo4.bin: exo4.c
	$(CC) $(CFLAGS) $< -o $@ $(X11LIB) $(X11FLAGS)

report: rapport.tex
	$(TEX) rapport.tex
	$(TEX) rapport.tex

outs: $(EXECS:%.bin=%.out) exo4.out.png

exo1.out: exo1.bin
	./$< 10 > $@

exo2.out: exo2.bin
	./$< > $@

exo3.out: exo3.bin
	./$< 1 2 3 4 > $@

exo4.out.png: exo4.bin
	./$< & sleep 1 ; import -window `xwininfo -root -tree | grep "()  200x200" | awk '{print $$1}'` exo4.out.png

clean:
	rm *.bin *.pdf *.aux *.log *.out* *.tar.gz

tarball: doc
	tar zcvf $(notdir $(shell pwd)).tar.gz exo* *.pdf *.tex Makefile

.PHONY: all doc clean tarball outs
