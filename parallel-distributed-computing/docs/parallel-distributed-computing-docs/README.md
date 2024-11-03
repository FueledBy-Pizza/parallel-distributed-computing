# LaTeX notes template

[![License](https://img.shields.io/badge/license-MIT-green)](https://opensource.org/licenses/MIT)

## Description

A simple macOS LaTeX project template to start writing notes with embedded apps like Inkscape.\
This has been built in order to use a local TeX distribution and not online tools, so that one could manage his whole LaTeX project.

## Installation

**Requirements: Inkscape has to be installed on the system.**

Built with MacTeX.

tex --version\
&emsp;TeX 3.141592653 (TeX Live 2023)
    kpathsea version 6.3.5
    Copyright 2023 D.E. Knuth.
    There is NO warranty.  Redistribution of this software is
    covered by the terms of both the TeX copyright and
    the Lesser GNU General Public License.
    For more information about these matters, see the file
    named COPYING and the TeX source.
    Primary author of TeX: D.E. Knuth.

pdflatex --version\
&emsp;pdfTeX 3.141592653-2.6-1.40.25 (TeX Live 2023)
    kpathsea version 6.3.5
    Copyright 2023 Han The Thanh (pdfTeX) et al.
    There is NO warranty.  Redistribution of this software is
    covered by the terms of both the pdfTeX copyright and
    the Lesser GNU General Public License.
    For more information about these matters, see the file
    named COPYING and the pdfTeX source.
    Primary author of pdfTeX: Han The Thanh (pdfTeX) et al.
    Compiled with libpng 1.6.39; using libpng 1.6.39
    Compiled with zlib 1.2.13; using zlib 1.2.13
    Compiled with xpdf version 4.04

biber --version\
&emsp;biber version: 2.19

## Usage

Ensure the following project structure is in place.
```bash
.
|-- bibliography.bib
|-- chapter
|   |-- chapter1
|   |   |-- prolusion.tex
|-- figure
|   |-- UniNa_logo.svg
|-- main.tex
|-- mypackage.sty
|-- README.md
|-- render.sh
```

Run `render.sh main.tex`. Open your `main.pdf` file.\
The script that was just executed performs the following steps:

### Svg

`--shell-escape` must be set as first option when compiling.

### Bibliography

Typeset `main.tex` (`pdflatex --shell-escape --file-line-error --synctex=1`).\
Run `biber main` (no extension must be provided).\
Typeset `main.tex` again twice.

### Glossaries
Typeset `main.tex` (`pdflatex --shell-escape --file-line-error --synctex=1`).\
Run `makeglossaries main` (no extension must be provided).\
Typeset `main.tex` again twice.

## Credits

Authored by [Giuliano Aiello](https://github.com/FueledBy-Pizza).

#### _Slow like molasses_ <br> - John "The Hangman" Ruth

[![Star History Chart](https://api.star-history.com/svg?repos=FueledBy-Pizza/latex-notes-template&type=Date)](https://star-history.com/#FueledBy-Pizza/latex-notes-template&Date)
