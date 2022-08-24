# nudoku #

nudoku is a ncurses based sudoku game.

## Install ##

nudoku is easily installable via many package managers.

[![Packaging status](https://repology.org/badge/vertical-allrepos/nudoku.svg)](https://repology.org/project/nudoku/versions)

### From Source ###

#### Dependency ####
- ncurses
- cairo (optional: for PDF/PNG output)

#### Compilation ####

Get the latest .tar.xz [release](https://github.com/jubalh/nudoku/releases) and extract it.
Then run:

```
./configure
make
./src/nudoku
```

For the git version you will have to do:

```
git clone https://github.com/jubalh/nudoku
cd nudoku
autoreconf -i
./configure
make
./src/nudoku
```

Add `-DDEBUG` to `make` to have some debug output.

nudoku can also output sudokus in PDF and PNG format (see `man nudoku` -p/-i/-n).
You need to have cairo installed and configure with:

```
./configure --enable-cairo
```

##### Usage #####

For normal interactive GUI run `nudoku`.
To print two PDF pages with 8 hard sudokus run `nudoku -p riddle.pdf -n 8 -d hard`.
To get a PNG with an easy sudoku run `nudoku -i sudoku.png -d easy`.
See `man nudoku` to learn more.

##### I18n #####

For i18n support use `make` like this:

```
make install
LANGUAGE=es
./src/nudoku
```

You can replace `es` with any other language suported (suported languages are listed in `po/LINGUAS` file).

## Bugtracker ##

https://github.com/jubalh/nudoku/issues

