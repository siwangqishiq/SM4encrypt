#include "Sm4Encrypt.h"
#include <fstream>
#include <vector>


void Sm4Encrypt::showFileInfo() {
    std::cout << "======= file info ======" << std::endl;
    std::cout << "file path : " << filePath << std::endl; 
    std::cout << "file size : " << fileSize << std::endl; 
    std::cout << "========== END =========" << std::endl;
}

void Sm4Encrypt::parseFile(){
    this->fileSize = getFileSize(this->filePath);
}

int Sm4Encrypt::encryptFile(uint8_t *pkey , std::string encryptFilePath){
    if(pkey == nullptr){
        pkey = encryptKey;
    }

    std::ifstream inputFile(filePath.c_str(),std::ios::binary);
    inputFile.close();

    std::cout << "gen header info" << std::endl;
    //生成加密文件头
    std::vector<uint8_t> fileHeadData;
    writeEncryptFileHeader(&fileHeadData);
    
    //加密文件头写入文件
    std::ofstream outFile;
    outFile.open(encryptFilePath.c_str() , std::ios::binary);
    outFile.write((char *)fileHeadData.data() , fileHeadData.size());
    outFile.close();
    return 1;
}

//写入加密文件的文件头
void Sm4Encrypt::writeEncryptFileHeader(std::vector<uint8_t> *pHeadData){
    //1.写入magic number
    writeMagicNumber(pHeadData);
    //写入版本号
    std::cout << "write header code" << std::endl;
    writeVersion(pHeadData);
}

void Sm4Encrypt::writeMagicNumber(std::vector<uint8_t> *header){
    if(header == nullptr)
        return;
    int len = sizeof(ENCRYPT_MAGIC_VALUE) / sizeof(ENCRYPT_MAGIC_VALUE[0]);
    
    for(int i = 0 ; i< len ;i++){
        header->push_back(ENCRYPT_MAGIC_VALUE[i]);
    }//end for i
}

void Sm4Encrypt::writeVersion(std::vector<uint8_t> *header){
    if(header == nullptr)
        return;
    
    const int perBytes = 4;
    uint8_t versionBuf[perBytes];
    intToUint8(this->version , versionBuf);
    for(int i= 0 ;i < perBytes;i++){
        header->push_back(versionBuf[i]);
    }//end for i
}

void Sm4Encrypt::readFile(std::string filename){
    const long size = getFileSize(filename);
    std::cout << "read file size = " << size << std::endl;
    char buf[size];
    std::ifstream file(filename.c_str() , std::ios::binary);
    file.read((char *)buf , size);

    file.close();

    printUint8Array((uint8_t *)buf ,size , true);
}

static uint32_t getFileSize(std::string filepath){
    std::ifstream file(filepath.c_str());
    file.seekg(0,std::ios::end);
    std::streampos ps = file.tellg();
    file.close();
    return ps;
}
