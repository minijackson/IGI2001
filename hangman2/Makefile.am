SUBDIRS=src
dist_doc_DATA=README

.aux.pdf:
	$(MKDIR_P) $(DOCDIR)
	$(PDFLATEX) $(PDFLATEXOPTS) $(docdir)/$(MANTEXSRC)
	@while ( grep "Rerun to "           \
        $(MANNAME).log  ); do           \
        echo '** Re-running LaTeX **';      \
        $(PDFLATEX) $(PDFLATEXOPTS) $(srcdir)/$(MANTEXSRC); \
    done
