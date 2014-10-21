CC       = ccache gcc
TEX      = pdflatex
CFLAGS   = -Wall -Wextra -pedantic -pthread -std=c99 -O2
SOURCES  = $(filter-out exo4.c,$(wildcard *.c))
EXECS    = $(SOURCES:%.c=%)
X11LIB   = `pkg-config --libs x11`
X11FLAGS = `pkg-config --cflags x11`
MATH     = -lm

all: $(EXECS) exo4

doc: outs report

$(%.c:%.c=%): %.c
	$(CC) $(CFLAGS) $@.c -o $@

exo4: exo4.c
	$(CC) $(CFLAGS) $< -o $@ $(X11LIB) $(X11FLAGS) $(MATH)

report: rapport.tex
	$(TEX) rapport.tex
	$(TEX) rapport.tex

outs: $(EXECS:exo%=out%) out4.png

out1: exo1
	./$< 10 > $@

out2: exo2
	./$< > $@

out3: exo3
	./$< 1 2 3 4 > $@

out4.png: exo4
	./exo4 & sleep 1 ; import -window `xwininfo -root -tree | grep "()  200x200" | awk '{print $$1}'` out4.png

clean:
	rm exo? out* *.pdf *.aux *.log *.out

tarball: doc
	tar zcvf $(notdir $(shell pwd)).tar.gz exo* out* *.pdf *.tex Makefile

.PHONY: all doc clean tarball outs
