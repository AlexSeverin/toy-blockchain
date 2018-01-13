#ifndef BLOCK_H_INCLUDED
#define BLOCK_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <sha256.h>
#include <sstream>
#include <fstream>

using namespace std;

struct block {
    long long id;
    long long time;
    string user;
    string data;
    string prev_hash;
    int difficulty;
    string nonce;
    long bytes;
    bool valid = false;
    string this_hash;
};

#endif // BLOCK_H_INCLUDED
