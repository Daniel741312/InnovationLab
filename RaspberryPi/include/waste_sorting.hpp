#ifndef __WASTE_SORTING_HPP__
#define __WASTE_SORTING_HPP__

#include <vector>
#include <string>
#include <iostream>

int getGarbageNamesByImage(const char* fileName,std::vector<std::string>* objectNames);

/*
   获取垃圾类型的函数：传入识别出来的物体种类（包含所有可能结果的string数组，返回其中最有可能所属的垃圾类型，约定垃圾类型如下：

   0-可回收
   1-有害
   2-厨余(湿)
   3-其他(干)

   -1表示出错，并打印到stderr中
*/
int getGarbageCategoryByNames(std::vector<std::string>* objectNames);

/*
   在“curl_easy_setopt(curl1,CURLOPT_WRITEFUNCTION,writer)”中注册的回调函数，其格式为：
   size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
   会将服务器返回的json字符串ptr写到泛型指针userdata所指向的缓冲区中去（这里用字符串）
*/
static int writer(char* data, size_t size, size_t nmemb, std::string* writer_data);

#endif