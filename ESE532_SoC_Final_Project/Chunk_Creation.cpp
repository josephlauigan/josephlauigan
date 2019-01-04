//
//  Chunk_Creation.cpp
//  ESE532 Project
//
//  Created by JD Lauigan on 12/6/17.
//  Copyright © 2017 Joseph Lauigan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Project1.h"




//#define POW (fmod(pow(PRIME,WINDOW_SIZE),MODULUS))
#define MASK (12)




#pragma SDS data access_pattern(Input:SEQUENTIAL, Output:SEQUENTIAL)
#pragma SDS data zero_copy(Input[0:MAX_DATA_SIZE], OUtput[0:MAX_FIFO_LENGTH])
unsigned int create_chunk_test(unsigned char Input[MAX_DATA_SIZE], unsigned int Output[MAX_FIFO_LENGTH])
{
    unsigned int i = 0, j = 0, hash = 0, k = 0, prime_c = PRIME;
    unsigned long long mod = MODULUS;
    unsigned char current, outbyte;
    unsigned char window[WINDOW_SIZE];
    unsigned int mask = (1 << MASK) - 1; //mask to check is last n bits are zero
    
    
    
    
    for(i = 0; i < WINDOW_SIZE; i++)
    {
        prime_c *= prime_c;
    }
    
    int POW = prime_c & (mod -1);
    
    
    for(i = 0; i < WINDOW_SIZE; i++)
    {
        current = Input[i];
        window[i] = current;
        hash += current;
    }
    
    for(i=WINDOW_SIZE; i<MAX_DATA_SIZE;i++)
    {
        current = Input[i];
        outbyte = window[0];
        outbyte *= POW;
        for(j = 0; j < WINDOW_SIZE-1;j++)
        {
            window[j] = window[j+1];
        }
        window[WINDOW_SIZE-1] = current;
        
        hash = (hash*PRIME + current - outbyte);
        hash = hash & (mod - 1);
        if((hash&mask) == 0)
        {
            Output[k++] = i; //output length offset for cut
        }
        
    }
    
    

    
    return k;
}

