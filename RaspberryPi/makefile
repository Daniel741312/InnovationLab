src_include=$(wildcard ./src/*.c)
obj_include=$(patsubst ./src/%.c,./obj/%.o,$(src_include))

src_main=main.c
obj_main=./obj/main.o

#自定义的一些参数
myArgs=-Wall

#头文件所在目录
include_path=./include

ALL:main
main:$(obj_include) $(obj_main)
	gcc  $^ -o $@ $(myArgs)

$(obj_include):./obj/%.o:./src/%.c							#目标和依赖都需要改变
	gcc -c $< -o $@ $(myArgs) -I $(include_path)

$(obj_main):$(src_main)
	gcc -c $< -o $@ $(myArgs) -I $(include_path)

.PHONY: ALL clean

clean:
	-rm -rf ./obj/*.o main 