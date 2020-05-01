#ifndef _SM4_ENCRYPT_H_
#define _SM4_ENCRYPT_H_

#include <iostream>
#include "sm4.h"
#include <string>
#include <vector>
#include <stdio.h>

static const char ENCRYPT_MAGIC_VALUE[6] = {'a','a','b','c','a','a'};

static uint32_t getFileSize(std::string filepath);//获取文件大小

static uint32_t uint8ToInt(const uint8_t *b, uint32_t n)
{
    return ((uint32_t)b[4 * n] << 24) |
           ((uint32_t)b[4 * n + 1] << 16) |
           ((uint32_t)b[4 * n + 2] << 8) |
           ((uint32_t)b[4 * n + 3]);
}

static void intToUint8(uint32_t v, uint8_t *b)
{
    b[0] = (uint8_t)(v >> 24);
    b[1] = (uint8_t)(v >> 16);
    b[2] = (uint8_t)(v >> 8);
    b[3] = (uint8_t)(v);
}

/**
 *   加密文件结构
 * 
 *  |magic_number|version(32)|
 *   header-length(32) | origin file name length (header-length) | origin file name|
 *   origin file size| custom field length | custom field<json> |
 *   ------------------------ file content ----------------------
 * 
 * */
class Sm4Encrypt{
public:
    static const uint8_t  BLOCK_SIZE = SM4_BLOCK_SIZE;

    static const uint32_t VERSION1 = 16;

    Sm4Encrypt(std::string _path):filePath(_path){
        version = VERSION1;
        parseFile();
    }

    void showFileInfo();
  
    int encryptFile(uint8_t *pkey , std::string encryptFilePath);//加密文件

    void readFile(std::string filename);//读取文件

    ~Sm4Encrypt(){
    }
    
private:
    std::string filePath;
    uint32_t fileSize;

    uint8_t encryptKey[BLOCK_SIZE];

    uint32_t version;//协议版本 

    void parseFile();

    void writeEncryptFileHeader(std::vector<uint8_t> *pHeadData);
    
    void writeMagicNumber(std::vector<uint8_t> *pHeaderData);

    void writeVersion(std::vector<uint8_t> *pHeaderData);
};

void static printUint8(uint8_t c){
    printf("%02x" , c);
}

void static printUint8Array(uint8_t *data , int size = 16 , bool hasSpace = true){
	for(int i=0;i < size ;i++){
        printUint8(data[i]);
        if(hasSpace){
            std::cout << " ";
        }
    }//end for i
    std::cout << std::endl;
}

#endif