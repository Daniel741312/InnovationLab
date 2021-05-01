#include "waste_sorting.h"
#include <jsoncpp/json/json.h>
#include <unistd.h>

#include "../aip-cpp-sdk-0.8.5/image_classify.h"

static int writer(char* data, size_t size, size_t nmemb, std::string* writer_data) {
    /*写入多少字节*/
    unsigned long sizes = size * nmemb;
    if (writer_data==nullptr) {
        return 0;
    }
    writer_data->append(data, sizes);
    return sizes;
}

namespace waste_sorting{

WasteSorting::WasteSorting(){

}

WasteSorting::~WasteSorting(){

}

int WasteSorting::getGarbageNamesByImage(const char* fileName,std::vector<std::string>* objectNames) {

    /*得到一个图像分类器（客户端）*/
    aip::Imageclassify* client = new aip::Imageclassify(app_id_, api_key_, secret_key_);

    /*把图像编码到base64字符串中去*/
    std::string* imageInBase64 = new std::string();
    aip::get_file_content(fileName, imageInBase64);

    /*调用通用物体识别，他的返回值是一个json对象*/
    Json::Value classifyResult;
    classifyResult = (*client).advanced_general(*imageInBase64, aip::null);

    /*gcc动态的向源码中注入宏：gcc -DMACRONAME=MACRODEF*/
#ifdef TEST
    std::cout << fileName << ":\n"
              << "In getGarbageNameByImage,Json returnd from BaiduAI:\n"
              << classifyResult << "\n"
              << std::endl;
#endif
    /*检查是否出错，如果error_code字段为null，则出错*/
    if (classifyResult["error_code"].asInt() != 0) {
        write(STDERR_FILENO,"BaiduAI reports an error\n",26);
        return -1;
    }

    /*遍历所有的可能结果（可能性依次递减），保存到传出参数std::vector<std::string>* objectNames指向的字符串数组*/
    int result_num = classifyResult["result_num"].asInt();
    for (int i = 0; i < result_num; ++i) {
        (*objectNames).push_back(classifyResult["result"][i]["keyword"].asString());
    }

    delete (client);
    delete (imageInBase64);
    return 0;
}

int WasteSorting::getGarbageCategoryByNames(std::vector<std::string>* objectNames) {
    /*创建一个curl句柄*/
    CURL* curl = nullptr;

    /*接收从服务器返回的json字符串*/
    std::string JsonStringRet;
    int garbageCategogry = -1;
    /*Json解析器和根结点*/
    Json::Reader reader;
    Json::Value root;

    /*初始化这个curl句柄*/
    curl = curl_easy_init();
    if (curl == nullptr) {
        write(STDERR_FILENO,"curl_easy_init error,returns nullptr\n",38);
        return -1;
    }

    /*
        给该句柄设置参数：
        请求方式为POST
        设置url地址：一定要转化为C风格的字符串哦
        设置回调函数，接收服务器返回的数据
    */
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &JsonStringRet);

    for (std::vector<std::string>::iterator it = (*objectNames).begin();it < (*objectNames).end(); ++it) {
        /*得到了完整的POST请求数据*/
        std::string postData = postData0_ + (*it);
#ifdef TEST
        std::cout << postData << std::endl;
#endif
        JsonStringRet = "";

        /*向远端服务器提交请求*/
        CURLcode curlRet;
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());
        curlRet = curl_easy_perform(curl);
        if (curlRet != CURLE_OK) {
            std::cerr << "curl_easy_perform error,ret=" << curlRet << std::endl;
            return -1;
        }

        reader.parse(JsonStringRet, root);

        /*错误状态码*/
        int retCode = root["code"].asInt();

        if (retCode == 250) {
            /*如果code==250，说明没有找到该关键字对应的垃圾类别，继续处理下一个*/
            continue;
        } else if (retCode == 200) {
            /*
               如果code==200，说明能够找到该关键字对应的垃圾类别，但是不保证一定是对的，BaiduAI也可能出错（很大概率）
               那么就处理服务器的返回结果，解析返回的json字符串，拿到newslist数组的第0个成员的type属性，就是垃圾类型
            */
            garbageCategogry = root["newslist"][0]["type"].asInt();
#ifdef TEST
            std::cout
                << "In getGarbageCategoryByNames,Json returnd from Tianapi:\n"
                << JsonStringRet << std::endl;
#endif
            break;
        } else {
            /*其他错误*/
            std::cerr << "Tianapi reports an error,code=" << retCode << "\n"
                      << std::endl;
            return -1;
        }
    }

    /*释放句柄*/
    curl_easy_cleanup(curl);
    return garbageCategogry;
}



}
