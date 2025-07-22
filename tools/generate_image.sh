#!/bin/bash 

docker build -t solidcris/development:1.0.1 .

docker tag solidcris/development solidcris/development:1.0.1

docker login -u solidcris  

docker push -a solidcris/development
