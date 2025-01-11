#!/bin/bash

# Configuration file path
CONFIG_FILE="config.txt"

# Output file for results
OUTPUT_FILE="R_15_100_1thread.txt"

# List of vamana_function types
VAMANA_FUNCTIONS=("vamana" "filtered" "stitched")

# Loop over each vamana_function type
for FUNCTION in "${VAMANA_FUNCTIONS[@]}"; do
    echo "Running for vamana_function=$FUNCTION"

    # Loop over each value of R
    for R in {15..100}; do
        echo "Running with R=$R and vamana_function=$FUNCTION"

        # Create a temporary config file with the current R value and function
        TEMP_CONFIG_FILE="temp_config.txt"
        cp "$CONFIG_FILE" "$TEMP_CONFIG_FILE"

        # Update config values
        sed -i '' "s/^vamana_function=.*$/vamana_function=$FUNCTION/" "$TEMP_CONFIG_FILE"
        sed -i '' "s/^R=.*$/R=$R/" "$TEMP_CONFIG_FILE"
        sed -i '' "s/^graph_name=.*$/graph_name=tmp/" "$TEMP_CONFIG_FILE"

        if [ "$FUNCTION" = "vamana" ]; then 
            sed -i '' "s|^dataset=.*$|dataset=data/ANN_SIFT10K/siftsmall_base.fvecs|" "$TEMP_CONFIG_FILE"
            sed -i '' "s|^query=.*$|query=data/ANN_SIFT10K/siftsmall_query.fvecs|" "$TEMP_CONFIG_FILE"
            sed -i '' "s|^groundtruth=.*$|groundtruth=data/ANN_SIFT10K/siftsmall_groundtruth.ivecs|" "$TEMP_CONFIG_FILE"
        else
            sed -i '' "s|^dataset=.*$|dataset=data/SIGMOD/DUMMY/dummy-data.bin|" "$TEMP_CONFIG_FILE"
            sed -i '' "s|^query=.*$|query=data/SIGMOD/1M_DS/contest-queries-release-1m.bin|" "$TEMP_CONFIG_FILE"
            sed -i '' "s|^groundtruth=.*$|groundtruth=data/SIGMOD/DUMMY/dummy_data_1M_queries.bin|" "$TEMP_CONFIG_FILE"
        fi

        # Run the app and append the result to the output file
        echo -n "R=$R vamana_function=$FUNCTION: " >> "$OUTPUT_FILE"
        export OMP_NUM_THREADS=1
        ./bin/vamana "$TEMP_CONFIG_FILE" >> "$OUTPUT_FILE" 2>&1

        # Delete the file tmp.bin after each iteration
        if [ -f "tmp.bin" ]; then
            rm -f "tmp.bin"
        fi
    done
done

# Clean up temporary config file
rm -f temp_config.txt

echo "All runs completed. Results saved in $OUTPUT_FILE."
