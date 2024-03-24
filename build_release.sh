#!/bin/sh

make clean
make format

CONFIG=release STRICT=yes make

