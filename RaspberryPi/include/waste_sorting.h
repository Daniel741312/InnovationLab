#ifndef __WASTE_SORTING_H__
#define __WASTE_SORTING_H__

#include <vector>
#include <string>
#include <iostream>
#include <jsoncpp/json/json.h>

/*
   获取垃圾类型的函数：传入识别出来的物体种类（包含所有可能结果的string数组），返回其中最有可能所属的垃圾类型，约定垃圾类型如下：

   0-可回收
   1-有害
   2-厨余(湿)
   3-其他(干)

   -1表示出错，并打印到stderr中
*/
int getGarbageCategoryByNames(std::vector<std::string> *objectNames);
int getGarbageNamesByImage(const char* fileName, std::vector<std::string> *objectNames);

#endif
