#!/bin/sh

set -e

cd "$( dirname "$0" )"
make ./main 
clear
./main
