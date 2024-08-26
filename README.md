# nudoku #

nudoku is a ncurses based sudoku game.

## Install ##

nudoku is easily installable via many package managers.

[![Packaging status](https://repology.org/badge/tiny-repos/nudoku.svg)](https://repology.org/project/nudoku/versions)

#### Dependency ###
- ncurses
- cairo (optional: for PDF/PNG output)

#### From source ####

Get the latest `.tar.xz` [release](https://github.com/jubalh/nudoku/releases) and extract it.
Then run:

```sh
./configure
make
./src/nudoku
```

For the git version you will have to do:

```sh
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

```sh
./configure --enable-cairo
```

## Usage ##

For normal interactive GUI run `nudoku`.
To print two PDF pages with 8 hard sudokus run `nudoku -p riddle.pdf -n 8 -d hard`.
To get a PNG with an easy sudoku run `nudoku -i sudoku.png -d easy`.
See `man nudoku` to learn more.

## I18n ##

For i18n support you need to set `LANGUAGE` variable:

```sh
export LANGUAGE=lang
```

Where `lang` is the translation of choice. See the list of supported languages in `po/LINGUAS` file.

## Contributing ##

If you plan to add/update the translation please follow these guidelines with examples:

1. Fork this repository and create a new git branch with language abbreviation as a name, e.g. `es`:
```sh
cd nudoku # your forked git repository
git checkout -b es
```
2. For new translation add language to `po/LINGUAS` and create `.po` file:
```sh
echo es >> po/LINGUAS && cp po/nudoku.pot po/es.po
```
3. Replace placeholders in uppercase in the head of the file with your information. Add translations for *(preferably)* each `msgid`.
4. Check your `.po` file with `msgfmt` for any errors:
```sh
msgfmt es.po # should not produce any output
```
5. Push your branch to remote and create PR:
```sh
git push origin es
```

If you add changes to source code make sure to update potfiles as well:
```sh
cd po # if you inside nudoku directory
make update-po
```


## Bugtracker ##

https://github.com/jubalh/nudoku/issues

