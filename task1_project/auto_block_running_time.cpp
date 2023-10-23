#include "mypackages/AES_Cipher.h"
#include "mypackages/running_task.h"
#include "mypackages/convert.h"
#include "mypackages/userio.h"
#include "mypackages/running_time.h"

using namespace std;

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

void dododaiinput(){
    wcout << "Length of input:\t";
    for (int i = 1; i < 9; i++){
        string data;
        string filename;
        #ifdef __linux__
        filename = "./input/input" + to_string(i) + ".txt";
        #elif _WIN32
        filename = ".\\input\\input" + to_string(i) + ".txt";
        #endif
        data = getDataFromFile(string_to_wstring(filename));
        wcout << data.size() << " ";
    }
    wcout << '\n';
}

void dothoigian(string mode){
    AES_Cipher *aes = new AES_Cipher(mode);
    wcout << "KEY : " << string_to_wstring(hexencode(aes->key, aes->keySize)) << '\n';
    if (mode != "ECB"){
        wcout << "IV/CTR : " << string_to_wstring(hexencode(aes->iv, aes->ivSize)) << '\n';
    }
    wcout << "Waiting...\n";
    double averageTimeEncrypt[8] = {0};
    double averageTimeDecrypt[8] = {0};
    for (int i = 1; i < 9; i++){
        string plaintext;
        string filename;
        string ciphertext;
        #ifdef __linux__
        filename = "./input/input" + to_string(i) + ".txt";
        #elif _WIN32
        filename = ".\\input\\input" + to_string(i) + ".txt";
        #endif
        plaintext = getDataFromFile(string_to_wstring(filename));
        auto start = chrono::high_resolution_clock::now();
        
        for (int i = 0; i < 1000; ++i) {
            ciphertext = aes->encrypt(plaintext);
        }

        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        averageTimeEncrypt[i - 1] = static_cast<double>(duration) / 1000.0;
        start = chrono::high_resolution_clock::now();
        
        for (int i = 0; i < 1000; ++i) {
            plaintext = aes->decrypt(ciphertext);
        }

        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        averageTimeDecrypt[i - 1] = static_cast<double>(duration) / 1000.0;
    }

    wcout << "RESULT " << string_to_wstring(mode) << " MODE:\n";
    wcout << "Encrypt:\t";
    for (int i = 0; i < 8; i++){
        wcout << averageTimeEncrypt[i] << ' ';
    }
    wcout << "\nDecrypt:\t";
    for (int i = 0; i < 8; i++){
        wcout << averageTimeDecrypt[i] << ' ';
    }
    wcout << '\n';
}

int main(){
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif
    dododaiinput();
    while(true){
        string mode;
        modeSelection(mode); 
        dothoigian(mode);
        #ifdef _WIN32
        wcin.ignore();
        #endif
    }
    return 0;
}