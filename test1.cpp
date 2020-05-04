#include <iostream>
#include "SM4Encrypt.h"
#include <map>

int main(int argc , char *argv[]){
    uint8_t key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
//    Sm4Encrypt crypt("res/gakki.jpeg");
//    crypt.showFileInfo();
//    
//    crypt.encryptFile(nullptr , "res/1.dat");
//    
//    crypt.readFile("res/1.dat");
    
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
    
    Sm4Encrypt instance;
    instance.encryptFile(key,"res/test.txt","res/en_test");
    instance.readEncryptFile("res/en_test");

    bool encrypt = instance.checkFileIsEncrypted("res/en_test");
    std::cout << "是否加密 : " << encrypt << std::endl;

    EncryptFileHeadInfo info;
    instance.parseFileHeader("res/en_test" , info);
    // std::cout << "head info :" << std::endl;
    // std::cout << "magic number :" << info.magicNumber << std::endl;
    // std::cout << "headLength :" << info.headLength << std::endl;
    // std::cout << "originFileName :" << info.originFileName << std::endl;
    // std::cout << "originFileSize :" << info.originFileSize << std::endl;
    // std::cout << "customJsonString :" << info.customJsonString << std::endl;

    instance.decryptFile(key , "res/en_test");

    // bool isEncrypt = encrypt.checkFileIsEncrypted("res/gakki.jpeg");
    // std::cout << isEncrypt << std::endl;
    
    // isEncrypt = encrypt.checkFileIsEncrypted("res/1.dat");
    // std::cout << isEncrypt << std::endl;
    
    // isEncrypt = encrypt.checkFileIsEncrypted("res/2.dat");
    // std::cout << isEncrypt << std::endl;
    
    return 0;
}