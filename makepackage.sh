#!/bin/bash
if [ -d package/ ]; then
	rm package/ -r
fi
if [ -e hxfcgi.zip ]; then
	rm hxfcgi.zip
fi
mkdir package
cp ndll package/ -r
cp lib/* package/ -r
cp src* package/ -r
cp haxelib.xml package/
cp Makefile package/
cp LICENSE package/
rm package/src/*.o
cd package
zip ../hxfcgi.zip * -r
cd ..
rm package/ -r