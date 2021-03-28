#通配src目录下的功能模块源文件
src=$(wildcard ./src/*.cpp)

#将变量src中所有%.cpp替换为%.o
obj=$(patsubst ./src/%.cpp,./obj/%.o,$(src))

#当前目录下的main.cpp，其目标文件编译到obj目录下
src_main=main.cpp
obj_main=./obj/main.o

#自定义的一些参数
myArgs=-Wall -ljsoncpp -lcurl -lcrypto -lwiringPi -DTEST

#头文件所在目录
include_path=./include

#用功能模块的目标文件和main的目标文件编译成main的elf文件
ALL:main
main:$(obj) $(obj_main)
	g++  $^ -o $@ $(myArgs)

#编译功能模块的目标文件
$(obj):./obj/%.o:./src/%.cpp
	g++ -c $< -o $@ $(myArgs) -I $(include_path)

#编译main的目标文件
$(obj_main):$(src_main)
	g++ -c $< -o $@ $(myArgs) -I $(include_path)

.PHONY: ALL clean

clean:
	-rm -rf ./obj/*.o main 
