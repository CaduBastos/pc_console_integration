#include <iostream>
#include <iomanip> // Para std::hex e std::setfill
#include <sstream> // Para std::ostringstream

#include <winsock2.h>
#include <ws2tcpip.h>

#include <ctime>
#include <chrono>
#include <windows.h>

#include "tcpip.h"

std::string calc_checksum(std::string msg);

//#pragma comment(lib, "ws2_32.lib") //Link winsock library

int main(){

    while(true){

        auto now = std::chrono::system_clock::now();
        std::time_t current_time = std::chrono::system_clock::to_time_t(now); //conversão de timestamp para segundos
        std::tm* local_time = std::localtime(&current_time);

        //"(&KW122010102111500A9)";
      
        //Aquisição e armazenamento da data e hora do PC
        std::string year = std::to_string(1900 + local_time->tm_year);
        year = year.substr(year.size() -2);

        std::string month = std::to_string(1 + local_time->tm_mon);
        if(month.length() < 2)
            month.insert(0, "0");
        
        std::string day = std::to_string(local_time->tm_mday);
        if(day.length() < 2)
            day.insert(0, "0");

        std::string hour = std::to_string(local_time->tm_hour);
        if(hour.length() < 2)
            hour.insert(0, "0"); 

        std::string min = std::to_string(local_time->tm_min);
        if(min.length() < 2)
            min.insert(0, "0");

        std::string sec = std::to_string(local_time->tm_sec);
        if(sec.length() < 2)
            sec.insert(0, "0");  

        std::string msg = "(&KW1" +
                        year +
                        month +
                        day +
                        "0" + std::to_string(local_time->tm_wday + 1) + 
                        hour +
                        min +
                        sec +
                        ")";
        msg.insert(19, calc_checksum(msg));

        char ip_address[] = "192.168.0.92";
        int port = 1771;

        // Leitura do IP e porta
        std::cout << "IP: ";
        std::cin >> ip_address;
        std::cout << "Port: ";
        std::cin >> port; 

        if(start_winsock()==0){
            std::cout << "Winsock started with success" << std::endl;
        }else
            std::cerr << "Fail to create socket! ERROR CODE: " << WSAGetLastError() << std::endl;

        if(connect_to_server(ip_address, port)==0){
            std::cout << "Connection established to " << ip_address << ":" << port << std::endl;
        }else
            std::cerr << "Connection to server failed! ERROR CODE: " << WSAGetLastError() << std::endl;
        
        // Mensagem a ser enviada
        // Const char* msg = "(&KW122010102111500A9)";
        if(send_msg(msg)==0)
            std::cout << "Message sended! MESSAGE: " << msg << std::endl;
        else
            std::cerr << "Failed to send message. ERROR CODE: " << WSAGetLastError() << std::endl;

        // Recepção
        //char buffer[1024] = rcv_msg();
        close_socket();   
    }
}
//Função para cálculo do checksum de soma de caracteres
std::string calc_checksum(std::string msg){

    int acc = 0;
    for(int i=1; i<=18; i++){ //Começa a varrer a strig a partir do '(' até o ')'       
        acc += static_cast<int>(msg[i]); //Pega o valor inteiro correspondente ao código ascii de cada caractere   
    }
    std::ostringstream oss;
    oss << std::uppercase << std::hex << acc; //Formata o valor de acc como hexadecimal
    std::string checksum = oss.str(); //Converte o conteúdo do stream para string
    checksum.erase(0, 1);
    return checksum;
}


