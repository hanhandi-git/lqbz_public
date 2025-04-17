#!/bin/bash

# 编译程序
g++ -std=c++11 -g topk_test.cpp -o topk_test
g++ -std=c++11 -g topk_extension.cpp -o topk_extension
g++ -std=c++11 -g nth_test.cpp -o nth_test