#pragma once

#ifndef IO_USER_SELECTION_H
#define IO_USER_SELECTION_H
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

#include "convert.h"

void getData(string &data, int userSelection, string dataName = "data"){
    switch(userSelection){
        case 1:{
            cout << ">>> Enter your " << dataName << ":" << endl << "> ";
            getline(cin, data);
            break;
        }
        case 2:{
            string filename;
            cout << ">>> Enter the file name of the " << dataName << ":" << endl << "> ";
            getline(cin, filename);
            FileSource(filename.data(), true, new StringSink(data));
            break;
        }
        default:
            cout << "Invalid input!" << endl;
            exit(0);
            break;
    }
}

void printData(string data, int userSelection, string dataName = "data"){
    switch (userSelection){
        case 1:{
            cout << "Your " << dataName << ": " << data << endl;   
            break;
        }
        case 2: {
            string filename;
            cout << ">>> Enter the file name:" << endl
                 << "> ";
            getline(cin, filename);
            StringSource(data, true, new FileSink(filename.data(), data.size()));
            break;
        }
        default:
            cout << "Invalid input\n";
            exit(0);
            break;
    }
}

string decodeInput(int perform, string data){
    string decoded;
    decoded.clear();
    switch(perform){
        case 1:{
            decoded = data;
            break;
        }
        case 2:{
            decoded = hexdecode(data);
            break;
        }
        case 3:{
            decoded = base64decode(data);
            break;
        }
        default:
            cout << "Invalid input" << endl;
            exit(0);
            break;
    }
    return decoded;
}

string encodeOutput(int perform, string data){
    string encoded;
    encoded.clear();
    switch (perform){
        case 1:{
            encoded = data;
            break;
        }
        case 2:{
            encoded = hexencode(data);
            break;
        }
        case 3:{
            encoded = base64encode(data);
            break;
        }
        default:
            cout << "Invalid input!" << endl;
            exit(0);
            break;
    }
    return encoded;
}

string encodeOutput(int perform, CryptoPP::byte *data, int dataSize){
    string strdata; 
    StringSource(data, dataSize, true, new StringSink(strdata));
    return encodeOutput(perform, strdata);
}

CryptoPP::byte *generateBlock(unsigned int blockSize, int genSelection, string nameOfBlock){
    CryptoPP::byte *block = new CryptoPP::byte[blockSize];
    switch (genSelection){
        case 1: {
            // Random block and iv
            AutoSeededRandomPool prng;
            prng.GenerateBlock(block, blockSize);
            break;
        }
        case 2:{
            // Load block (hex) from screen
            string data;
            cout << "Please enter your " << nameOfBlock << " (hex - " << blockSize << "):"
                 << endl << "> ";
            getline(cin, data);
            StringSource(decodeInput(2, data).data(), true, new ArraySink(block, blockSize));
            break;
        }   
        case 3:{
            // Select performance of block/IV
            int performanceSelection;
            cout << ">>> Select encoding performance of the " << nameOfBlock << ":" << endl
                 << "1. Bytes" << endl
                 << "2. Hex" << endl
                 << "3. Base64" << endl
                 << "Please enter your number (1/2/3)?" << endl
                 << "> ";
            cin >> performanceSelection;
            cin.ignore();

            string fileblock, data;
            cout << "Please enter the path of your file which contains " << nameOfBlock << ":" << endl << "> ";
            getline(cin, fileblock);
            FileSource(fileblock.data(), true, new StringSink(data));
            // Decode block
            StringSource(decodeInput(performanceSelection, data).data(), true, new ArraySink(block, blockSize));
            break;
        }  
        default:
            cout << "Invalid input!" << endl;
            exit(0);
            break;
    }
    return block;
}


#endif