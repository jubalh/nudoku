# nudoku #

nudoku is a ncurses based sudoku game.

## Install ##

### Using package manager ###

#### Funtoo/Gentoo ####
Add [strangeland-overlay](https://github.com/jubalh/strangeland-overlay).

Then run:

```
emerge --sync
emerge nudoku -a
```

### From Source ###

Get the lastest [release](https://github.com/jubalh/nudoku/releases) and extract it.
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
./nudoku
```

Add `-DDEBUG` to `make` to have some debug output.

## Bugtracker ##

https://github.com/jubalh/nudoku/issues
