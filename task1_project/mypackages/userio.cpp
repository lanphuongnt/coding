//C internal library 
#include <iostream>
using std::wcin;
using std::wcout;
using std::wcerr;
using std::endl;
using std::cout;
using std::cin;

#include <string>
using std::string;
using std::wstring;
#include <cstdlib>
using std::exit;
#include "assert.h"
#include "fstream"


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

#include "cryptopp/cryptlib.h"
using CryptoPP::Exception;

// convert string
// Hex <---> Binary
#include "cryptopp/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;

// Base64 <---> Binary
#include "cryptopp/base64.h"
using CryptoPP::Base64Encoder;
using CryptoPP::Base64Decoder;

/* Set utf8 support for windows*/ 
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#endif
/* Convert string <--> utf8*/ 
#include <locale>
using std::wstring_convert;
#include <codecvt>
using  std::codecvt_utf8;

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

using namespace std;
using namespace CryptoPP;

#include "userio.h"
#include "convert.h"

string getDataFromFile(wstring filename){
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif
    ifstream file;
    string data;
    try {
        file.open(wstring_to_string(filename));
    }
    catch(const CryptoPP::Exception& e){
        wcerr << e.what() << endl;
        exit(1);
    }
    data.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return data;
}

CryptoPP::byte *generateBlock(unsigned int blockSize, int genSelection, string nameOfBlock){
    CryptoPP::byte *block = new CryptoPP::byte[blockSize];
    string str, decoded;
    switch (genSelection) {
        case 1: {
            AutoSeededRandomPool prng;
            prng.GenerateBlock(block, blockSize);
            return block;
        }
        case 2: {
            wstring wstr;
            wcout << "Enter your " << string_to_wstring(nameOfBlock) << " (hex - " << blockSize << " bytes):\n> ";
            #ifdef _WIN32
            wcin.ignore();
            #endif
            getline(wcin, wstr);
            str = wstring_to_string(wstr);
            
            break;
        }
        case 3: {
            wstring filename;
            string fn;    
            ifstream file;

            wcout << ">>> Enter your filename which contains " << string_to_wstring(nameOfBlock) << " (hex - " << blockSize << " bytes):\n> ";
            #ifdef _WIN32
            wcin.ignore();
            #endif
            getline(wcin, filename);
            fn = wstring_to_string(filename);
            try {
                file.open(fn);
            }
            catch(const CryptoPP::Exception& e){
                wcerr << e.what() << endl;
                exit(1);
            }
            // getline cua file
            getline(file, str);
            file.close();
            break;
        }
        default:
            wcout << "Invalid input!\n";
            exit(0);
            break;
    }

    decoded = hexdecode(str);
    if (decoded.size() != blockSize){
        wcout << "Invalid " << string_to_wstring(nameOfBlock) << " length!\n";
        exit(1);
    }
    try{
        StringSource(decoded, true, new ArraySink(block, blockSize));
    }
    catch(const CryptoPP::Exception& e){
        wcout << "Invalid input!\n";
        wcerr << e.what() << '\n';
        exit(1);
    }
    // wcout << string_to_wstring(hexencode(block, 16));
    return block;
}

void inputPlaintextSelection(string &plaintext){
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif
    int userSelection;
    wstring wtext, input;

    wcout << ">>> Where would you like to get your plaintext from?\n"
          << "1. From screen\n"
          << "2. From file\n"
          << "Please enter your number (1/2)?\n> ";

    #ifdef _WIN32
    wcin.ignore();
    #endif
    userSelection = getInt();

    switch (userSelection) {
        case 1: {
            wcout << ">>> Enter your plaintext (characters):\n> ";
            #ifdef _WIN32
            wcin.ignore();
            #endif
            getline(wcin, wtext);
            plaintext = wstring_to_string(wtext);
            break;
        }
        case 2: {
            wstring filename;
            wcout << ">>> Enter your filename:\n> ";
            #ifdef _WIN32
            wcin.ignore();
            #endif
            getline(wcin, filename);
            plaintext = getDataFromFile(filename);
            break;
        }
        default:
            wcout << "Invalid input!\n";
            exit(0);
            break;
    }
}

string inputCiphertextSelection(){
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif
    string cipher;
    int userSelection, performSelection;
    wstring wtext;
    string encoded;

    wcout << ">>> Select encoding performance of your ciphertext:\n"
          << "1. Unicode\n"
          << "2. Hex\n"
          << "3. Base64\n"
          << "Please enter your number (1/2/3)?\n> ";
    #ifdef _WIN32
    wcin.ignore();
    #endif
    performSelection = getInt();


    wcout << ">>> Where would you like to get your ciphertext from?\n"
          << "1. From screen\n"
          << "2. From file\n"
          << "Please enter your number (1/2)?\n> ";

    #ifdef _WIN32
    wcin.ignore();
    #endif
    userSelection = getInt();

    switch (userSelection) {
        case 1: {
            wcout << ">>> Enter your ciphertext:\n> ";
            #ifdef _WIN32
            wcin.ignore();
            #endif
            getline(wcin, wtext);
            encoded = wstring_to_string(wtext);
            break;
        }
        case 2: {
            wstring filename;
            wcout << ">>> Enter your filename:\n> ";
            #ifdef _WIN32
            wcin.ignore();
            #endif
            getline(wcin, filename);
            encoded = getDataFromFile(filename);
            break;
        }
        default:
            wcout << "Invalid input!\n";
            exit(0);
            break;
    }

    cipher.clear();
    switch (performSelection)
    {
    case 1:
        cipher = encoded;
        break;
    case 2:
        cipher = hexdecode(encoded);
        break;
    case 3:
        cipher = base64decode(encoded);
        break;
    default:
        wcout << "Invalid input!\n";
        exit(0);
        break;
    }
    return cipher;
}

void outputSelection(string text, bool plaintext, bool ciphertext){
    #ifdef __linux__
    setlocale(LC_ALL, "");
    #elif _WIN32
    _setmode(_fileno(stdin), _O_U16TEXT);
    _setmode(_fileno(stdout), _O_U16TEXT);
    #endif
    int userSelection;
    string encoded;
    wcout << ">>> Select encoding performance of the " << (plaintext ? "plaintext" : "ciphertext") << " :\n"
          << "1. Unicode\n"
          << "2. Hex\n"
          << "3. Base64\n"
          << "Please enter your number (1/2/3)?\n> ";
    
    #ifdef _WIN32
    wcin.ignore();
    #endif
    userSelection = getInt();

    
    encoded.clear();
    switch (userSelection)
    {
    case 1:
        encoded = text;
        break;
    case 2:
        encoded = hexencode(text);
        break;
    case 3:
        encoded = base64encode(text);
        break;
    default:
        wcout << "Invalid input!\n";
        exit(0);
        break;
    }

    wcout << ">>> How would you like to output the result?\n"
          << "1. Display in screen\n"
          << "2. Write to file\n"
          << "Please enter your number (1/2)?\n> ";
    #ifdef _WIN32
    wcin.ignore();
    #endif
    userSelection = getInt();


    switch (userSelection) {
        case 1: {
            if (plaintext == true)
                wcout << "Plaintext: ";
            if (ciphertext == true)
                wcout << "Ciphertext: ";
            wcout << string_to_wstring(encoded) << '\n';
            break;
        }
        case 2: {
            wstring filename;
            string fn;    
            ofstream file;

            wcout << ">>> Enter your filename:\n> ";
            #ifdef _WIN32
            wcin.ignore();
            #endif
            getline(wcin, filename);
            fn = wstring_to_string(filename);
            try {
                file.open(fn);
            }
            catch(const CryptoPP::Exception& e){
                wcerr << e.what() << endl;
                exit(1);
            }
            file << encoded << '\n';
            file.close();
            break;
        }
        default:
            wcout << "Invalid input!\n";
            exit(0);
            break;
    }
}


string selectEncryptOrDecrypt(){
    int userSelection;
    wcout << ">>> Would you like to encryption or decryption message?\n"
          << "1. Encrypt\n"
          << "2. Decrypt\n"
          << "Please enter your number (1/2):\n> ";
    #ifdef _WIN32
    wcin.ignore();
    #endif
    userSelection = getInt();

    switch (userSelection)
    {
    case 1:
        return "encrypt";
        break;
    case 2:
        return "decrypt";
    default:
        wcout << "Invalid input!\n";
        exit(0);
        break;
    }
}

void modeSelection(string &mode){ 
    int userSelection;
    wcout << "Select AES mode:\n"
          << "1. ECB\n"
          << "2. CBC\n"
          << "3. OFB\n"
          << "4. CFB\n"
          << "5. CTR\n"
          << "6. XTS\n"
          << "7. CCM\n"
          << "8. GCM\n"
          << "Please enter your number (1/2/3/4/5/6/7/8):\n> ";
    // Goi dau tien nen khong can ignore
    userSelection = getInt();
    switch (userSelection) {
        case 1: 
            mode = "ECB";
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
            break;
        case 7: 
            mode = "CCM";
            break;
        case 8: 
            mode = "GCM";
            break;
        default:
            wcout << "Invalid input!\n";
            exit(0);
            break;
    }
}

int inputWaySelection(){
    int userSelection;
    wcout << ">>> How would you like to generate key and iv/ctr?\n"
          << "1. Random\n"
          << "2. Input from screen\n"
          << "3. Input from file\n"
          << "Please enter your number (1/2/3)?\n> ";
    #ifdef _WIN32
    wcin.ignore();
    #endif
    userSelection = getInt();
    return userSelection;
}

int getInt(){
    wstring inp;
    int res = -1;
    // Da ignore truoc khi goi ham
    getline(wcin, inp); 
    try {
        res = stoi(inp);
    }
    catch (const invalid_argument& e) {
        goto RETURN;
    }
    RETURN:
    return res;
}