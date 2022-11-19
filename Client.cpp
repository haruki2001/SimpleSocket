#include <stdio.h>
#include <iostream>

#include "Socket/Client.h"

int main(){
    Client client;
    if(client.Init()){
        std::cout << "Client Success" << std::endl;
    }

    float a = 0.f;

    while(1){
        // char a[64] = "Helloaaaaaaaaaaaaaaaaaaaaaaa";
        // std::cout << sizeof(a) << std::endl;
        client.Send(a, sizeof(a));
        a += 0.1;
    }
}