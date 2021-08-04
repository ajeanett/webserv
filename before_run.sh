#!/bin/bash

dirs=(www cgi_scripts default.conf testers YoupiBanane)

echo "--- Removing files from cmake-build-debug/ ---"

for i in ${dirs};
do
	rm -rf ./cmake-build-debug/$i;
done;

echo

echo "--- Copying files to cmake-build-debug/ ---"

mkdir -p ./YoupiBanane/put_dir
for i in ${dirs};
do
	cp -r ./$i ./cmake-build-debug/$i;
done;

echo

echo "--- chmod-ing scripts ---"
chmod +x ./cmake-build-debug/cgi_scripts/*
chmod +x ./cmake-build-debug/testers/*
echo "--- Done ---"
