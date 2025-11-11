#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

VERSION=1.0.5

# Function to check command execution and log results
check_command() {
    local command="$1"
    local description="$2"
    
    echo "Executing: $description"
    if $command; then
        echo "✅ Success: $description"
    else
        echo "❌ Error: Failed to $description"
        exit 1
    fi
}
