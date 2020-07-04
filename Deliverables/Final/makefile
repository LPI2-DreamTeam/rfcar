# Makefile for latex docs
# src:
# https://danielkaes.wordpress.com/2009/03/14/compiling-latex-documents-using-makefiles/

# PROJ=$(basename $(wildcard *.tex))
PROJ=dissertation
#TEX=pdflatex
TEX=xelatex
BIBTEX=bibtex
MAKEGLOSS=makeglossaries
BUILDTEX=$(TEX) $(PROJ).tex
VIEWPDF=open $(PROJ).pdf

# How to generate 2 different files based on conditionals
# Normal file and file w/ comments: https://tex.stackexchange.com/a/220101
COMM='\def\comm{}\input{$(PROJ)}'
BUILD_COMM=$(TEX) $(COMM)
PROJ_COMM_PDF=$(PROJ)_COMM.pdf
MV_COMM=mv $(PROJ).pdf $(PROJ_COMM_PDF)
VIEWPDF_COMM=open $(PROJ_COMM_PDF)

TEMPFILES=*.log *.bak *.aux *.bbl *.blg *.idx *.toc *.out *.spl *.acn *.ist *.acr *.alg *.nlo *.upa *.lot *.lof *.sym* *.gls *.glo *.glg *.lol *~
EXTRAFILES=*.eps $(PROJ).pdf *.dvi

all:
	$(BUILDTEX)
	$(BIBTEX) $(PROJ)
	$(MAKEGLOSS) $(PROJ)
	$(BUILDTEX)
	$(BUILDTEX)
	$(VIEWPDF)
all-comm:
	@# BUILD FILE WITH COMMENTS
	$(BUILD_COMM) 
	$(BIBTEX) $(PROJ)
	$(MAKEGLOSS) $(PROJ)
	$(BUILD_COMM)
	$(BUILD_COM)
	$(MV_COMM)
	$(VIEWPDF_COMM)
.PHONY: clean clean-aux clean-all
	@# Use recursive calling for clean
	@# see: https://stackoverflow.com/a/8646714
	@# and https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
clean:
	@# Echoing messages in makefile:
	@#source: https://stackoverflow.com/a/47262821
	$(info Removing all temp files...)
	@rm -f $(TEMPFILES)
clean-aux:
	$(info Removing .aux files in all subdirs...)
	@find . -type f -name '*.aux' -delete
clean-all: clean clean-aux
	$(info Removing all files...)
	@rm -f $(EXTRAFILES) 

