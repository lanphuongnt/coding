//C internal library 
#include <iostream>
using std::endl;
using std::cout;
using std::cin;

#include <string>
using std::string;

#include <cstdlib>
using std::exit;
#include "assert.h"

//Cryptopp Librari
#include <cryptopp/files.h>
using CryptoPP::FileSource;
using CryptoPP::FileSink;
using CryptoPP::BufferedTransformation;

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::Redirector; // string to bytes

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::byte;

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#include <windows.h>
#endif

#include <locale>
#include <string>
using std::string;
#include <cstdlib>
using std::exit;
#include "assert.h"

#include "cryptopp/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;

#include "cryptopp/osrng.h"
using CryptoPP::AutoSeededRandomPool;
using CryptoPP::byte;

using namespace std;
using namespace CryptoPP;

#include "mypackages/AES_Cipher.h"
#include "mypackages/convert.h"
#include "mypackages/userio.h"

int main(){
    #ifdef __linux__
    std::locale::global(std::locale("C.UTF-8"));
    #endif
  
    #ifdef _WIN32
    // Set console code page to UTF-8 on Windows
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    
    string mode;
    int modeSelection;
    int inputSelection;

    int keySize = AES::DEFAULT_KEYLENGTH, ivSize = AES::BLOCKSIZE;

    // Select mode
    cout << ">>> Select AES mode:" << endl
          << "1. ECB" << endl
          << "2. CBC" << endl
          << "3. OFB" << endl
          << "4. CFB" << endl
          << "5. CTR" << endl
          << "6. XTS" << endl
          << "7. CCM" << endl
          << "8. GCM" << endl
          << "Please enter your number (1/2/3/4/5/6/7/8):" << endl
          << "> ";

    cin >> modeSelection;
    cin.ignore();

    // Set mode, keySize and ivSize
    switch (modeSelection) {
        case 1: 
            mode = "ECB";
            ivSize = 0;
            break;
        case 2: 
            mode = "CBC";
            break;
        case 3: 
            mode = "OFB";
            break;
        case 4: 
            mode = "CFB";
            break;
        case 5: 
            mode = "CTR";
            break;
        case 6: 
            mode = "XTS";
            keySize = 32;
            break;
        case 7: 
            mode = "CCM";
            ivSize = 12;
            break;
        case 8: 
            mode = "GCM";
            break;
        default:
            cout << "Invalid input!" << endl;
            exit(0);
            break;
    }

    AES_Cipher aes(mode);

    // Generate key/iv randomly, from screen, from file
    CryptoPP::byte *key;
    CryptoPP::byte *iv;
    
    // Generate KEY
    cout << ">>> How would you like to generate KEY?" << endl
          << "1. Random" << endl
          << "2. Input from screen" << endl
          << "3. Input from file" << endl
          << "Please enter your number (1/2/3)?" << endl << "> ";
    cin >> inputSelection;
    cin.ignore();
    key = generateBlock(keySize, inputSelection, "KEY");
    aes.setKey(key, keySize);

    // Save KEY
    cout << "Would you like to save KEY to file?" << endl
         << "1. Yes" << endl
         << "2. No" << endl
         << "> ";
    cin >> inputSelection;
    cin.ignore();
    switch (inputSelection){
        case 1:{
            int performanceSelection;
            cout << ">>> Select encoding performance of the KEY?" << endl
                 << "1. Bytes" << endl
                 << "2. Hex" << endl
                 << "3. Base64" << endl
                 << "Please enter your number (1/2/3)?" << endl
                 << "> ";
            cin >> performanceSelection;
            cin.ignore();
            printData(encodeOutput(performanceSelection, key, keySize), 2, "KEY");
            break;
        }
        case 2:{
            printData(encodeOutput(2, key, keySize), 1, "KEY");
            break;
        }
        default:
            cout << "Invalid input!" << endl;
            break;       
    }


    if (mode != "ECB"){
        // Generate iv
        cout << ">>> How would you like to generate IV?" << endl
            << "1. Random" << endl
            << "2. Input from screen" << endl
            << "3. Input from file" << endl
            << "Please enter your number (1/2/3)?" << endl << "> ";
        cin >> inputSelection;
        cin.ignore();
        iv = generateBlock(ivSize, inputSelection, "IV");
        aes.setIV(iv, ivSize);
        // Save IV
        cout << "Would you like to save iv to file?" << endl
            << "1. Yes" << endl
            << "2. No" << endl
            << "> ";
        cin >> inputSelection;
        cin.ignore();
        switch (inputSelection){
            case 1:{
                int performanceSelection;
                cout << ">>> Select encoding performance of the IV?" << endl
                    << "1. Bytes" << endl
                    << "2. Hex" << endl
                    << "3. Base64" << endl
                    << "Please enter your number (1/2/3)?" << endl
                    << "> ";
                cin >> performanceSelection;
                cin.ignore();
                printData(encodeOutput(performanceSelection, iv, ivSize), 2, "IV");
                break;
            }
            case 2:{
                // Display hex to screen
                printData(encodeOutput(2, iv, ivSize), 1, "IV");
                break;
            }
            default:
                cout << "Invalid input!" << endl;
                break;       
        }
    }

    // Decrypt or Encrypt
    cout << ">>> The code supports:" << endl
         << "1. Encrypt" << endl
         << "2. Decrypt" << endl
         << "Would you like to Encrypt or Decrypt?" << endl << "> ";

    int aescipher;
    cin >> aescipher;
    cin.ignore();

    switch(aescipher){
        case 1:{
            cout << ">>> How would you like to get the PLAINTEXT?" << endl 
                 << "1. From screen" << endl
                 << "2. From file" << endl
                 << "Please enter your number (1/2):" << endl << "> ";
            cin >> inputSelection;
            cin.ignore();
            string plaintext, ciphertext, encoded;
            getData(plaintext, inputSelection, "PLAINTEXT");
            ciphertext = aes.encrypt(plaintext);
            int performanceSelection;
            cout << ">>> Select encoding performance of the CIPHERTEXT?" << endl
                 << "1. Bytes" << endl
                 << "2. Hex" << endl
                 << "3. Base64" << endl
                 << "Please enter your number (1/2/3)?" << endl
                 << "> ";
            cin >> performanceSelection;
            cin.ignore();

            cout << ">>> How would you like to display the CIPHERTEXT?" << endl 
                 << "1. Display in screen" << endl
                 << "2. Save to file" << endl
                 << "Please enter your number (1/2):" << endl 
                 << "> ";
            int outputSelection;
            cin >> outputSelection;
            cin.ignore();
            printData(encodeOutput(performanceSelection, ciphertext), outputSelection, "CIPHERTEXT");
            break;
        }
        case 2:{
            int performanceSelection;
            cout << ">>> Select encoding performance of the CIPHERTEXT?" << endl
                 << "1. Bytes" << endl
                 << "2. Hex" << endl
                 << "3. Base64" << endl
                 << "Please enter your number (1/2/3)?" << endl
                 << "> ";
            cin >> performanceSelection;
            cin.ignore();

            cout << ">>> How would you like to get the CIPHERTEXT?" << endl 
                 << "1. From screen" << endl
                 << "2. From file" << endl
                 << "Please enter your number (1/2):" << endl << "> ";
            
            cin >> inputSelection;
            cin.ignore();

            string ciphertext, decoded, plaintext;
            getData(ciphertext, inputSelection, "CIPHERTEXT");
            decoded = decodeInput(performanceSelection, ciphertext);
            plaintext = aes.decrypt(decoded);
            cout << ">>> How would you like to display the PLAINTEXT?" << endl 
                 << "1. Display in screen" << endl
                 << "2. Save to file" << endl
                 << "Please enter your number (1/2):" << endl 
                 << "> ";

            int outputSelection;
            cin >> outputSelection;
            cin.ignore();
            printData(plaintext, outputSelection, "PLAINTEXT");
            break;
        }
        default:{
            cout << "Invalid input" << endl;
            exit(0);
            break;
        }
    }
    return 0;
}