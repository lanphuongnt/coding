//Cout the block code running time
#include <iostream>
#include <chrono>
#include <string>

using namespace std;
//Before starting record the start time
//auto start = std::chrono::high_resolution_clock::now();
//Run the encryption/decryption function 1000 times in a loop
//After the loop, record the end time
//auto end = std::chrono::high_resolution_clock::now();
//auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//double averageTime = static_cast<double>(duration) / 1000.0; // average time per round in milliseconds
//Print the results std::cout << "Average time for encryption/decryption over 1000 rounds: " << averageTime << " ms" << std::endl;

#include "convert.h"
#include "AES_Cipher.h"
#include "userio.h"
#include "running_time.h"

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

void block_running_time(){
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif

    wstring filename;
    string mode, userSelection, plaintext, ciphertext;
    AES_Cipher *aes = NULL;
    int aescipher;
    modeSelection(mode);         

    while (true){
        wcout << "What would you like to do:\n"
              << "1. Key/IV generation\n"
              << "2. Encryption\n"
              << "3. Decryption\n"
              << "4. Exit\n"
              << "Please enter your number (1/2/3/4)?\n> ";
        #ifdef _WIN32
        wcin.ignore();
        #endif
        aescipher = getInt();
        switch (aescipher) {
            case 1: {
                aes = new AES_Cipher(mode);
                wcout << "KEY : " << string_to_wstring(hexencode(aes->key, aes->keySize)) << '\n';
                if (mode != "ECB"){
                    wcout << "IV/CTR : " << string_to_wstring(hexencode(aes->iv, aes->ivSize)) << '\n';
                }
                break;
            }
            case 2: {
                if (aes == NULL){
                    goto WARNING1;
                }
                wcout << "Enter the file name which contains plaintext:\n> ";
                #ifdef _WIN32
                wcin.ignore();
                #endif
                getline(wcin, filename);
                plaintext = getDataFromFile(filename);
                auto start = chrono::high_resolution_clock::now();
                
                for (int i = 0; i < 1000; ++i) {
                    ciphertext = aes->encrypt(plaintext);
                }

                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
                double averageTime = static_cast<double>(duration) / 1000.0;
                // wcout << "Ciphertext :\n" << string_to_wstring(base64encode(ciphertext)) << '\n';
                wcout << "Length of plaintext : " << plaintext.size() << '\n';
                wcout << "Length of ciphertext : " << ciphertext.size() << '\n';
                wcout << "Average time for encryption over 1000 rounds of plaintext in file " << filename << " : " << averageTime << " ms" << endl;
                // outputSelection(ciphertext, false, true);
                break;
            }
            case 3: {
                if (aes == NULL){
                    goto WARNING1;
                }
                if (ciphertext.size() == 0){
                    goto WARNING2;
                }
                auto start = chrono::high_resolution_clock::now();
                
                for (int i = 0; i < 1000; ++i) {
                    plaintext = aes->decrypt(ciphertext);
                }

                auto end = chrono::high_resolution_clock::now();
                auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
                double averageTime = static_cast<double>(duration) / 1000.0;
                // wcout << "Recovered plaintext :\n" << string_to_wstring(plaintext) << '\n';
                wcout << "Length of plaintext : " << plaintext.size() << '\n';
                wcout << "Length of ciphertext : " << ciphertext.size() << '\n';
                wcout << "Average time for decryption over 1000 rounds of plaintext in file " << filename << " : " << averageTime << " ms" << endl;
                outputSelection(plaintext, true, false);
                break;
            }
            case 4: {
                wcout << "Good bye ^_^ !\n";
                return;
            }
            default:
                cout << "Invalid input\n";
                break;
        }
        continue;
        WARNING1:
            wcout << "Please generate key/iv first!!!\n";
            continue;
        WARNING2:
            wcout << "Please encrypt before!!!\n";
            continue;
    }
}