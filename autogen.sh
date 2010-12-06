#!/bin/sh

set -xe
rm -rf autom4te.cache
aclocal --force -I m4 \
&& libtoolize -c -f \
&& autoheader -f -Wall \
&& automake -a -c -f -Wall \
&& autoconf -f -Wall,no-obsolete
rm -rf autom4te.cache
exit 0

