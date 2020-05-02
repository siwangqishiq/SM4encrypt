#ifndef _SM4_ENCRYPT_H_
#define _SM4_ENCRYPT_H_

#include <iostream>
#include "sm4.h"
#include <string>
#include <vector>
#include <stdio.h>

static const char ENCRYPT_MAGIC_VALUE[6] = {'x','i','n','l','a','n'};

static uint32_t getFileSize(std::string filepath);//获取文件大小

static void writeIntToByteVector(std::vector<uint8_t> *buf,uint32_t value);//

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

static void stringReplace(std::string &strBig, const std::string &strsrc, 
        const std::string &strdst) {
    std::string::size_type pos = 0;
    std::string::size_type srclen = strsrc.size();
    std::string::size_type dstlen = strdst.size();

    while( (pos=strBig.find(strsrc, pos)) != std::string::npos) {
        strBig.replace( pos, srclen, strdst );
        pos += dstlen;
    }
}

static std::string getPathOrURLShortName(std::string strFullName){
    if (strFullName.empty()){
        return "";
    }
    stringReplace(strFullName, "/", "\\");
    std::string::size_type iPos = strFullName.find_last_of('\\') + 1;
    return strFullName.substr(iPos, strFullName.length() - iPos);
}

/**
 *   加密文件结构
 * 
 *  |magic_number|version(32)|
 *   header-length(32) | 
 *   origin file name length (32) |
 *   origin file name (origin file name length)|
 *   origin file size (32)| 
 *   custom field length | custom field<json > |
 *   ------------------------ file content ----------------------
 * 
 * */
class Sm4Encrypt{
public:
    static const uint8_t  BLOCK_SIZE = SM4_BLOCK_SIZE;

    static const uint32_t VERSION1 = 1;

    Sm4Encrypt(std::string _path):filePath(_path){
        version = VERSION1;
        filename = getPathOrURLShortName(filePath);
        parseFile();
    }

    void showFileInfo();
  
    int encryptFile(uint8_t *pkey , std::string encryptFilePath);//加密文件
    
    void readFile(std::string filename);//读取文件

    ~Sm4Encrypt(){
    }

    // 用户自定义数据 json格式返回
    virtual std::string genCustomJsonData();

    virtual void handleCusonJsonData(std::string &jsonStr);
private:
    std::string filename;
    std::string filePath;
    uint32_t fileSize;

    uint8_t encryptKey[BLOCK_SIZE];

    uint32_t version;//协议版本 

    uint32_t originFileSize;

    void parseFile();

    void writeEncryptFileHeader(std::vector<uint8_t> *pHeadData);
    
    void writeMagicNumber(std::vector<uint8_t> *pHeaderData);

    void writeVersion(std::vector<uint8_t> *pHeaderData);

    void writeFileName(std::vector<uint8_t> *pHeaderData);

    void parseFileHeader(std::string filename);

    uint32_t readUint32FromFile(std::ifstream &file); //read a int from file stream
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