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

### openSUSE ###

Add the games repository and install nudoku via zypper. For 13.2 this can be done via:

```
zypper ar http://download.opensuse.org/repositories/games/openSUSE_13.2/ games
zypper in nudoku
```

### From Source ###

#### Dependency ####
- ncurses

#### Compilation ####

Get the lastest .tar.xz [release](https://github.com/jubalh/nudoku/releases) and extract it.
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

## Bugtracker ##

https://github.com/jubalh/nudoku/issues

