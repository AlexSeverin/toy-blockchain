#include <iostream>
#include <fstream>

#include <blocks.h>

using namespace std;

int main(){

    block prev_block,temp;
    string buffer;
    long long blocks = 0;

    ifstream* fin = new ifstream();
    ofstream* fout = new ofstream();

    fin->open("blockchain.txt");

    while(!fin->eof()){
        *fin >> buffer;
        if(fin->eof())break;
        if(buffer == "[block]"){
            *fin >> buffer >> prev_block.id;
            *fin >> buffer >> prev_block.time;
            *fin >> buffer >> prev_block.data;
            *fin >> buffer >> prev_block.prev_hash;
            *fin >> buffer;
            if(buffer == "[end]"){
                prev_block.valid = true;
            }
        }
        blocks++;
    }

    fin->close();

    cout << "blocks now: " << blocks << endl << "last-";
    blocks::out(prev_block);




    delete fout,fin;

    return 0;
}

