#pragma once

#include "block.h"

using namespace std;

string rand_string(int len, long long seed){
    srand(seed);
    char* alphabet = "ABCDEFGHIJKLMNOPQRSTUYWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string tmp = "";
    for(int i = 0;i < len;i++){
        tmp += alphabet[rand() % strlen(alphabet)];
    }
    return tmp;
}

string generate_hash(block temp){
    stringstream io;
    io.clear();
    io << temp.id << temp.user << temp.time << temp.data << temp.prev_hash << temp.nonce << temp.difficulty << temp.bytes;
    return sha256(io.str());
}

string sum2bin(string checksum){
    string tmp = "";
    for(int i = 0;i < checksum.length();i++){
        switch(checksum[i]){
            case '0':
                tmp += "0000";
                break;
            case '1':
                tmp += "0001";
                break;
            case '2':
                tmp += "0010";
                break;
            case '3':
                tmp += "0011";
                break;
            case '4':
                tmp += "0100";
                break;
            case '5':
                tmp += "0101";
                break;
            case '6':
                tmp += "0110";
                break;
            case '7':
                tmp += "0111";
                break;
            case '8':
                tmp += "1000";
                break;
            case '9':
                tmp += "1001";
                break;
            case 'a':
                tmp += "1010";
                break;
            case 'b':
                tmp += "1011";
                break;
            case 'c':
                tmp += "1100";
                break;
            case 'd':
                tmp += "1101";
                break;
            case 'e':
                tmp += "1110";
                break;
            case 'f':
                tmp += "1111";
                break;
            default:
                return "error";
        }
    }
    return tmp;
}

int nice_hash(string tmp){
    string bin_tmp = sum2bin(tmp);
    int i = 0;
    while(bin_tmp[i] == '0')i++;
    return i;
}

string int_to_str(long long a){
    stringstream io;
    io << a;
    return io.str();
}

block str2block(string s){
    block new_block;
    string buffer;
    stringstream s2b;
    s2b.clear();
    s2b << s;
    s2b >> new_block.id;
    s2b >> new_block.user;
    s2b >> new_block.time;
    s2b >> new_block.data;
    s2b >> new_block.prev_hash;
    s2b >> new_block.nonce;
    s2b >> new_block.difficulty;
    s2b >> new_block.bytes;
    s2b >> new_block.this_hash;
    return new_block;
}

string _2ws(string a){
    string b = "";
    for(int i = 0;i < a.length();i++){
        if(a[i] == '$')b += ' ';
        else b += a[i];
    }
    return b;
}

string block2str(block b){
    stringstream io;
    io.clear();
    io << b.id << " " <<
          b.user << " " <<
          b.time << " " <<
          b.data << " " <<
          b.prev_hash << " " <<
          b.nonce << " " <<
          b.difficulty << " " <<
          b.bytes << " " <<
          b.this_hash;
    return io.str();
}

string ws2_(string a){
    string b = "";
    for(int i = 0;i < a.length();i++){
        if(a[i] == ' ')b += '$';
        else b += a[i];
    }
    return b;
}

