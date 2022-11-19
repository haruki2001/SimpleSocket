#ifndef _SERVER_HPP_
#define _SERVER_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>


class Server{
    private:
        char buf[1024];
        int rsize;
        int sockfd;
        int client_socketfd;
        struct sockaddr_in addr;
        struct sockaddr_in from_addr;
        socklen_t len;
        pthread_t receive_thread;

        static void *ServerReceiver(void* arg){
            Server *sp = (Server *)arg;
            while(1){
                int size = sp->Receive();
                if(size == 0){
                    break;
                }else if(size < 0){
                    // std::cout << "receive error" << std::endl;
                    break;
                }else{
                    float data;
                    memcpy(&data, sp->GetReceiveBuffer(), sizeof(sp->GetReceiveDataSize()));
                    std::cout << data << std::endl;
                }
            }
        }
    
    public:
        Server() : len(sizeof(sockaddr_in)){
            pthread_create(&receive_thread, NULL, ServerReceiver, this);
        }

        bool Init(){
            // 受信バッファ初期化
            memset(buf, 0, sizeof(buf));

            // ソケット作成
            if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
                std::cout << "socket error" << std::endl;
                return false;
            }

            // 待ち受け用 IPアドレス・ポート番号設定
            addr.sin_family = AF_INET;
            addr.sin_port = htons(1234);
            addr.sin_addr.s_addr = INADDR_ANY;

            // バインド
            if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
                std::cout << "bind error" << std::endl;
                return false;
            }

            // 接続要求待ち
            if(listen(sockfd, 1) < 0){
                std::cout << "listen error" << std::endl;
                return false;
            }

            // クライアントからのコネクト要求待ち
            if((client_socketfd = accept(sockfd, (struct sockaddr *)&from_addr, &len)) < 0 ) {
                std::cout << "accept error" << std::endl;
                return false;
            }
            pthread_create(&receive_thread, NULL, ServerReceiver, this);
 

            // std::cout << "Server Init Success" << std::endl;

            return true;
        }

        template<typename tn> int Send(tn &data){
            return send(client_socketfd, (char*)&data, sizeof(data), 0);
        }

        bool Receive(){
            if((rsize = recv(client_socketfd, buf, sizeof(buf), 0)) < 0){
                std::cout << "receive message type error" << std::endl;
                return false;
            }
            // std::cout << rsize << std::endl;
            return true;
        }

        char* GetReceiveBuffer(){
            return this->buf;
        }

        int GetReceiveDataSize(){
            return rsize;
        }

};

#endif