#ifndef __WASTE_SORTING_H__
#define __WASTE_SORTING_H__

#include "aip-cpp-sdk-0.8.5/image_classify.h"
#include <vector>

//提供垃圾分类查询的接口：https://www.tianapi.com/apiview/97
std::string url="http://api.tianapi.com/txapi/lajifenlei/index";

//POST请求携带的数据，key是我在平台上的密钥，wrod是查询的物体名，也就是需要拼接上的传入的参数objectNames[i]
std::string postData0="key=7db7bf1a82adb28c86e9892e813df06d&word=";

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