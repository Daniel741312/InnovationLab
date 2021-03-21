#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <jsoncpp/json/json.h>
using namespace std;


int writeIntoFile(const char* item){

    std::ifstream trashInfoR("./allTrashesInfo.json",ios::binary);
    if (!trashInfoR.is_open()){
        std::cerr<< "ifstream file error" << std::endl;
        return -1;
    }

    Json::Reader* reader=new Json::Reader;

    Json::Value* rootOfFile=new Json::Value;
    reader->parse(trashInfoR,*rootOfFile);
    trashInfoR.close();

    Json::Value* rootOfItem=new Json::Value;
    reader->parse(item,*rootOfItem);

#ifdef TEST
    cout<<"rootOfFile:"<<endl;
    cout<<(*rootOfFile)<<endl;

    cout<<"item:"<<endl;
    cout<<(*rootOfItem)<<endl;
#endif

    int itemId=(*rootOfItem)["id"].asInt();
    (*rootOfFile)[itemId]=(*rootOfItem);
    
#ifdef TEST
    cout<<"after change,rootOfFile:"<<endl;
    cout<<(*rootOfFile)<<endl;
#endif

    ofstream trashInfoW("./allTrashesInfo.json",ios::out);
    if(!trashInfoW.is_open()){
        std::cerr<<"oftream file error"<<std::endl;
        return -1;
    }

    Json::StyledWriter* jsw=new Json::StyledWriter;
    trashInfoW<<(*jsw).write(*rootOfFile);
    trashInfoW.close();

    delete(jsw);
    delete(rootOfItem);
    delete(rootOfFile);
    delete(reader);

    return 0;
}


int main(int argc, char *argv[]){

    const char* str = "{\"id\":2,\"recyleBitMap\":0,\"location\":[120.3502, 30.3202],\"usage\":[1, 1, 1, 0.25]}";
    std::cout<<writeIntoFile(str)<<std::endl;

    return 0;
}