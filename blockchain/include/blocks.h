#ifndef block_h
#define block_h

#include <util.h>
#include <block.h>

using namespace std;

#include <vector>

class user_lib {
public:
    user_lib(string name,long long blcks){
        blocks_from = blcks;
        user = name;
    }
    string user;
    int blocks_from;
};

vector<user_lib*> lib;

int have_yet(string user){
    for(int i = 0;i < lib.size();i++){
        if(lib[i]->user == user)return i;
    }
    return -1;
}

namespace blocks {
long long amount = 0;

string add(ofstream* fout,block prev_block,block new_block){
    fout->open("blockchain.txt",ios::app);

    if(prev_block.this_hash == new_block.prev_hash &&
       new_block.this_hash == generate_hash(new_block) &&
       new_block.id - prev_block.id == 1 &&
       nice_hash(new_block.this_hash) >= new_block.difficulty && new_block.bytes > 0){
       *fout << "[block] " << endl;
       *fout << "\tid: " << new_block.id << endl;
       *fout << "\tuser: " << new_block.user << endl;
       *fout << "\ttime: " << new_block.time << endl;
       *fout << "\tdata: " << new_block.data << endl;
       *fout << "\tprev_hash: " << new_block.prev_hash << endl;
       *fout << "\tnonce: " << new_block.nonce << endl;
       *fout << "\tdifficulty: " << new_block.difficulty << endl;
       *fout << "\tsize: " << new_block.bytes << " bytes" << endl;
       *fout << "\tthis_hash: " << new_block.this_hash << endl;
       *fout << "[end] " << endl;
       blocks::amount++;
       fout->close();
       return "ok";
    }
    else {
        fout->close();
        return "error";
    }
}

void print(block new_block){
    cout << "[block]" << endl;
    cout << "\tid: " << new_block.id << endl;
    cout << "\tuser: " << new_block.user << endl;
    cout << "\ttime: " << new_block.time << endl;
    cout << "\tdata: " << new_block.data << endl;
    cout << "\tprev_hash: " << new_block.prev_hash << endl;
    cout << "\tnonce: " << new_block.nonce << endl;
    cout << "\tdifficulty: " << new_block.difficulty << endl;
    cout << "\tsize: " << new_block.bytes << " bytes" << endl;
    cout << "\tthis_hash: " << new_block.this_hash << endl;
    cout << "[end]";
}

block read(ifstream* fin,long long id){
    string buffer;
    fin->open("blockchain.txt");
    block new_block;
    while(!fin->eof()){
        *fin >> buffer;
        if(fin->eof())break;
        if(buffer == "[block]"){
            *fin >> buffer >> new_block.id;
            *fin >> buffer >> new_block.user;
            *fin >> buffer >> new_block.time;
            *fin >> buffer >> new_block.data;
            *fin >> buffer >> new_block.prev_hash;
            *fin >> buffer >> new_block.nonce;
            *fin >> buffer >> new_block.difficulty;
            *fin >> buffer >> new_block.bytes;
            *fin >> buffer;
            *fin >> buffer >> new_block.this_hash;
            *fin >> buffer;
            if(buffer == "[end]"){
                new_block.valid = true;
            }
        }
        if(new_block.id == id){
            fin->close();
            return new_block;
        }
    }
    fin->close();
    return new_block;
}
}

namespace chain {
    block init(ifstream* fin){
        string buffer;
        fin->open("blockchain.txt");
        block new_block;
        while(!fin->eof()){
            *fin >> buffer;
            if(fin->eof())break;
            if(buffer == "[block]"){
                *fin >> buffer >> new_block.id;
                *fin >> buffer >> new_block.user;
                *fin >> buffer >> new_block.time;
                *fin >> buffer >> new_block.data;
                *fin >> buffer >> new_block.prev_hash;
                *fin >> buffer >> new_block.nonce;
                *fin >> buffer >> new_block.difficulty;
                *fin >> buffer >> new_block.bytes;
                *fin >> buffer;
                *fin >> buffer >> new_block.this_hash;
                *fin >> buffer;
                if(buffer == "[end]"){
                    new_block.valid = true;
                }
            }
            blocks::amount++;

            int having = have_yet(new_block.user);
            if(having != -1){
                lib[having]->blocks_from++;
            }
            else {
                lib.push_back(new user_lib(new_block.user,1));
            }

        }
        fin->close();
        return new_block;
    }

    string is_valid(ifstream* fin){
        string buffer;
        fin->open("blockchain.txt");
        block prev_block,new_block;

        *fin >> buffer;
        if(buffer == "[block]"){
            *fin >> buffer >> prev_block.id;
            *fin >> buffer >> prev_block.user;
            *fin >> buffer >> prev_block.time;
            *fin >> buffer >> prev_block.data;
            *fin >> buffer >> prev_block.prev_hash;
            *fin >> buffer >> prev_block.nonce;
            *fin >> buffer >> prev_block.difficulty;
            *fin >> buffer >> prev_block.bytes;
            *fin >> buffer;
            *fin >> buffer >> prev_block.this_hash;
            *fin >> buffer;
            if(buffer == "[end]"){
                prev_block.valid = true;
            }
        }

        if(prev_block.this_hash != generate_hash(prev_block)){
            fin->close();
            return "error on block #" + int_to_str(prev_block.id);
        }

        while(!fin->eof()){
            *fin >> buffer;
            if(fin->eof())break;
            if(buffer == "[block]"){
                *fin >> buffer >> new_block.id;
                *fin >> buffer >> new_block.user;
                *fin >> buffer >> new_block.time;
                *fin >> buffer >> new_block.data;
                *fin >> buffer >> new_block.prev_hash;
                *fin >> buffer >> new_block.nonce;
                *fin >> buffer >> new_block.difficulty;
                *fin >> buffer >> new_block.bytes;
                *fin >> buffer;
                *fin >> buffer >> new_block.this_hash;
                *fin >> buffer;
                if(buffer == "[end]"){
                    new_block.valid = true;
                }

                if(new_block.this_hash != generate_hash(new_block) ||
                   new_block.prev_hash != generate_hash(prev_block)){
                    fin->close();
                    return "error on block #" + int_to_str(new_block.id);
                }

                prev_block = new_block;
            }
        }

        fin->close();
        return "ok";
    }
};

#endif // block_h
