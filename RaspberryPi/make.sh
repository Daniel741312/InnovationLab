echo "编译中"

#gcc main.c src/tcp_send.c -o main -Iinclude
g++ waste_sorting_main.cpp ./src/waste_sorting.cpp -o waste_sorting_main -ljsoncpp -lcurl -lcrypto -std=c++11

echo "编译完成"
