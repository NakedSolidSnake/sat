#!/bin/bash 

docker build -t codenotalk/development .

docker tag codenotalk/development codenotalk/development:1.0.0

docker login -u codenotalk  

docker push -a codenotalk/development
