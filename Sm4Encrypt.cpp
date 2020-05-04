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

int Sm4Encrypt::encryptFile(uint8_t *pkey ,std::string path, std::string encryptFilePath){
    if(pkey == nullptr){
        pkey = encryptKey;
    }
    originFileSize = getFileSize(path);

    std::cout << "gen header info" << std::endl;
    //生成加密文件头
    std::vector<uint8_t> fileHeadData;
    std::string originFileName = getPathOrURLShortName(path);
    writeEncryptFileHeader(&fileHeadData , originFileName);
    
    //加密文件头写入文件
    std::ofstream outFile;
    outFile.open(encryptFilePath.c_str() , std::ios::binary);
    outFile.write((char *)fileHeadData.data() , fileHeadData.size());

    //写入加密后的文件内容
    std::ifstream inputFile(path.c_str(),std::ios::binary);
    uint32_t handleCount = writeEncryptFileContent(pkey ,inputFile, outFile , originFileSize);
    
    inputFile.close();
    outFile.close();

    std::cout << "encrypt file handle cout : " << handleCount << std::endl;

    return ENCRYPT_CODE_OK;
}

static void writeIntToByteVector(std::vector<uint8_t> *buf,uint32_t value){
    if(buf == nullptr)
        return;

    const long size = 4;
    uint8_t byteBuf[size];
    intToUint8(value , byteBuf);
    for(int i= 0 ;i < size;i++){
        buf->push_back(byteBuf[i]);
    }//end for i
}

//写入加密文件的文件头
void Sm4Encrypt::writeEncryptFileHeader(std::vector<uint8_t> *pHeaderData, 
            std::string &originFileName){
    //1.写入magic number
    writeMagicNumber(pHeaderData);

    //写入版本号
    writeVersion(pHeaderData);

    //写入head长度 先占位 后面修改
    int zoneHeadLengthPos = pHeaderData->size();
    writeIntToByteVector(pHeaderData , 0);

    //写入文件名信息到头中  文件原始名长度 ＋ 原始名
    writeFileName(pHeaderData , originFileName);

    //写入原始文件长度
    writeIntToByteVector(pHeaderData , this->originFileSize);

    //写入用户自定义json数据
    std::string jsonStr = genCustomJsonData();
    std::cout << "custom json : " << jsonStr << std::endl;
    uint32_t jsonStrLength = jsonStr.size();
    writeIntToByteVector(pHeaderData , jsonStrLength);// 写入长度
    //写入内容
    const char *cJsonStr = jsonStr.c_str();
    for(int i= 0 ; i < jsonStrLength;i++){
        pHeaderData->push_back((uint8_t)cJsonStr[i]);
    }//end for i

    //重新写入正确的head长度
    int headRealSize = pHeaderData->size();
    //std::cout << "zoneHeadLengthPos : " << zoneHeadLengthPos << std::endl;
    uint8_t headLenArray[4];
    intToUint8((uint32_t)headRealSize , headLenArray);
    (*pHeaderData)[zoneHeadLengthPos] = headLenArray[0];
    (*pHeaderData)[zoneHeadLengthPos + 1] = headLenArray[1];
    (*pHeaderData)[zoneHeadLengthPos + 2] = headLenArray[2];
    (*pHeaderData)[zoneHeadLengthPos + 3] = headLenArray[3];
}

void Sm4Encrypt::writeFileName(std::vector<uint8_t> *pHeaderData , std::string &originFileName){
    uint32_t len = originFileName.size();
     //写入原始文件名长度
    writeIntToByteVector(pHeaderData , len);
    //写入文件名称
    const char *pFileName = originFileName.c_str();

    for(int i = 0 ; i < len ;i++){
        pHeaderData->push_back((uint8_t)pFileName[i]);
        //std::cout << pFileName[i];
    }//end for i
    // std::cout << std::endl;
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
    
    writeIntToByteVector(header , this->version);
}

/**
* 
**/
void Sm4Encrypt::readEncryptFile(std::string filename){
    const long size = getFileSize(filename);
    std::cout << "read file size = " << size << std::endl;
    char buf[size];
    std::ifstream file(filename.c_str() , std::ios::binary);
    file.read((char *)buf , size);

    file.close();

    printUint8Array((uint8_t *)buf ,size , true);

    EncryptFileHeadInfo headInfo;
    parseFileHeader(filename ,headInfo);
}

//从文件流中读取出一个32位整型数据
uint32_t Sm4Encrypt::readUint32FromFile(std::ifstream &file){
    const int byteCount = 4;
    uint8_t buf[byteCount];

    file.read((char *)buf , 4);
    uint32_t result = uint8ToInt(buf , 0);

    return result;
}

void Sm4Encrypt::parseFileHeader(std::string filename , EncryptFileHeadInfo &header){
    std::cout << "======== " << filename << " info ===========" << std::endl;
    
    std::ifstream file;
    file.open(filename.c_str(), std::ios::binary);

    //read magic number 
    const long magicNumberSize = sizeof(ENCRYPT_MAGIC_VALUE) / sizeof(ENCRYPT_MAGIC_VALUE[0]);
    uint8_t magicNumberBuf[magicNumberSize];
    file.read((char *)magicNumberBuf , magicNumberSize);
    std::string magicNumber = std::string((char *)magicNumberBuf);
    header.magicNumber = magicNumber;
    std::cout << "magic number : " << magicNumber << std::endl;

    //read version
    uint32_t version = readUint32FromFile(file);
    header.version = version;
    std::cout << "version :" << version << std::endl;

    //read head length
    uint32_t headLen = readUint32FromFile(file);
    header.headLength = headLen;
    std::cout << "headLen :" << headLen << std::endl;

    //read origin file name length
    const uint32_t originFileNameLen = readUint32FromFile(file);
    std::cout << "origin file name length :" << originFileNameLen << std::endl;

    // read origin file name content
    uint8_t originFileNameBuf[originFileNameLen + 1];
    originFileNameBuf[originFileNameLen]='\0';
    file.read((char *)originFileNameBuf , originFileNameLen);
    std::string originFileName = std::string((const char *)originFileNameBuf);
    header.originFileName = originFileName;
    std::cout << "origin file name :" << originFileName << std::endl;

    //read origin file length
    uint32_t originFileLength = readUint32FromFile(file);
    header.originFileSize = originFileLength;
    std::cout << "origin file length :" << originFileLength << std::endl;

    //read custom json string
    uint32_t customJsonLength = readUint32FromFile(file);
    std::cout << "customJsonLenght : " << customJsonLength << std::endl;
    if(customJsonLength > 0){
        uint8_t jsonStr[customJsonLength + 1];
        jsonStr[customJsonLength] = '\0';
        file.read((char *)jsonStr , customJsonLength);
        std::string jsonString = std::string((const char *)jsonStr);
        header.customJsonString = jsonString;

        //call back for users
        handleCustomJsonData(jsonString);
        //std::cout << jsonString << std::endl;
    }

    file.close();
}

// 用户自定义数据 json格式返回
std::string Sm4Encrypt::genCustomJsonData(){
    return "{}";
}

//读取加密文件中 用户自定义的json字符串 完成相应业务逻辑
void Sm4Encrypt::handleCustomJsonData(std::string jsonStr){
    std::cout << "custom json : " << jsonStr << std::endl;
}

static uint32_t getFileSize(std::string filepath){
    std::ifstream file(filepath.c_str());
    file.seekg(0,std::ios::end);
    std::streampos ps = file.tellg();
    file.close();
    return ps;
}

/**
*检测文件是否已经被加密
**/
bool Sm4Encrypt::checkFileIsEncrypted(std::string checkFilePath){
    long size = getFileSize(checkFilePath);

    std::ifstream checkFile(checkFilePath.c_str());
    if(!checkFile.good()){
        checkFile.close();
        return false;
    }
    
    bool isEncrypt = false;
    const int magicNumberSize = sizeof(ENCRYPT_MAGIC_VALUE) / sizeof(ENCRYPT_MAGIC_VALUE[0]);
    if(size < magicNumberSize){
        checkFile.close();
        return false;
    }
    uint8_t buf[magicNumberSize];
    checkFile.read((char *)buf , magicNumberSize);

    isEncrypt = true;
    for(int i = 0 ;i < magicNumberSize;i++){
        if(ENCRYPT_MAGIC_VALUE[i] != buf[i]){
            isEncrypt = false;
            break;
        }
    }//end for i
    checkFile.close();

    return isEncrypt;
}

uint32_t Sm4Encrypt::writeEncryptFileContent(uint8_t *key , std::ifstream &inputStream,
    std::ofstream &outputStream , uint32_t size){
    uint32_t handleBytes = 0;

    SM4_KEY sm_key;
    SM4_set_key(key,&sm_key);

    uint8_t input[BLOCK_SIZE];
    uint8_t output[BLOCK_SIZE];

    while(handleBytes <= size){
        uint32_t lastSize = size - handleBytes;//剩余字节数
        uint32_t readCount = lastSize > BLOCK_SIZE ? BLOCK_SIZE:BLOCK_SIZE - lastSize;
        inputStream.read((char *)input , readCount);
        if(readCount < BLOCK_SIZE){
            for(int i = BLOCK_SIZE - readCount;i < BLOCK_SIZE;i++){ //不足字节补0
                input[i] = 0;
            }//end for i
        }
        SM4_encrypt(input, output, &sm_key);
        outputStream.write((char *)output , BLOCK_SIZE);
        handleBytes+=BLOCK_SIZE;
    }//end while

    return handleBytes;
}

