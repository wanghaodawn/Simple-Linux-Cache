/********************************************************
 Finished by Hao Wang, Andrew ID: haow2
 Used to count hits, misses and evictions then returns
 ********************************************************/

#include "cachelab.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <getopt.h>
#include <unistd.h>
#include <string.h>

// Define cache units
struct cache_unit {
    int valid;
    unsigned tag;
    int LRU_Counter;
};

// Define global variables
struct cache_unit **cache;
int v = 0;
int s;
int E;
int b;

int hits = 0;
int misses = 0;
int evictions = 0;

char identifier;
unsigned address;
int size;
char* state;
int flag = 0;

// Cache initialization
void initCache() {
    int setsNum = 1 << s;
    int linesNum = E;
    cache = (struct cache_unit**) malloc(setsNum * sizeof(struct cache_unit*));
    
    for(int i = 0; i < setsNum; i++) {
        cache[i] = (struct cache_unit*) malloc (linesNum * sizeof(struct cache_unit));
        for(int j = 0; j < linesNum; j++) {
            cache[i][j].valid = 0;
            cache[i][j].tag = 0;
            cache[i][j].LRU_Counter = 0;
        }
    }
}

// Get the index of the cache
int getIndex() {
    return (address >> b) & ((1 << s) - 1);
}

// Get the tag of the cache
int getTag() {
    return (address >> (b + s)) & ((1 << (64 - s - b)) - 1);
}

// Judge whether there is a hit, miss or miss eviction.
int cacheCheck() {
    int index = getIndex();
    int tag = getTag();
    int maxCounter = 0;
    for(int i = 0; i < E; i++) {
        cache[index][i].LRU_Counter++;
    }
    for(int i = 0; i < E; i++) {
        if(cache[index][i].valid == 1) {
            if(cache[index][i].tag == tag) {
                cache[index][i].LRU_Counter = 0;
                hits++;
                state = "hit";
                return 1;
            }
        } else {
            cache[index][i].valid = 1;
            cache[index][i].tag = tag;
            cache[index][i].LRU_Counter = 0;
            misses++;
            state = "miss";
            return 2;
        }
        if(cache[index][i].LRU_Counter > cache[index][maxCounter].LRU_Counter) {
            maxCounter = i;
        }
    }
    cache[index][maxCounter].valid = 1;
    cache[index][maxCounter].tag = tag;
    cache[index][maxCounter].LRU_Counter = 0;
    evictions++;
    misses++;
    state = "miss eviction";
    return 3;
}

// Main function
int main(int argc, char **argv) {
    int opt;
    char* traceFile;
    
    // Loop within arguments
    while(-1 != (opt = getopt(argc, argv, "hvs:E:b:t:"))) {
        // Get the argument
        switch(opt) {
            case 'h':
                printf("They might be helpful to you:\n");
                printf("-h: Optional help flag that prints usage info\n");
                printf("-v: Optional verbose flag that displays trace info\n");
                printf("-s <s>: Number of set index bits (S = 2s is the number of sets)\n");
                printf("-E <E>: Associativity (number of lines per set)\n");
                printf("-b <b>: Number of block bits (B = 2b is the block size)\n");
                printf("-t <tracefile>: Name of the valgrind trace to replay\n");
                break;
            case 'v':
                v = 1;
                break;
            case 's':
                if(atoi(optarg) > -1) {
                    s = atoi(optarg);
                } else {
                    printf("Invalid value for s, try a nonnegative one!\n");
                }
                break;
            case 'E':
                if(atoi(optarg) > 0) {
                    E = atoi(optarg);
                } else {
                    printf("Invalid value for E, try a positive one!\n");
                }
                break;
            case 'b':
                if(atoi(optarg) > -1) {
                    b = atoi(optarg);
                } else {
                    printf("Invalid value for b, try a nonnegative one!\n");
                }
                break;
            case 't':
                traceFile = optarg;
                break;
            default:
                printf("Wrong argument, try others next time!\n");
                break;
        }
    }
    
    FILE *pFile;    // File pointer
    pFile = fopen(traceFile, "r");  // Open file in the reading mode
    initCache();    // Cache initialization

    if(traceFile != NULL) {
        while(fscanf(pFile, " %c %x,%d", &identifier, &address, &size) > 0) {
            if(identifier != 'I'){
                cacheCheck();
                if(v == 1) {
                    printf("%c %x,%d %s", identifier, address, size, state);
                }
                // M is different than others
                if(identifier == 'M') {
                    hits++;
                    state = "hit";
                    if(v == 1) {
                        printf(" %s\n", state);
                    }
                } else {
                    if(v == 1) {
                        printf("\n");
                    }
                }
            }
        }
    } else {
        printf("Cannot find the file!\n");
        return -1;
    }

    int S = 1 << s;
    printSummary(hits, misses, evictions);    // Output result
    fclose(pFile);    // Close the file
    // Free the cache
    for(int i = 0; i < S; i++) {
        free(cache[i]);
    }
    return 0;
}