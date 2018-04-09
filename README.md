# nudoku #

nudoku is a ncurses based sudoku game.

## Install ##

### Using package manager ###

#### Arch Linux ####
I heard rumors of nudoku being in the [AUR](https://aur.archlinux.org/packages/nudoku-git/).

#### Debian and derivates ####
Contained in official stable repositories since Debian Stretch.

```
apt-get install nudoku
```

#### EL 7 (CentOS/Oracle Linux/Scientific Linux/RedHat ####
You need to enable the EPEL repository and run:

```
yum install nudoku
```
  
#### Fedora ####
nudoku is in the official Fedora repository since June 2016:

```
dnf install nudoku
```

#### Funtoo/Gentoo ####
Contained in official repo since 2018-02-16. Before that you had to use [strangeland-overlay](https://github.com/jubalh/strangeland-overlay).

```
emerge nudoku -a
```

### openSUSE ###

Since openSUSE Leap 42.1 (and Tumbleweed) you can find nudoku in the official repository.
For others you need to add the `games` repo.

```
zypper ar obs://games games
zypper in nudoku
```

### From Source ###

#### Dependency ####
- ncurses

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

## Bugtracker ##

https://github.com/jubalh/nudoku/issues

