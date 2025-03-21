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
#include <cstring>
#endif

#ifndef STREAM
#define STREAM
#include <iostream>
#endif

#ifndef TCPIP_H
#define TCPIP_H
bool start_winsock();
bool connect_to_server(char ip_address[], int port);
bool send_msg(std::string msg);
int rcv_msg(char* buffer, size_t buffer_size);
bool close_socket();
#endif

SOCKET sock;

// Start winsock
bool start_winsock(void){
    WSADATA WSAData; // Struct to store winsock version informations
    if(WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
        return 1; // Fail to start Winsock
    
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Create socket
    if(sock == INVALID_SOCKET){ 
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
        closesocket(sock);
        WSACleanup();
        return 1; // Connection failed
    }
    return 0; // Connection succesfully
}

bool send_msg(std::string msg){
    if(send(sock, msg.c_str(), msg.size(), 0)==SOCKET_ERROR)
        return 1;
    else  
        return 0;
}

int rcv_msg(char* buffer, size_t buffer_size){
    memset(buffer, 0, buffer_size); // Clean the buffer
    ssize_t bytesReceived = recv(sock, buffer, buffer_size, 0);
    
    if(bytesReceived == 0)
        return 0; // Connection closed by the server
    else if(bytesReceived > 0){
        buffer[bytesReceived] = '\0'; // Insert terminator char
        return bytesReceived; // Return number of bytes received
    }else{
        return -1; // Error on reception
    }
}

// Close socket
bool close_socket(){
    closesocket(sock);
    WSACleanup();
    return 0;
}

