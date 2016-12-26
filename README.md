# nudoku #

nudoku is a ncurses based sudoku game.

## Install ##

### Using package manager ###

#### Arch Linux ####
I heard rumors of nudoku being in the [AUR](https://aur.archlinux.org/packages/nudoku-git/).

#### EL 7 (CentOS/Oracle Linux/Scientific Linux/RedHat ####
Since nudoku is in the EPEL repository, you just need to enable the EPEL repository and run:

```
yum install nudoku
```
  
#### Fedora ####
Since nudoku is in the official Fedora repository, you just need to run:

```
dnf install nudoku
```

#### Funtoo/Gentoo ####
Add [strangeland-overlay](https://github.com/jubalh/strangeland-overlay).

Then run:

```
emerge --sync
emerge nudoku -a
```

### openSUSE ###

Since openSUSE Leap 42.1 (and Tumbleweed) you can find nudoku in the official repository.
For others you need to add the `games` repo.
On openSUSE 13.2 for example, you can do it like this:

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

Add `-DEBUG` to `make` to have some debug output.

## Bugtracker ##

https://github.com/jubalh/nudoku/issues

