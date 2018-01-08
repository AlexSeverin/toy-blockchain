#include "blocks.h"

#include "sdl.h"
#include "sdl_net.h"

#define SDL_main main

using namespace std;

block str2block(string s){
    block new_block;
    string buffer;
    stringstream s2b;
    s2b.clear();
    s2b << s;
    s2b >> new_block.id;
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

int main(){
    block prev_block;

    ifstream* fin = new ifstream();
    ofstream* fout = new ofstream();

    cout << "Blockchain initializing..." << endl;

    prev_block = chain::init(fin);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDLNet_Init();

    IPaddress ip;
    SDLNet_ResolveHost(&ip,NULL,49001);

    TCPsocket server = SDLNet_TCP_Open(&ip);
    TCPsocket client;

    cout << "Resolving host on 49001..." << endl;
    cout << "Successfully! Listening..." << endl;

    char buffer[2048];
    stringstream io;
    string cmd;

    block temp_block;

    while(1){
        SDL_Delay(10);
        client = SDLNet_TCP_Accept(server);
        if(client){
            SDLNet_TCP_Recv(client,buffer,2048);

            io.clear();
            io << buffer;
            io >> cmd;

            if(cmd == "check"){
                SDLNet_TCP_Send(client,"ok",3);
            }

            else if(cmd == "cnt-my-dif"){
                int coins;
                io >> coins;
                string a = int_to_str(coins);
                SDLNet_TCP_Send(client,a.c_str(),a.length() + 1);
            }

            else if(cmd == "lblock"){
                SDLNet_TCP_Send(client,int_to_str(prev_block.id).c_str(),int_to_str(prev_block.id).length() + 1);
            }

            else if(cmd == "view"){
                long long id;
                io >> id;
                block temp = blocks::read(fin,id);
                string s_block = ws2_(block2str(temp));
                SDLNet_TCP_Send(client,s_block.c_str(),s_block.length() + 1);
            }

            else if(cmd == "lhash"){
                SDLNet_TCP_Send(client,prev_block.this_hash.c_str(),prev_block.this_hash.length() + 1);
            }

            else if(cmd == "is-valid"){
                string ans = chain::is_valid(fin);
                SDLNet_TCP_Send(client,ans.c_str(),ans.length() + 1);
            }

            else if(cmd == "new"){
                string s_block;
                io >> s_block;
                temp_block = str2block(_2ws(s_block));
                string ans = blocks::add(fout,prev_block,temp_block);
                if(ans == "ok"){
                    prev_block = temp_block;
                }
                SDLNet_TCP_Send(client,ans.c_str(),ans.length() + 1);
            }

            else if(cmd == "exit"){
                SDLNet_TCP_Send(client,"ok",3);
                SDLNet_TCP_Close(client);
                break;
            }

            else {
                SDLNet_TCP_Send(client,"error",6);
            }


            SDLNet_TCP_Close(client);
        }

    }

    SDLNet_TCP_Close(server);
    SDLNet_Quit();
    SDL_Quit();

    delete fout, fin;


    return 0;
}
