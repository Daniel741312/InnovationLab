#通配src目录下的“功能模块”源文件（就是网络发送函数啊，超声波测距函数啊，垃圾分类函数啊blabla）
src=$(wildcard src/*.cpp)

#将变量src中所有%.cpp替换为目标文件名%.o
obj=$(patsubst src/%.cpp,obj/%.o,$(src))

#最终的目标：当前目录下的main.cpp，其目标文件也编译到obj目录下
src_main=main.cpp
obj_main=obj/main.o

#自定义的一些参数
#Wranging all，4个动态库，1个动态注入的宏开关
myArgs=-Wall -ljsoncpp -lcurl -lcrypto -lwiringPi -DTEST

#头文件所在目录
include_path=include

#用“功能模块”的目标文件和main的目标文件链接成main的elf可执行文件(最终的目标)
ALL:main
main:$(obj) $(obj_main)
	g++  $^ -o $@ $(myArgs)

#编译“功能模块”的目标文件
$(obj):obj/%.o:src/%.cpp
	g++ -c $< -o $@ -I $(include_path) $(myArgs) 

#编译main的目标文件
$(obj_main):$(src_main)
	g++ -c $< -o $@ -I $(include_path) $(myArgs) 

.PHONY: ALL clean

clean:
	-rm -rf ./obj/*.o main 
