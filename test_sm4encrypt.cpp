#include <iostream>
#include "Sm4Encrypt.h"

int main(int argc , char *argv[]){
    // Sm4Encrypt encrypt("gakki.jpeg");
    // encrypt.showFileInfo();
    // std::string s = std::string(ENCRYPT_MAGIC_VALUE);
    // std::cout << s << std::endl;

    Sm4Encrypt encrypt("gakki.jpeg");
    encrypt.encryptFile(nullptr , "");
    encrypt.readFile("1.dat");
    
    return 0;
}