// server.cpp
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "server.h"

void listen(int port) {
    // Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return;
    }

    // Bind socket to the IP / port
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return;
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return;
    }
    
    std::cout << "Listening on port " << port << std::endl;

    // Accept a connection (blocking call)
    int addrlen = sizeof(address);
    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        std::cerr << "Accept failed" << std::endl;
        return;
    }
    
    std::cout << "Connection accepted" << std::endl;

    // Close the socket
    close(server_fd);
}

void sendMessage(const char* serverIp, int port, const char* message) {
    // Create socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return;
    }

    // Server address structure
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr)); // Zero out the structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 or IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, serverIp, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return;
    }

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return;
    }

    // Send the message
    send(sock, message, strlen(message), 0);
    std::cout << "Message sent" << std::endl;

    // Close the socket
    close(sock);
}

void convert_format(){
    //To Be Finished

}

void add_to_list(std::tuple<double, double, double> info, std::vector<std::tuple<double, double, double>> node_info){
    node_info.push_back(info);
}

void run_Aequitas(std::tuple<double, double, double> node_info){
    //To be Finished
}


// // start the server and make it functional
// int main() {

/*   Example
     const char* serverIp = "127.0.0.1"; // Example server IP address
     int port = 8080; // Example port
     const char* message = "Hello from sender!";
     sender.sendMessage(serverIp, port, message); 
*/     


//     TODO: define a simple protocol for communication: send an index together with message. 
//     E.g. sendMessage(serverIp, port, message), message = (1, Aleph)

//     TODO: convert_format() and add_to_list()

//     TODO: run_Aequitas() for all tuples in the list

//     TODO: send back computed values to nodes
     
//     return 0;
// }