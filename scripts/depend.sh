#!/bin/sh

# Evaluate C++ dependencies, for use in Makefile

# TODO: 
# 1. make this dependent on CC? (looks to be a bug in clang++ where -MM includes system headers)

DIR="$1"
shift 1
case "$DIR" in
    "" | ".")
        gcc -MM -MG "$@" | sed -e 's@ˆ\(.*\)\.o:@\1.d \1.o:@' ;;
    *)
        gcc -MM -MG "$@" | sed -e "s@ˆ\(.*\)\.o:@$DIR/\1.d \ $DIR/\1.o:@" ;;
esac
