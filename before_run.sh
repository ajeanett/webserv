#!/bin/bash

dirs=(www cgi_scripts ex.conf testers YoupiBanane)

echo "--- Removing files from cmake-build-debug/ ---"

for i in ${dirs};
do
	rm -rf ./cmake-build-debug/$i;
done;

echo

echo "--- Copying files to cmake-build-debug/ ---"

for i in ${dirs};
do
	cp -r ./$i ./cmake-build-debug/$i;
done;

echo

echo "--- chmod-ing scripts ---"
chmod +x ./cmake-build-debug/cgi_scripts/*
chmod +x ./cmake-build-debug/testers/*
echo "--- Done ---"
