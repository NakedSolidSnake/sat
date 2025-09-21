#!/bin/bash

# Exit immediately if a command exits with a non-zero status
set -e

VERSION=1.0.4

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

check_command "docker build -t tools-development -f Dockerfile ." "build sat-build image"

# Login to Docker Hub
check_command "docker login -u solidcris" "login to Docker Hub"

# Tag the development image with version
check_command "docker tag tools-development solidcris/development:$VERSION" "tag image with version $VERSION"

# Tag the development image as latest
check_command "docker tag solidcris/development:$VERSION solidcris/development:latest" "tag image as latest"

# Push all tags to Docker Hub
check_command "docker push -a solidcris/development" "push all tags to Docker Hub"

echo "🎉 All operations completed successfully!"