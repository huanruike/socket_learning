#!/bin/bash
echo "==================================================================\n"
g++ server.cpp ServerThread.cpp UserData.cpp -o server.out -std=c++11 -pthread
echo "==================================================================\n"
