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
#endif

#ifndef STREAM
#define STREAM
#include <iostream>
#endif

SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

// Start winsock
bool start_winsock(void){
    WSADATA WSAData; // Struct to store winsock version informations
    if(WSAStartup(MAKEWORD(2, 2), &WSAData) != 0)
        return 1; // Fail to start up Windosck
    return 0; // Winsock started with success
}

// Create socket
bool create_socket(void){
    if(sock == INVALID_SOCKET){
        WSACleanup();
        return 1;
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
        return 1; // Connection succesfuly
    }
    return 0; // Connection failed
}

