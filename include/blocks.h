#ifndef block_h
#define block_h

#include <sha256.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

using namespace std;

struct block {
    long long id;
    long long time;
    string data;
    string prev_hash;
    bool valid = false;
};

namespace blocks {

block create(string data, block prev_block){
    long long now_time = time(NULL);
    ostringstream io;
    io << prev_block.id << " " << prev_block.time << " " << prev_block.data << " " << prev_block.prev_hash;
    string prev_hash = sha256(io.str());
    block new_block;
    new_block.id = prev_block.id + 1;
    new_block.time = now_time;
    new_block.data = data;
    new_block.prev_hash = prev_hash;
    return new_block;
}

void write(ofstream* fout, block temp){
    fout->open("blockchain.txt", ios::app);
    *fout << endl
          << "[block]" << endl;
    *fout << "\t"
          << "id: " << temp.id << endl;
    *fout << "\t"
          << "time: " << temp.time << endl;
    *fout << "\t"
          << "data: " << temp.data << endl;
    *fout << "\t"
          << "prev_hash: " << temp.prev_hash << endl;
    *fout << "[end]" << endl;
    fout->close();
}

void out(block temp){
    cout << "[block]" << endl;
    cout << "\t"
         << "id: " << temp.id << endl;
    cout << "\t"
         << "time: " << temp.time << endl;
    cout << "\t"
         << "data: " << temp.data << endl;
    cout << "\t"
         << "prev_hash: " << temp.prev_hash << endl;
    cout << "[end]" << endl
         << endl;
}

block read(ifstream* fin){
    string buffer;
    *fin >> buffer;
    block new_block;
    if (buffer == "[block]") {
        *fin >> buffer >> new_block.id;
        *fin >> buffer >> new_block.time;
        *fin >> buffer >> new_block.data;
        *fin >> buffer >> new_block.prev_hash;
        *fin >> buffer;
        if (buffer == "[end]") {
            new_block.valid = true;
            return new_block;
        }
    }
    new_block.valid = false;
    return new_block;
}
};

#endif // block_h
