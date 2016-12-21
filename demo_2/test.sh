#!/bin/bash
echo "----------------------------------------------"
g++ server.cpp -o server.out -std=c++11 -pthread
g++ client.cpp -o client.out -std=c++11 -pthread
echo "----------------------------------------------"
