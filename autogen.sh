#!/bin/sh

top_srcdir=$(dirname $_)

mkdir -p m4
autoreconf --force --install $top_srcdir
$top_srcdir/configure
