/*
 *  Header file for TCP IP connection at Windows SO
 * 
 *  This header is seto to IPV4 connections only  
 *
 *  Written by Carlos Eduardo Bastos at 20/12/2024
 *  cadulbastos@gmail.com
 * 
 */
#ifndef INCLUDE_LIBS
#define INCLUDE_LIBS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#endif

#ifndef STREAM
#define STREAM
#include <iostream>
#endif

SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// Start winsock
bool start_winsock(void){
    WSADATA WSAData; // Struct to store winsock version informations
    if(WSAStartup(MAKEWORD(2, 2), &WSAData) != 0){
        std::cerr << "Fail to start Winsock! ERROR CODE: " << WSAGetLastError() << std::endl;
        return 1; // Fail to start Windosck
    }
    // Create socket
    if(sock == INVALID_SOCKET){ 
        std::cerr << "Fail to create socket! ERROR CODE: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1; // Fail to create socket
    }
    return 0; // Socket created with success
}

// Connect to server
bool connect_to_server(char ip_address[], int port){
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET; // Set to IPV4
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = inet_addr(ip_address);
    if(connect(sock, (SOCKADDR*)&serverAddress, sizeof(serverAddress))==SOCKET_ERROR){
        std::cerr << "Failed to connect! ERROR CODE: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1; // Connection failed
    }
    return 0; // Connection succesfully
}

bool send_msg(std::string msg){
    if(send(sock, msg.c_str(), msg.size(), 0)==SOCKET_ERROR){
        std::cerr << "Failed to send message. ERROR CODE: " << WSAGetLastError() << std::endl;
        return 1;
    }else  
        return 0;
}

short int rcv_msg(){
    char buffer[1024]; // Buffer to store message received
    int bytesReceived = recv(sock, buffer, sizeof(buffer)-1, 0);

    if(bytesReceived > 0){
        buffer[bytesReceived] = '\0'; // insert terminator char
        return 0;
    }else if (bytesReceived==0){
        return 1; // Connection closed by the server
    }else{
        std::cerr << "Error on receptio. ERROR CODE: " << WSAGetLastError << std::endl;
        return 2; // Error on reception
    }  
}

// Close socket
bool close_socket(){
    closesocket(sock);
    WSACleanup();
    return 0;
}

