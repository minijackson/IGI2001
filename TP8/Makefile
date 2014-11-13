CC             = ccache gcc
TEX            = pdflatex
CFLAGS         = -Wall -Wextra -pedantic -pthread -std=c99
PROFILE_CFLAGS = -O -pg
DEBUG_CFLAGS   = -O -g
RELEASE_CFLAGS = -O2
SOURCES        = $(wildcard *.c)
EXECS          = $(SOURCES:%.c= %.bin)
DEPS           = lib/utils.c
X11LIB         = `pkg-config --libs x11`
X11FLAGS       = `pkg-config --cflags x11`
MATHFLAGS      = -lm

all: debug

release: CFLAGS += $(RELEASE_CFLAGS)

debug: CFLAGS += $(DEBUG_CFLAGS)
debug: $(EXECS)

profile: CFLAGS += $(PROFILE_CFLAGS)

doc: outs rapport.pdf

.o: .c
	$(CC) -c $< -o $@ $(CFLAGS)

%.bin: %.o $(DEPS)
	$(CC) $^ -o $@ $(CFLAGS)

outs: $(EXECS:%.bin=%.out)

%.out: %.bin
	./$< > $@

rapport.pdf: rapport.tex outs $(SOURCES)
	$(TEX) rapport.tex
	latex_count=8 ; \
	while egrep -s 'Rerun (LaTeX|to get cross-references right)' rapport.log && [ $$latex_count -gt 0 ] ;\
		do \
			echo "Rerunning latex...." ;\
			$(TEX) rapport.tex ;\
			latex_count=`expr $$latex_count - 1` ;\
	done

clean:
	rm *.o *.bin *.pdf *.aux *.log *.out* *.toc *.tar.gz

tarball: doc
	tar zcvf $(notdir $(shell pwd)).tar.gz exo* *.pdf *.tex Makefile

.PHONY: all release debug profile doc clean tarball outs
