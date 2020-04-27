#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include "sm4.h"

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
        //printf("%02x ", output[i]);
        printUint8(data[i]);
        if(hasSpace){
            std::cout << " ";
        }
    }//end for i
    std::cout << std::endl;
}

void encryptData(const uint8_t *pKey , uint8_t *in,long size, uint8_t *out){
    long startPos = 0;
    int offset = SM4_CUBE_LEN;

    SM4_KEY sm_key;
    
    setKey(pKey , &sm_key);
    
    uint8_t inputBuf[SM4_CUBE_LEN];
    uint8_t outBuf[SM4_CUBE_LEN];

    while(startPos < size){
       
        int lastBytes = size - startPos;
        offset = lastBytes >= SM4_BLOCK_SIZE ? SM4_BLOCK_SIZE:lastBytes;
        std::cout << "size = " << size << std::endl;
        std::cout << "startPos = " << startPos << std::endl;
        std::cout << "offset = " << offset << std::endl;

        for(int i = 0 ;i < offset;i++){
            inputBuf[i] = in[startPos + i];
            outBuf[i] = 0;
        }//end for i

        if(offset < SM4_BLOCK_SIZE){
            for(int i = offset ; i< SM4_BLOCK_SIZE ; i++){
                inputBuf[i] = 0;
                outBuf[i] = 0;
            }//end for i
        }
        
        std::cout << "input buffer" << std::endl;
        printUint8Array(inputBuf);

        SM4_encrypt(inputBuf, outBuf, &sm_key);

        // std::cout << "output buffer" << std::endl;
        // printUint8Array(outBuf);

        for(int i = 0 ;i < offset;i++){
            out[startPos + i] = outBuf[i];
        }//end for i

        startPos += offset;
    }//end while
    
}

// void decryptData(char *pKey , uint8_t *in,long size, uint8_t *out, const SM4_KEY *ks){
//     long index = 0;

//     SM4_KEY sm_key;
//     setKey(pKey , &sm_key);
    
//     uint8_t inputBuf[SM4_CUBE_LEN];
//     uint8_t outBuf[SM4_CUBE_LEN];
    
//     SM4_decrypt(inputBuf, outBuf, &sm_key);
// }


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
    const auto size = str1.size();
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
	uint8_t input[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	uint8_t output[16];

    
    //01 23 45 67 89 ab cd ef fe dc ba 98 76 54 32 10
    encryptData(key , input , 16 , output);
	printUint8Array(output);
}

//https://aks.jd.com/tools/sec/
int main(int argc , char *argv[]){
    test1();
    //test2();
    //test3();
    return 0;
}

