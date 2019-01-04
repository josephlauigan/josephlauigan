/*
 * lzw.h
 *
 *  Created on: Nov 7, 2017
 *      Author: William
 */

#ifndef SRC_LZW_H_
#define SRC_LZW_H_

//#define MAX_CHUNK_SIZE (2 * 8192)


#include <string>
#include "Project1.h"

void software_compress(const char input[MAX_DATA_SIZE], unsigned int length, unsigned char Output[MAX_DATA_SIZE]);
void hw_compress(const char input[MAX_DATA_SIZE], unsigned int length, unsigned char Output[MAX_DATA_SIZE]);
void test();


#endif /* SRC_LZW_H_ */