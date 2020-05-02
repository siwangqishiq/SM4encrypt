#include <iostream>
#include "SM4Encrypt.h"
#include <map>

int main(int argc , char *argv[]){
    Sm4Encrypt crypt("res/gakki.jpeg");
    crypt.showFileInfo();
    
    crypt.encryptFile(nullptr , "res/1.dat");
    
    crypt.readFile("res/1.dat");
    
//    std::map<std::string , std::string> customMap;
//    customMap["name"] = std::string("panyi_test");
//    customMap["name2"] = std::string("panyi_test2");
//    customMap["name3"] = std::string("panyi_test3 哈哈哈");
    
    
//    std::cout << customMap["name"] << std::endl;
    
//    std::string s = "你好 世界 哈哈😄";
//    const char * c_s = s.c_str();
//    for(int i = 0 ; i < s.size();i++){
//        std::cout << c_s[i];
//    }
//    std::cout << std::endl;
    return 0;
}