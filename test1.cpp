#include <iostream>
#include "SM4Encrypt.h"

int main(int argc , char *argv[]){
    Sm4Encrypt crypt("res/gakki.jpeg");
    crypt.showFileInfo();
    
    crypt.encryptFile(nullptr , "res/1.dat");
    
    crypt.readFile("res/1.dat");
    
    return 0;
}