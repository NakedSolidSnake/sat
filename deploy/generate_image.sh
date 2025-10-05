#!/bin/bash

source ./config.sh

# Build development image
check_command "docker build -t tools-development -f development/Dockerfile ." "build sat-build image"

# Build runtime image
check_command "docker build -t tools-runtime -f runtime/Dockerfile ." "build sat-runtime image"

# Login to Docker Hub
# check_command "docker login -u solidcris" "login to Docker Hub"

# Tag the development image with version
check_command "docker tag tools-development solidcris/development:$VERSION" "tag development image with version $VERSION"
check_command "docker tag tools-development solidcris/development:latest" "tag development image as latest"

# Tag the runtime image with version
check_command "docker tag tools-runtime solidcris/production:$VERSION" "tag production image with version $VERSION"
check_command "docker tag tools-runtime solidcris/production:latest" "tag production image as latest"

echo "🎉 All operations completed successfully!"