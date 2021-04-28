#ifndef __WASTE_SORTING_H__
#define __WASTE_SORTING_H__
/*
   0-可回收
   1-有害
   2-厨余(湿)
   3-其他(干)
*/

namespace waste_sorting{

   class WasteSorting{
      public:
         WasteSorting();
         ~WasteSorting();
         int getGarbageCategoryByNames(std::vector<std::string> *objectNames);
         int getGarbageNamesByImage(const char *fileName, std::vector<std::string> *objectNames);
         std::string app_id_ = "23750677";
         std::string api_key_ = "SheH7Uw5i3utHLiWhW9iFtgr";
         std::string secret_key_ = "T1RkeHrk1rLTPe0IDeieXjslRTMcCfXr";
         std::string postData0_ = "key=7db7bf1a82adb28c86e9892e813df06d&word=";
      private:
         int writer(char* data, size_t size, size_t nmemb,std::string* writer_data);
         //提供垃圾分类查询的接口：https://www.tianapi.com/apiview/97
         std::string url_ = "http://api.tianapi.com/txapi/lajifenlei/index";
         // POST请求携带的数据，key是我在平台上的密钥，wrod是查询的物体名，也就是需要拼接上的传入的参数objectNames[i]
   };
   
}

#endif
