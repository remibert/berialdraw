#!/bin/bash
OUTPUT_FOLDER="$ICLOUD/GitHub/berialdraw/berialdraw"
export OUTPUT_FOLDER
conan profile detect
conan install . --build=missing -of "$OUTPUT_FOLDER"
conan build   .                 -of "$OUTPUT_FOLDER"
