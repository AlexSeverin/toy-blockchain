#include <iostream>
#include <locale.h>
#include <ctime>
#include <sha256.h>
#include <cstdlib>
#include <cstring>
#include <sstream>

#include <sdl.h>
#include <sdl_net.h>

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

string rand_string(int len){
    char* alphabet = "ABCDEFGHIJKLMNOPQRSTUYWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    string tmp = "";
    for(int i = 0;i < len;i++){
        tmp += alphabet[rand() % strlen(alphabet)];
    }
    return tmp;
}

void print_block(block new_block){
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

#define SDL_main main

int main(){
    srand(time(NULL));
    cout << "Welcome to blocks-generator v.18.1" << endl << endl;
    block new_block;

    cout << "Connecting..." << endl;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();

    IPaddress ip;
    SDLNet_ResolveHost(&ip,"127.0.0.1",49001);

    string cmd,user;
    stringstream io;
    char buffer[2048];

    long difficulty;
    long long id;
    string last_hash;

    cout << "OK! Validating..." << endl;

    TCPsocket client;

    client = SDLNet_TCP_Open(&ip);
    SDLNet_TCP_Send(client,"is-valid",9);
    SDLNet_TCP_Recv(client,buffer,2048);
    SDLNet_TCP_Close(client);

    cout << buffer << endl;

    again:

    cout << "use user \'exit\' to exit" << endl;

    cout << "User: ";
    cin >> user;

    if(user == "exit" || user == "exit-with-halt"){
        if(user == "exit-with-halt"){
            client = SDLNet_TCP_Open(&ip);

            SDLNet_TCP_Send(client,user.c_str(),user.length() + 1);
            SDLNet_TCP_Recv(client,buffer,2048);

            cout << buffer << endl;
            SDLNet_TCP_Close(client);
        }

        SDLNet_Quit();
        SDL_Quit();

        return 0;
    }

    do{
        cout << user << " > ";
        cin >> cmd;

        if(cmd == "new"){
            cout << "Actual information updating..." << endl;
            client = SDLNet_TCP_Open(&ip);
            SDLNet_TCP_Send(client,"lblock",7);
            SDLNet_TCP_Recv(client,buffer,2048);
            SDLNet_TCP_Close(client);
            io.clear();
            io << buffer;
            io >> id;

            client = SDLNet_TCP_Open(&ip);
            SDLNet_TCP_Send(client,"lhash",6);
            SDLNet_TCP_Recv(client,buffer,2048);
            SDLNet_TCP_Close(client);
            io.clear();
            io << buffer;
            io >> last_hash;

            client = SDLNet_TCP_Open(&ip);
            string cmd_pack = "cnt-my-dif " + user;
            SDLNet_TCP_Send(client,cmd_pack.c_str(),cmd_pack.length() + 1);
            SDLNet_TCP_Recv(client,buffer,2048);
            SDLNet_TCP_Close(client);
            io.clear();
            io << buffer;

            io >> difficulty;

            cout << "latest_id: " << id << endl;
            cout << "latest_hash: " << last_hash << endl;
            cout << "wanted_difficulty: " << difficulty << endl;

            string data;
            cout << "Enter block data: ";
            cin >> data;

            cout << "Starting..." << endl;
            cout << "Picking nonce..." << endl;

            new_block.id = id + 1;
            new_block.user = user;
            new_block.prev_hash = last_hash;
            new_block.difficulty = difficulty;
            new_block.data = data;
            new_block.time = time(NULL);
            new_block.this_hash = "";
            new_block.bytes = block2str(new_block).length() + 144;

            do{
                new_block.nonce = rand_string(10);
            }while(nice_hash(generate_hash(new_block)) < new_block.difficulty);

            new_block.this_hash = generate_hash(new_block);

            cout << "Sharing block:" << endl;
            print_block(new_block);
            cout << endl;

            string b2s = "new " + ws2_(block2str(new_block));

            client = SDLNet_TCP_Open(&ip);
            SDLNet_TCP_Send(client,b2s.c_str(),b2s.length() + 1);

            SDLNet_TCP_Recv(client,buffer,2048);

            cout << buffer << endl;
            SDLNet_TCP_Close(client);
        }
        else if(cmd == "view"){
            cout << "Enter id: ";
            long long _id;
            cin >> _id;

            string to_send = "view " + int_to_str(_id);

            client = SDLNet_TCP_Open(&ip);
            SDLNet_TCP_Send(client,to_send.c_str(),to_send.length() + 1);
            SDLNet_TCP_Recv(client,buffer,2048);


            string h = "";
            h += buffer;

            cout << endl;

            print_block(str2block(_2ws(h)));

            cout << endl;

            SDLNet_TCP_Close(client);
        }
        else if(cmd == "upd"){
            cout << "Actual information updating..." << endl;
            client = SDLNet_TCP_Open(&ip);
            SDLNet_TCP_Send(client,"lblock",7);
            SDLNet_TCP_Recv(client,buffer,2048);
            SDLNet_TCP_Close(client);
            io.clear();
            io << buffer;
            io >> id;

            client = SDLNet_TCP_Open(&ip);
            SDLNet_TCP_Send(client,"lhash",6);
            SDLNet_TCP_Recv(client,buffer,2048);
            SDLNet_TCP_Close(client);
            io.clear();
            io << buffer;
            io >> last_hash;

            client = SDLNet_TCP_Open(&ip);
            string cmd_pack = "cnt-my-dif " + user;
            SDLNet_TCP_Send(client,cmd_pack.c_str(),cmd_pack.length() + 1);
            SDLNet_TCP_Recv(client,buffer,2048);
            SDLNet_TCP_Close(client);
            io.clear();
            io << buffer;

            io >> difficulty;

            cout << "latest_id: " << id << endl;
            cout << "latest_hash: " << last_hash << endl;
            cout << "wanted_difficulty: " << difficulty << endl;
        }
        else {

            client = SDLNet_TCP_Open(&ip);

            SDLNet_TCP_Send(client,cmd.c_str(),cmd.length() + 1);
            SDLNet_TCP_Recv(client,buffer,2048);

            cout << buffer << endl;
            SDLNet_TCP_Close(client);
        }

    }while(cmd != "exit" && cmd != "exit-with-halt");

    if(cmd != "exit-with-halt")goto again;

    SDLNet_Quit();
    SDL_Quit();

    return 0;
}
