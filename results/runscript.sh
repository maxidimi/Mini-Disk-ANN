#!/bin/bash

# Initialize an empty array to store the results
declare -a results
iterations=10

# Run the app 10 times
for ((i=1; i<=iterations; i++)); do
    # Capture the output of the app
    output=$(./bin/vamana config.txt)
    
    # Print the output to the terminal
    echo "Output $i: $output"

    # Split the output into an array of values (comma-separated)
    IFS=',' read -r -a values <<< "$output"
    
    # Add values to the results array (summing by column)
    for j in "${!values[@]}"; do
        results[j]=$(echo "${results[j]:-0} + ${values[j]}" | bc)
    done

    # Remove the graph
    rm -f tmp.bin
done

# Compute the averages
for j in "${!results[@]}"; do
    avg=$(echo "${results[j]} / $iterations" | bc -l)

    # Format the average to up to 4 decimal places without trailing zeros
    formatted_avg=$(printf "%.4g" "$avg")
    averages+=("$formatted_avg")
done

# Join averages into a comma-separated string and write to the file
IFS=','; echo "${averages[*]}" 
