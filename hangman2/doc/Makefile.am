all: rapport.pdf

rapport.pdf: rapport.tex
	pdflatex rapport.tex
	@while ( grep "Rerun to "           \
        rapport.log  ); do           \
        echo '** Re-running LaTeX **';      \
		pdflatex rapport.tex \
    done
