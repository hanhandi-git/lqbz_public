#!/bin/bash

# 检查是否存在名为go_learn的容器
if [ $(docker ps -a -q -f name=go_learn) ]; then
    echo "Container go_learn exists, removing it..."
    docker rm -f go_learn
fi

# 启动新的容器
echo "Starting new go_learn container..."
docker run -d --name go_learn -v $(pwd):/app go-dev:latest