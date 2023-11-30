#!/bin/bash

# FILEPATH: /dssg/home/acct-hpc/asc/YTchen/repos/cs149-asst1/prog1_mandelbrot_threads/run.sh

# Create or clear the result file
> result.txt

# Loop through thread counts from 1 to 32, stepping 2
for ((threads=2; threads<=32; threads+=2)); do
    # Run mandelbrot with the specified thread count and record the time
    output=$(./mandelbrot -t $threads -v 1)
    # Extract the time from the output using grep
    time=$(echo "$output" | grep -oE '\[[0-9]+\.[0-9]+\] ms' | sed -n 2p | tr -d '[] ms')
    echo "mandelbrot thread $threads: $time"
    # Append the result to the result file
    echo -e "$threads\t$time" >> result.txt
done
