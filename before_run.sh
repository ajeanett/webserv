#!/bin/bash

echo "--- Copying files to cmake-build-debug/ ---"
cp -r ./www ./cmake-build-debug/
cp -r ./cgi_scripts ./cmake-build-debug/
cp -r ./ex.conf ./cmake-build-debug/
cp -r ./testers ./cmake-build-debug/
cp -r ./YoupiBanane ./cmake-build-debug/
echo "--- Done ---"

echo

echo "--- chmod-ing scripts ---"
chmod +x ./cmake-build-debug/cgi_scripts/*
chmod +x ./cmake-build-debug/testers/*
echo "--- Done ---"
