#include "waste_sorting.hpp"

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