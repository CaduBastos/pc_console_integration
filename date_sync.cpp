#include <iostream>
#include <iomanip> // Para std::hex e std::setfill
#include <sstream> // Para std::ostringstream

#include <winsock2.h>
#include <ws2tcpip.h>

#include <ctime>
#include <chrono>
#include <windows.h>

std::string calc_checksum(std::string msg);

//#pragma comment(lib, "ws2_32.lib") //Link winsock library

int main(){

    if(true){

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

        //Leitura do IP e porta
        std::cout << "IP do console: ";
        std::cin >> ip_address;
        std::cout << "Porta de conexao: ";
        std::cin >> port; 
    
        WSADATA WSAData;        //Estrutura para armazenar informações sobre a versão do winsock
        //Inicializa a winsock
        if(WSAStartup(MAKEWORD(2, 2), &WSAData) != 0){ //Solicita a versão do winsock2
            std::cerr << "Falha ao inicilizar Winsock. ERROR CODE: " << WSAGetLastError() << std::endl; 
            return 1; 
        }
                    
        //criar socket
        SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if(sock == INVALID_SOCKET){
            std::cerr << "Erro ao criar socket. ERROR CODE: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return 1;
        }

        //Configura endereço do servidor
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET; //Configura para IPV4
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = inet_addr(ip_address);

        //Conecta ao servidor
        if(connect(sock, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR){
            std::cerr << "Erro ao conectar com a automacao. ERROR CODE: " << WSAGetLastError() << std::endl;
            closesocket(sock);
            WSACleanup();
            return 1;
        }

        std::cout << "Conexao estabelecida com " << ip_address << ":" << port << std::endl;

        //Mensagem a ser enviada
        //const char* msg = "(&KW122010102111500A9)";
        if(send(sock, msg.c_str(), msg.size(), 0) == SOCKET_ERROR){
            std::cerr << "Erro ao enviar a mensagem! ERROR CODE: " << WSAGetLastError() << std::endl;
        }
        else{
            std::cout << "TX: " << msg << std::endl; 
        }
        
        char buffer[1024]; //Buffer para armazenar o retorno
        int bytesReceived = recv(sock, buffer, sizeof(buffer)-1, 0);
    
        if(bytesReceived > 0){
            buffer[bytesReceived] = '\0'; //Adiciona o caractere terminador da string
            std::cout << "RX: " << buffer << std::endl;
        }else if (bytesReceived == 0){
            std::cout << "Conexão encerrada pelo servidor" << std::endl;
        }else{
            std::cerr << "Erro na recepção: " << WSAGetLastError() << std::endl;
        }

        closesocket(sock);     //Fechar o socket
        WSACleanup();          //Finalizar winsock  
        return 0;
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


