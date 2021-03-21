#include "inc/aip-cpp-sdk-0.8.5/image_classify.h"
#include <vector>

//提供垃圾分类查询的接口：https://www.tianapi.com/apiview/97
std::string url="http://api.tianapi.com/txapi/lajifenlei/index";

//POST请求携带的数据，key是我在平台上的密钥，wrod是查询的物体名，也就是需要拼接上的传入的参数objectNames[i]
std::string postData0="key=7db7bf1a82adb28c86e9892e813df06d&word=";

int getGarbageNamesByImage(const char* fileName,std::vector<std::string>* objectNames);
int getGarbageCategoryByNames(std::vector<std::string>* objectNames);
static int writer(char* data, size_t size, size_t nmemb, std::string* writer_data);

int main(int argc,char* argv[]){

	std::vector<std::string>* objectNames=new std::vector<std::string>();
	int type=0;
	int ret=0;

	for(int i=1;i<argc;++i){
		/*从main函数参数中获取图片文件路径*/
		ret=getGarbageNamesByImage(argv[i],objectNames);
		if(ret!=0){
			std::cerr<<"BaiduAI reports an error\n"<<std::endl;
			exit(1);
		}

		type=getGarbageCategoryByNames(objectNames);
		std::cout<<"----In the main:type="<<type<<"----\n\n"<<std::endl;

		(*objectNames).erase((*objectNames).begin(),(*objectNames).end());
	}

	delete(objectNames);
	return 0;
}

int getGarbageNamesByImage(const char* fileName,std::vector<std::string>* objectNames){

	/*设置APPID和密钥*/
	std::string app_id = "23750677";
	std::string api_key = "SheH7Uw5i3utHLiWhW9iFtgr";
	std::string secret_key = "T1RkeHrk1rLTPe0IDeieXjslRTMcCfXr";

	/*得到一个图像分类器（客户端）*/
	aip::Imageclassify* client=new aip::Imageclassify(app_id, api_key, secret_key);

	/*把图像编码到base64字符串中去*/
	std::string* imageInBase64=new std::string();
	aip::get_file_content(fileName, imageInBase64);

	/*调用通用物体识别，他的返回值是一个json对象*/
	Json::Value classifyResult;
	classifyResult = (*client).advanced_general(*imageInBase64, aip::null);

	std::cout<<fileName<<":\n"<<"In getGarbageNameByImage,Json returnd from BaiduAI:\n"<<classifyResult<<std::endl;

	/*检查是否出错，如果error_code字段为null，则出错*/
	if(classifyResult["error_code"].asInt()!=0){
		return -1;
	}

	/*遍历所有的可能结果（可能性依次递减），保存到传入传出参数std::vector<std::string>* objectNames指向的字符串数组*/
	int result_num=classifyResult["result_num"].asInt();
	for(int i=0;i<result_num;++i){
		(*objectNames).push_back(classifyResult["result"][i]["keyword"].asString());
	}

	delete(client);
	delete(imageInBase64);
	return 0;
}

/*
   获取垃圾类型的函数：传入识别出来的物体种类（包含所有可能结果的string数组，返回其中最有可能所属的垃圾类型，约定垃圾类型如下：

   0-可回收
   1-有害
   2-厨余(湿)
   3-其他(干)

   -1表示出错，并打印到stderr中
   */
int getGarbageCategoryByNames(std::vector<std::string>* objectNames){
	/*创建一个curl句柄*/
	CURL* curl=NULL;
	CURLcode curlRet;

	/*接收从服务器返回的json字符串*/
	std::string JsonStringRet;
	int garbageCategogry=-1;
	/*Json解析器和根结点*/
	Json::Reader reader;
	Json::Value root;

	/*初始化这个curl句柄*/
	curl=curl_easy_init();
	if(curl==NULL){
		std::cerr<<"curl_easy_init error,returns NULL"<<std::endl;
		return -1;
	}

	/*
		给该句柄设置参数：
			请求方式为POST
			设置url地址：一定要转化为C风格的字符串哦
			设置回调函数，接收服务器返回的数据
	*/
	curl_easy_setopt(curl,CURLOPT_POST,1);
	curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
	curl_easy_setopt(curl,CURLOPT_WRITEFUNCTION,writer);
	curl_easy_setopt(curl,CURLOPT_WRITEDATA,&JsonStringRet);

	for(std::vector<std::string>::iterator it=(*objectNames).begin();it<(*objectNames).end();++it){
		/*得到了完整的POST请求数据*/
		std::string postData=postData0+(*it);
		std::cout<<postData<<std::endl;

		JsonStringRet="";

		/*向远端服务器提交请求*/
		curl_easy_setopt(curl,CURLOPT_POSTFIELDS,postData.c_str());
		curlRet=curl_easy_perform(curl);
		if(curlRet!=CURLE_OK){
			std::cerr<<"curl_easy_perform error,ret="<<curlRet<<std::endl;
			return -1;
		}

		reader.parse(JsonStringRet, root);

		/*错误状态码*/
		int retCode=root["code"].asInt();

		if(retCode==250){
			/*如果code==250，说明没有找到该关键字对应的垃圾类别，继续处理下一个*/
			continue;
		}else if(retCode==200){
			/*
			   如果code==200，说明能够找到该关键字对应的垃圾类别，但是不保证一定是对的，BaiduAI也可能出错（很大概率）
			   那么就处理服务器的返回结果，解析返回的json字符串，拿到newslist数组的第0个成员的type属性，就是垃圾类型
			*/
			garbageCategogry=root["newslist"][0]["type"].asInt();
			std::cout<<"In getGarbageCategoryByNames,Json returnd from Tianapi:\n"<<JsonStringRet<<std::endl;
			break;
		}else{
			/*其他错误*/
			std::cerr<<"Tianapi reports an error,code="<<retCode<<"\n"<<std::endl;
			return -1;
		}
	}

	/*释放句柄*/
	curl_easy_cleanup(curl);
	return garbageCategogry;
}

/*
   在“curl_easy_setopt(curl1,CURLOPT_WRITEFUNCTION,writer)”中注册的回调函数，其格式为：
   size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
   会将服务器返回的json字符串ptr写到泛型指针userdata所指向的缓冲区中去（这里用字符串）
   */
static int writer(char* data, size_t size, size_t nmemb, std::string* writer_data){ 
	/*写入多少字节*/
	unsigned long sizes = size * nmemb;
	if (NULL == writer_data){  
		return 0;
	}  
	writer_data->append(data, sizes);
	return sizes;
}

