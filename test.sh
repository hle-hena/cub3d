#!/bin/bash

# Check if directory was given
if [ -z "$1" ]; then
  echo "Usage: $0 <directory>"
  exit 1
fi

# Iterate over all files in the given directory
for file in "$1"/*; do
  if [ -f "$file" ]; then
    echo "Running: ./cub3d \"$file\""
    ./cub3d "$file"
    echo -n "Continue to next file? (y to continue): "
    read -r answer
    if [ "$answer" != "y" ]; then
      echo "Stopped by user."
      exit 0
    fi
  fi
done
