#!/bin/bash 

docker build --no-cache -t solidcris/development .
 

docker tag solidcris/development solidcris/development:1.0.0

docker login -u solidcris  

docker push -a solidcris/development
