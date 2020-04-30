#ifndef _SM4_ENCRYPT_H_
#define _SM4_ENCRYPT_H_

#include <iostream>
#include "sm4.h"
#include <string>
#include <vector>
#include <stdio.h>

static const char ENCRYPT_MAGIC_VALUE[6] = {'a','a','b','c','a','a'};

class Sm4Encrypt{
public:
    static const uint8_t  BLOCK_SIZE = SM4_BLOCK_SIZE;

    Sm4Encrypt(std::string _path):filePath(_path){
        parseFile();
    }

    void showFileInfo();

    static uint32_t getFileSize(std::string filepath);//获取文件大小

    int encryptFile(uint8_t *pkey , std::string encryptFilePath);//加密文件

    void readFile(std::string filename);//读取文件

    ~Sm4Encrypt(){
    }
    
private:
    std::string filePath;
    uint32_t fileSize;

    uint8_t encryptKey[BLOCK_SIZE];

    void parseFile();

    void writeEncryptFileHeader(std::vector<uint8_t> *pHeadData);
    
    void writeMagicNumber(std::vector<uint8_t> *pHeaderData);
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