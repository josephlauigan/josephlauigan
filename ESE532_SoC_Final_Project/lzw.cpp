//
//  lzw.cpp
//  ESE532 Project
//
//  Created by JD Lauigan on 12/6/17.
//  Copyright © 2017 Joseph Lauigan. All rights reserved.
//

#include "lzw.h"
/*
 * lzw.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: William
 */

#include <map>
#include <iostream>
#include <vector>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string.h>



//this is defined in the beginning, but is a toggle for compression ratio
#define ALPHABET_SIZE 256
#define MAX_MATCH_LENGTH 16
#define MAX_CHUNK_SIZE 2*8192

int curr_bit = 0;
char curr_byte = 0;
int curr_index = 0;
int curr_size = 0; //for software_implementation

static int Get_length(int Last_code)
{
    int Length = 0;
    while(Last_code > 0)
    {
        Last_code >>= 1;
        Length++;
    }
    return Length;
}


std::string big_endian(int little_endian) {
    std::bitset<32> bit_sequence(little_endian);
    std::string bits_to_string = bit_sequence.to_string();
    int num_bits = log2(little_endian)+1;
    std::reverse(bits_to_string.begin(), bits_to_string.end());
    
    int first_one = bits_to_string.find('1');
    std::string ret_val = bits_to_string.substr(first_one, num_bits-first_one);
    return ret_val;
}


unsigned int little_to_big_endian(unsigned int little_endian) {
    unsigned int first_byte = little_endian & 0x000000FF;
    unsigned int second_byte = (little_endian & 0x0000FF00) >> 8;
    unsigned int third_byte = (little_endian & 0x00FF0000) >> 16;
    unsigned int fourth_byte = (little_endian & 0xFF000000) >> 24;
    
    return (first_byte << 24) | (second_byte << 16) | (third_byte << 8) | fourth_byte;
}

std::string create_bit_sequence(int value) {
    std::bitset<32> bit_sequence(value);
    std::string bits_to_string = bit_sequence.to_string();
    int num_bits = Get_length(curr_size + 1);
    std::string ret_val = bits_to_string.substr(32-num_bits, num_bits);
    return ret_val;
}



static void write_to_file(std::string compression, unsigned char Output[MAX_DATA_SIZE]) {
    int curr_output_index = 0;
    std::ofstream writer("test.enc", std::ios::binary);
    if(!writer.is_open()) {
        std::cerr << "cannot open file";
        return;
    }
    writer.imbue(std::locale::classic());
    for(unsigned int i=0; i<compression.length()/8; i++) {
        unsigned char c;
        std::string curr_bytes = compression.substr(i*8, 8);
        std::bitset<8> one_byte(curr_bytes);
        c = (one_byte.to_ulong() & 0xFFFF);
        writer.write((const char*)&c, sizeof(unsigned char));
    }
    writer.close();
}



//TODO: this assumes that we will never write two bytes, need to fix
static void write_value(int new_value, unsigned char Output[MAX_DATA_SIZE]) {
    int num_bits = Get_length(curr_size); //get number of bits we need to write
    int write_num_bits = ((curr_bit/8 + 1)*8 - curr_bit) % 8; //get number of bits we can write to put into Output
    write_num_bits = !write_num_bits ? 8 : write_num_bits;
    int bits_remaining = num_bits - write_num_bits; //get number of bits we need to save to write later
    int mask = (1 << write_num_bits) - 1; //get num ones we want to mask
    mask = (mask << (num_bits - write_num_bits));
    unsigned int bits_write = (curr_byte << (num_bits-curr_bit)) | ((new_value & mask) >> curr_bit); //put the bits we can write into the char
    Output[curr_index++] = bits_write >> (num_bits-8);
    int new_mask = (1 << bits_remaining) - 1;
    curr_byte = new_value & new_mask;
    curr_bit = bits_remaining;
    //Check to see if this will ever > 8
    if(curr_bit == 8) {
        Output[curr_index++] = curr_byte;
        curr_byte = 0;
        curr_bit = 0;
    }
}

void software_compress(const char input[MAX_DATA_SIZE], unsigned int length, unsigned char Output[MAX_DATA_SIZE]) {
    //SEE: https://rosettacode.org/wiki/LZW_compression#C.2B.2B
    std::map<std::string, int> dictionary;
    for(int i=0; i<ALPHABET_SIZE; i++) {
        dictionary[std::string(1, i)] = i;
    }
    
    std::string ret_val;
    std::string curr_word;
    curr_size = ALPHABET_SIZE;
    int max_size = (2 * 8192);
    for(unsigned int i=0; i<length; i++) {
        char curr_char = input[i];
        std::string next_seq = curr_word + curr_char;
        if(dictionary.count(next_seq)) {
            curr_word = next_seq;
        } else {
            int val = dictionary[curr_word];
            std::string fixed_val = create_bit_sequence(val); //big_endian(val);
            ret_val += fixed_val;
            if(curr_size < max_size) {
                dictionary[next_seq] = curr_size++;
            }
            curr_word.clear();
            curr_word += curr_char;
        }
    }
    if(!curr_word.empty()) {
        int val = dictionary[curr_word];
        std::string fixed_val = create_bit_sequence(val); //big_endian(val);
        ret_val += fixed_val;
    }
    int num_bits = ret_val.length(); //31 bits for size
    int remainder_bits = ((num_bits/8 + 1)*8 - num_bits) % 8;
    ret_val.append(remainder_bits, '0');
    //subtract one because it counts the first bit as well
    int size = ((num_bits+remainder_bits)/8);
    size = little_to_big_endian(size);
    size = size << 1;
    std::bitset<32> bits(size);
    ret_val.insert(0, bits.to_string());
    write_to_file(ret_val, Output);
}


typedef struct{
    unsigned int  value;
    unsigned char key[3];
} map;


//from http://www.cse.yorku.ca/~oz/hash.html djb2
unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;
    while (c == *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}

unsigned int get_hash_index(unsigned char *key, map *dictionary) {
    int hash_val = hash(key) % MAX_CHUNK_SIZE;
    while(dictionary[hash_val].key[0] && strncmp((const char*) key, (const char*) dictionary[hash_val].key, 3)) {
        hash_val++;
    }
    return hash_val;
}

//MAP is max 98KB
void hw_compress(const char input[MAX_DATA_SIZE], unsigned int length, unsigned char Output[MAX_DATA_SIZE]) {
    map dictionary[MAX_CHUNK_SIZE];
    curr_byte = 0;
    curr_bit = 0;
    curr_index = 4; //skip header for now
    //TODO: currently doesn't include 0, need to fix
    for(unsigned int i = 1; i<256; i++) {
        unsigned char key[3];
        key[0] = (unsigned char) i;
        key[1] = '\0';
        key[2] = '\0';
        int index = get_hash_index(key, dictionary);
        memcpy(dictionary[index].key, key, 3);
        dictionary[index].value = i;
    }
    curr_size = 256;
    unsigned char* curr_word = (unsigned char *)calloc(MAX_MATCH_LENGTH, sizeof(unsigned char));
    unsigned int curr_word_index = 0;
    for(unsigned int i=0; i<length; i++) {
        char curr_char = input[i];
        unsigned char next_seq[MAX_MATCH_LENGTH];
        memcpy((char *) next_seq, (const char*) curr_word, MAX_MATCH_LENGTH);
        next_seq[curr_word_index] = curr_char;
        unsigned int hash_index = get_hash_index(next_seq, dictionary);
        if(dictionary[hash_index].value && dictionary[hash_index].value < curr_size) {
            curr_word[curr_word_index++] = curr_char;
        } else {
            unsigned int hash_index = get_hash_index(curr_word, dictionary);
            int val = dictionary[hash_index].value;
            write_value(val, Output);
            if(curr_size < MAX_CHUNK_SIZE) {
                unsigned int hash_index = get_hash_index(next_seq, dictionary);
                strncpy((char*) dictionary[hash_index].key, (char*) next_seq, 3);
                dictionary[hash_index].value = curr_size++;
            }
            memset(curr_word,0,strlen((const char*) curr_word));
            curr_word_index = 0;
            curr_word[curr_word_index++] = curr_char;
        }
    }
    if(curr_word[0] != '\0') {
        unsigned int hash_index = get_hash_index(curr_word, dictionary);
        int val = dictionary[hash_index].value;
        write_value(val, Output);
    }
    Output[curr_index] = curr_byte << (8 - curr_bit); //write last byte
    int num_bytes = !curr_byte ? curr_index-4 : (curr_index+1)-4; //check if you needed to write last byte
    num_bytes = num_bytes << 1;
    char first_byte = num_bytes & 0x000000FF;
    char second_byte = (num_bytes & 0x0000FF00) >> 8;
    char third_byte = (num_bytes & 0x00FF0000) >> 16;
    char fourth_byte = (num_bytes & 0xFF000000) >> 24;
    Output[0] = first_byte;
    Output[1] = second_byte;
    Output[2] = third_byte;
    Output[3] = fourth_byte;
}