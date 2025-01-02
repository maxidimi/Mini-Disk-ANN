#!/bin/bash

# Configuration file path
CONFIG_FILE="config.txt"

# Output file for results
OUTPUT_FILE="vamana_results.txt"

# List of vamana_function types
VAMANA_FUNCTIONS=("vamana")

# Initialize the output file
echo "Vamana Results" > "$OUTPUT_FILE"
echo "-----------------" >> "$OUTPUT_FILE"

# Loop over each vamana_function type
for FUNCTION in "${VAMANA_FUNCTIONS[@]}"; do
    echo "Running for vamana_function=$FUNCTION"

    # Update the vamana_function in the configuration file
    for L in {100..200}; do
        echo "Running with L=$L and vamana_function=$FUNCTION"

        # Create a temporary config file with the current L value and function
        TEMP_CONFIG_FILE="temp_config.txt"
        cp "$CONFIG_FILE" "$TEMP_CONFIG_FILE"
        sed -i "s/^vamana_function=.*$/vamana_function=$FUNCTION/" "$TEMP_CONFIG_FILE"
        sed -i "s/^L=.*$/L=$L/" "$TEMP_CONFIG_FILE"
        sed -i "s/^graph_name=.*$/graph_name=filtered_tmp/" "$TEMP_CONFIG_FILE"

        # Run the application and store the result
        echo "Results for L=$L, vamana_function=$FUNCTION:" >> "$OUTPUT_FILE"
        ./bin/vamana "$TEMP_CONFIG_FILE" >> "$OUTPUT_FILE" 2>&1

        # Delete the file filtered_tmp.bin after each iteration
        if [ -f "filtered_tmp.bin" ]; then
            rm -f "filtered_tmp.bin"
            echo "Deleted filtered_tmp.bin for L=$L, vamana_function=$FUNCTION"
        fi

        echo "-----------------" >> "$OUTPUT_FILE"
    done
done

# Clean up temporary config file
rm -f temp_config.txt

echo "All runs completed. Results saved in $OUTPUT_FILE."
