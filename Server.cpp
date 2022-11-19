#include <stdio.h>
#include <iostream>

#include "Socket/Server.h"

int main(){
    Server server;
    if(server.Init()){
        std::cout << "Sever Init Success" << std::endl;
    }

    while(1){

    }
    return 0;
}