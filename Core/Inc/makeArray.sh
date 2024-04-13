#!/bin/bash

# File to write to
header_file="songs.h"

# Specify the directory containing the .wav files
directory_path="D:/"

if ! [ -d "$directory_path" ]; then
	echo "No USB found"
	bash
fi

# Check if the directory exists
if [ -d "$directory_path" ]; then
    # List all .wav files in the directory and store them in an array
    wav_files=("$directory_path"/*.wav)
    
    # Format the list of .wav files as an array
    wav_formatted_array="static char* staticSongArr[NUM_OF_SONGS] = {"
    for file in "${wav_files[@]}"; do
        wav_formatted_array+="\"$(basename "$file")\", "
    done
    wav_formatted_array="${wav_formatted_array%, }"  # Remove the trailing comma and space
    wav_formatted_array+="};"
    
    # Output the formatted array
    echo "$wav_formatted_array"
else
	echo "Directory not found: $directory_path"
fi

echo "+----------------------+"

if [ -d "$directory_path" ]; then
    # List all .wav files in the directory and store them in an array
    txt_files=("$directory_path"/*.txt)
    
    # Format the list of .wav files as an array
    txt_formatted_array="char* staticTextArr[NUM_OF_SONGS] = {"
    for file in "${txt_files[@]}"; do
        txt_formatted_array+="\"$(basename "$file")\", "
    done
    txt_formatted_array="${txt_formatted_array%, }"  # Remove the trailing comma and space
    txt_formatted_array+="};"
    
    # Output the formatted array
    echo "$txt_formatted_array"
	echo "+----------------------+"
	array_length=${#wav_files[@]}
    echo "Array Length: $array_length"
else
	echo "Directory not found: $directory_path"
fi

rm $header_file

printf "#define NUM_OF_SONGS $array_length\n\n$wav_formatted_array\n\n$txt_formatted_array" >> $header_file

echo "Written to file $header_file"

sleep 5
exit(0)