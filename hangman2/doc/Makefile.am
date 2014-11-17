GENERATED_FILES=report.pdf report.log report.out report.aux

all: report.pdf

report.pdf: report.tex
	pdflatex report.tex
	latex_count=8	                                \
	while egrep -s 'Rerun (LaTeX|to)' report.log && [ $$latex_count -gt 0 ] ;\
		echo '** Re-running LaTeX **';              \
		pdflatex report.tex                         \
	done

clean-local:
	-test -z "$(GENERATED_FILES)" || rm -f $(GENERATED_FILES)
