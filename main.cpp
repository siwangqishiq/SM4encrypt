#include <iostream>
#include <iomanip>
#include "sm4.h"

void printUint8(uint8_t c){
    std::cout<< std::hex<< std::setfill('0')<< std::setw(2)<<
    std::uppercase<< (static_cast<int>(c)& 0xFF);
}

void pintUint8Array(uint8_t *data , int size = 16){
	for(int i=0;i < size ;i++){
        //printf("%02x ", output[i]);
        printUint8(data[i]);
        std::cout << " ";
    }//end for i
    std::cout << std::endl;
}

int main(int argc , char *argv[]){
    uint8_t key[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	uint8_t input[16] = {0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
	uint8_t output[16];

    char str[] = "Hello World!";

    SM4_KEY sm_key;

    //加密
	SM4_set_key(key,&sm_key);
	SM4_encrypt(input, output, &sm_key);

	pintUint8Array(output);

    std::cout << "xxxxxxxxxxx" << std::endl;
    
	//解密
	SM4_decrypt(output, output, &sm_key);
	pintUint8Array(output);

    return 0;
}

