#!/bin/bash

echo "--- Copying 'html_pages', 'python', 'ex.conf' to cmake-build-debug/ ---"
cp -r ./html_pages ./cmake-build-debug/
cp -r ./cgi_scripts ./cmake-build-debug/
cp -r ./ex.conf ./cmake-build-debug/
echo "--- Done ---"
echo
