.PHONY: build install clean

build:
	g++ ./src/main.cpp -o ./docs -Wno-conversion-null -Wno-write-strings -Wno-pointer-arith

install:
	sudo cp ./docs /usr/bin/docs
	sudo mkdir -p /usr/docs
