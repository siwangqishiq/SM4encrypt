#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <fstream>
#include "sm4.h"

long get_file_size(std::string filepath);

const int SM4_CUBE_LEN = SM4_BLOCK_SIZE;

void setKey(const uint8_t *pKey , SM4_KEY *pSmKey) {
    // for(int i = 0 ; i< SM4_CUBE_LEN ; i++){
    //     pSmKey->rk[i] = pKey[i];
    // }//end for i
    SM4_set_key(pKey,pSmKey);
}

void printUint8(uint8_t c){
    printf("%02x" , c);
}

void printUint8Array(uint8_t *data , int size = 16 , bool hasSpace = true){
	for(int i=0;i < size ;i++){
        printUint8(data[i]);
        if(hasSpace){
            std::cout << " ";
        }
    }//end for i
    std::cout << std::endl;
}

void switchByteData(const uint8_t *pKey , uint8_t *in, long size ,uint8_t *out , bool encrypt){
    long startPos = 0;
    int offset = SM4_CUBE_LEN;

    SM4_KEY sm_key;
    
    setKey(pKey , &sm_key);
    
    uint8_t input_buf[SM4_CUBE_LEN];
    uint8_t out_buf[SM4_CUBE_LEN];

    while(startPos < size){
        int lastBytes = size - startPos;
        offset = lastBytes >= SM4_BLOCK_SIZE ? SM4_BLOCK_SIZE:lastBytes;

        for(int i = 0 ;i < offset;i++){
            input_buf[i] = in[startPos + i];
            out_buf[i] = 0;
        }//end for i

        if(offset < SM4_BLOCK_SIZE){
            for(int i = offset ; i< SM4_BLOCK_SIZE ; i++){
                input_buf[i] = 0;
                out_buf[i] = 0;
            }//end for i
        }

        if(encrypt){//
            SM4_encrypt(input_buf,  out_buf,  &sm_key);
        }else{
            SM4_decrypt(input_buf , out_buf , &sm_key);
        }

        for(int i = 0 ;i < offset;i++){
            out[startPos + i] = out_buf[i];
        }//end for i

        startPos += offset;
    }//end while
}

void encryptData(const uint8_t *pKey , uint8_t *in,long size, uint8_t *out){
    switchByteData(pKey , in , size , out ,true);
}

void decryptData(const uint8_t *pKey , uint8_t *in,long size, uint8_t *out){
    switchByteData(pKey , in , size , out ,false);
}


void test1() {
    uint8_t key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	uint8_t input[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	uint8_t output[16];


    char str[] = "Hello World!";

    SM4_KEY sm_key;

    //加密
	SM4_set_key(key,&sm_key);
	SM4_encrypt(input, output, &sm_key);

	printUint8Array(output);

    std::cout << "xxxxxxxxxxx" << std::endl;
    
	//解密
	SM4_decrypt(output, output, &sm_key);
	printUint8Array(output);
}


void test2(){
    std::string key= "1111111111111111";
    printUint8Array((uint8_t *)key.c_str() , key.length() , false);

    std::string str1 = "HelloHelloHelloH";
    std::cout << "origin str : " << std::endl;
    printUint8Array((uint8_t *)str1.c_str() , str1.length() , false);
    const int size = str1.size();
    std::cout << "str len = " << size <<std::endl;

    uint8_t out[size];
    encryptData((uint8_t *)key.c_str() , (uint8_t *)str1.c_str() , size , out);
    std::cout << "encrypt str : " << std::endl;
    printUint8Array(out , size , false);

    uint8_t origin[size];
    encryptData((uint8_t *)key.c_str() , out , size , origin);
    printUint8Array(origin , size , false);
}

void test3() {
    uint8_t key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	uint8_t input[16] = {0x23,0x23,0x23,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};  //01234567 89ab cdef fedc ba98 7654 3210
	uint8_t output[16];
    //uint8_t key2[16] = {0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30};
    
    //01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
    printUint8Array(input , 16 , false);

    // encrypt : 68 1e df 34 d2 06 96 5e 86 b3 e9 4f 53 6e 42 46
    //encryptData(key , input , 16 , output);
    encryptData(key , input , 16 , output);
	printUint8Array(output, 16, false);

    decryptData(key , output , 16 , input);
    printUint8Array(input, 16, false);
}

void test4(){
    unsigned char key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	unsigned char input[16] = {0x23,0x23,0x23,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	unsigned char output[16];
	
    SM4_KEY sm_key;

    int i;
    for(i=0;i<16;i++)
		printf("%02x ", input[i]);
	printf("\n");

	//加密
	SM4_set_key(key,&sm_key);
	SM4_encrypt(input, output, &sm_key);

	
	for(i=0;i<16;i++)
		printf("%02x ", output[i]);
	printf("\n");

	//解密
	SM4_decrypt(output, output, &sm_key);
	for(i=0;i<16;i++)
		printf("%02x ", output[i]);
	printf("\n");
}

void test5(){
    std::string input_str = "HelloHelloHelloHello";
    const long size = input_str.length();
    std::string key = "1234567890123456";
    
    uint8_t encrypt_buf[size];

    encryptData((uint8_t *)key.c_str() , (uint8_t *)input_str.c_str(),size , encrypt_buf);
    printUint8Array(encrypt_buf , size , false);

    uint8_t decrypt_buf[size];
    decryptData((uint8_t *)key.c_str() , encrypt_buf , size , decrypt_buf);

    std::string decrypt_str = std::string((const char *)decrypt_buf);
    std::cout << decrypt_str << std::endl;
}

const uint8_t key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};

long get_file_size(std::string filepath){
    std::ifstream file(filepath.c_str());
    file.seekg(0,std::ios::end);
    std::streampos ps = file.tellg();
    file.close();
    return ps;
}

void encrypt_file(std::string filepath ,const uint8_t *key) {
    //std::fstream file(filepath.c_str(),std::ios::in | std::ios::binary);
    std::string encrypt_file_name = filepath + "_encrypt";
    std::cout << encrypt_file_name << std::endl;

    const long file_size = get_file_size(filepath);
    std::cout << "file size = " << file_size << std::endl;
    
    char data[file_size];
    memset(data,0,file_size);

    std::ifstream file(filepath.c_str(),std::ios::in | std::ios::binary);
    //file >> data;
    file.read(data , file_size);
    file.close();

    std::cout << data << std::endl;
    //printUint8Array(data , file_size);
}

void decrypt_file(std::string filepath ,const uint8_t *key){

}

void test6() {
    uint8_t key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
    const long size = 16;
	uint8_t input[size] = {0x01,0x23};
	uint8_t output[size];

    encryptData(key , input , size , output);
    printUint8Array(output , size , true);

    uint8_t output2[size];
    decryptData(key , output , size , output2);
    printUint8Array(output2 , size , true);
}

//https://aks.jd.com/tools/sec/
int main(int argc , char *argv[]){
    //test1();
    //test2();
    //test3();
    // test4();
    // test5();

    //std::string s = "Hello World!";
    //const char *s_str = s.c_str();

    // printf("%s \n", s_str);

    //encrypt_file("gakki.jpeg", key);
    test6();
    return 0;
}

