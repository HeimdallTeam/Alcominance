#!/bin/sh

set -ex

aclocal
#libtoolize --force
automake --foreign --add-missing
autoconf


