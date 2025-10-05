#!/bin/bash

source ./config.sh

# Push all tags to Docker Hub
# check_command "docker push -a solidcris/development" "push all development tags to Docker Hub"
check_command "docker push -a solidcris/production" "push all production tags to Docker Hub"