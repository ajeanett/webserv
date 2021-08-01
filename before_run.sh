#!/bin/bash

echo "--- Copying 'html_pages', 'python', 'ex.conf' to cmake-build-debug/ ---"
cp -r ./html_pages ./cmake-build-debug/
cp -r ./cgi_scripts ./cmake-build-debug/
cp -r ./ex.conf ./cmake-build-debug/
cp -r ./testers ./cmake-build-debug/
cp -r ./YoupiBanane ./cmake-build-debug/
chmod 755 ./cmake-build-debug/cgi_scripts/*
chmod 755 ./cmake-build-debug/testers/*
echo "--- Done ---"
echo
