#!/bin/sh

set -e

path=$(pwd)
tempdir=$(mktemp -d)

cd $(tempdir)
mkdir -p src
cd src
wget -O main.cpp https://raw.githubusercontent.com/cornusandu-org/docs/refs/heads/main/src/main.cpp
cd ..
wget -O Makefile https://raw.githubusercontent.com/cornusandu-org/docs/refs/heads/main/Makefile
wget -O README.md https://raw.githubusercontent.com/cornusandu-org/docs/refs/heads/main/README.md
make build
make install
cd "$path"