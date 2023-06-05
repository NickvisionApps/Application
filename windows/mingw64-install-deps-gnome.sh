#!/bin/sh

cd $(dirname $0)
cd ..
[ -z $NICK_BUILDDIR ] && NICK_BUILDDIR="_nickbuild"
[ -z $DESTDIR ] && DESTDIR=$PWD/$NICK_BUILDDIR

mkdir -p $DESTDIR/var/lib/pacman
mkdir -p $DESTDIR/var/cache/pacman/pkg
pacman --root $DESTDIR --cachedir $DESTDIR/var/cache/pacman/pkg -Sy $(pactree -l mingw-w64-x86_64-libadwaita) --noconfirm
rm -r $DESTDIR/tmp $DESTDIR/usr $DESTDIR/var