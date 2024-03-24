#!/bin/sh

make clean
make format

CONFIG=debug STRICT=no make
