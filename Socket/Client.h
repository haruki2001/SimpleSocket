#ifndef _CLIENT_HPP_
#define _CLIENT_HPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

class Client{
    private:
        char buf[1024];
        int socket_fd;
        struct sockaddr_in addr;
        pthread_t receive_thread;
        int size;

        static void* ClientReceiver(void *arg){
            Client *cp = (Client *)arg;
            while(1){
                int size = cp->Receive();
                if(size == 0){
                    break;
                }else if(size < 0){
                    std::cout << "receive error" << std::endl;
                }else{
                    std::cout << cp->GetReceiveBuffer() << std::endl;
                }
            }
        }
    public:
        Client(){
            
        }

        bool Init(){
            // 受信バッファ初期化
            memset(buf, 0, sizeof(buf));

            // ソケット生成
            socket_fd = socket(AF_INET, SOCK_STREAM, 0);
            if(socket_fd < 0){
                std::cout << "socket error" << std::endl;
                return false;
            }

            // 送信先 IPアドレス・ポート番号設定
            memset(&addr, 0, sizeof(struct sockaddr_in));
            addr.sin_family = AF_INET;
            addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            addr.sin_port = htons(1234);

            // 接続要求
            if(connect(socket_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0){
                std::cout << "connect error" << std::endl;
                return false;
            }

            pthread_create(&receive_thread, NULL, ClientReceiver, this);

            return true;
        }

        template<typename tn> bool Send(tn data, size_t ssize){

            char msg[sizeof(ssize)];

            memcpy(&msg, &data, sizeof(ssize));

            // for(int i = 0; i < sizeof(msg); i++){
            //     printf("%c,", msg[i]);
            // }
            // printf("\n");

            // std::cout << sizeof(msg) << std::endl;
            // std::cout << "---------" << std::endl;

            if((size = send(socket_fd, msg, sizeof(msg), 0)) < 0){
                std::cout << "send message error" << std::endl;
                return false;
            }

            return true;
        }

        int Receive(){
            return recv(socket_fd, buf, sizeof(buf), 0);
        }

        char* GetReceiveBuffer(){
            return this->buf;
        }
};

#endif