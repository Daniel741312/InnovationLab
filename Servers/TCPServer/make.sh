#!/bin/bash

echo "正在编译TCPserver.cpp"

g++ TCPserver.cpp wrap.c -o TCPserver -lpthread -ljsoncpp -I./ -Wall -DTEST

echo "编译结束"