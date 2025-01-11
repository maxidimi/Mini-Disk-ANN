#!/bin/bash

# Configuration file path
CONFIG_FILE="config.txt"

# Output file for results
OUTPUT_FILE="rL_100_200_1thread.txt"

# List of vamana_function types
VAMANA_FUNCTIONS=("stitched")

# Loop over each vamana_function type
for FUNCTION in "${VAMANA_FUNCTIONS[@]}"; do
    echo "Running for vamana_function=$FUNCTION"

    # Update the vamana_function in the configuration file
    for L in {119..200}; do
        echo "Running with L=$L and vamana_function=$FUNCTION"

        # Create a temporary config file with the current L value and function
        TEMP_CONFIG_FILE="ltemp_config.txt"
        cp "$CONFIG_FILE" "$TEMP_CONFIG_FILE"

        # Update config values
        sed -i '' "s/^vamana_function=.*$/vamana_function=$FUNCTION/" "$TEMP_CONFIG_FILE"
        sed -i '' "s/^L=.*$/L=$L/" "$TEMP_CONFIG_FILE"
        sed -i '' "s/^graph_name=.*$/graph_name=ltmp/" "$TEMP_CONFIG_FILE"

        if [ "$FUNCTION" = "vamana" ]; then 
            sed -i '' "s|^dataset=.*$|dataset=data/ANN_SIFT10K/siftsmall_base.fvecs|" "$TEMP_CONFIG_FILE"
            sed -i '' "s|^query=.*$|query=data/ANN_SIFT10K/siftsmall_query.fvecs|" "$TEMP_CONFIG_FILE"
            sed -i '' "s|^groundtruth=.*$|groundtruth=data/ANN_SIFT10K/siftsmall_groundtruth.ivecs|" "$TEMP_CONFIG_FILE"
        else
            sed -i '' "s|^dataset=.*$|dataset=data/SIGMOD/DUMMY/dummy-data.bin|" "$TEMP_CONFIG_FILE"
            sed -i '' "s|^query=.*$|query=data/SIGMOD/1M_DS/contest-queries-release-1m.bin|" "$TEMP_CONFIG_FILE"
            sed -i '' "s|^groundtruth=.*$|groundtruth=data/SIGMOD/DUMMY/dummy_data_1M_queries.bin|" "$TEMP_CONFIG_FILE"
        fi

        # Run the app and store the result
        echo -n "L=$L vamana_function=$FUNCTION:" >> "$OUTPUT_FILE"
        export OMP_NUM_THREADS=1
        ./bin/vamana "$TEMP_CONFIG_FILE" >> "$OUTPUT_FILE" 2>&1

        # Delete the file ltmp.bin after each iteration
        if [ -f "ltmp.bin" ]; then
            rm -f "ltmp.bin"
        fi
    done
done

# Clean up temporary config file
rm -f ltemp_config.txt

echo "All runs completed. Results saved in $OUTPUT_FILE." in this 