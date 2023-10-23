#pragma once
#ifndef RUNNING_TIME_H
#define RUNNING_TIME_H
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

#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#endif

void block_running_time();

#endif