/*
* Desciption: This application opens a socket and constantly listens for message coming from port 62001.
*             It receives one byte of message each time and performs keyboard emulation according to the data received.
*             There is 100ms sleep after each keyboard action to mimic real-life scenario.
*/
#define PORT    62001  
//#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <iostream>

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <winsock2.h>
#include <Ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */

typedef int SOCKET;
#endif

int sockInit(void)
{
#ifdef _WIN32
    WSADATA wsa_data;
    return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
    return 0;
#endif
}

int sockQuit(void)
{
#ifdef _WIN32
    return WSACleanup();
#else
    return 0;
#endif
}

int sockClose(SOCKET sock)
{

    int status = 0;

#ifdef _WIN32
    status = shutdown(sock, 2);
    if (status == 0)
    {
        status = closesocket(sock);
    }
#else
    status = shutdown(sock, SHUT_RDWR);
    if (status == 0)
    {
        status = close(sock);
    }
#endif

    return status;

}

void error(const char* msg)
{
    perror(msg);
    // Make sure any open sockets are closed before calling exit
    exit(1);
}

int main()
{
    // This structure will be used to create the keyboard
    // input event.
    INPUT ip;
    int sockfd;
    socklen_t fromlen;
    char buffer;
    struct sockaddr_in serv_addr, from;

    sockInit();
    // Create the socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    // Make sure the socket was created
    if (sockfd < 0)
        error("ERROR opening socket");
    // Zero out the variable serv_addr
    memset((char*)&serv_addr, sizeof(serv_addr), 0);

    // Initialize the serv_addr
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    // Convert port number from host to network
    serv_addr.sin_port = htons(PORT);
    // Bind the socket to the port number
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    printf("Waiting on messages...\n");
    fromlen = sizeof(struct sockaddr_in);
    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;
    //Waiting for all procedures to finish
    Sleep(5000);
    while (true)
    {
        int n = recvfrom(sockfd, &buffer, 1023, 0, (struct sockaddr*)&from, &fromlen);
        if (n < 0)
            error("recvfrom");
        //buffer[n] = 0;  // Null terminate
        printf("Received a datagram: %s\n", buffer);
        //n = sendto(sockfd, "Got your message\n", 17, 0, (struct sockaddr*)&from, fromlen);
        //if (n < 0)
        //    error("sendto");

        //If the command is a W
        if ((buffer & 0b10000000) == 1) {
            ip.ki.wVk = 0x57; // virtual-key code for the "W" key
            ip.ki.dwFlags = 0; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
            // Release the "W" key
            ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            SendInput(1, &ip, sizeof(INPUT));
            Sleep(100);
            continue;
        }
        else if ((buffer & 0b01000000) == 1) {
            ip.ki.wVk = 0x41; // virtual-key code for the "A" key
            ip.ki.dwFlags = 0; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
            // Release the "A" key
            ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            SendInput(1, &ip, sizeof(INPUT));
            Sleep(100);
            continue;
        }
        else if ((buffer & 0b00100000) == 1) {
            ip.ki.wVk = 0x53; // virtual-key code for the "S" key
            ip.ki.dwFlags = 0; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
            // Release the "S" key
            ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            SendInput(1, &ip, sizeof(INPUT));
            Sleep(100);
            continue;
        }
        else if ((buffer & 0b00010000) == 1) {
            ip.ki.wVk = 0x44; // virtual-key code for the "D" key
            ip.ki.dwFlags = 0; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));

            // Release the "D" key
            ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            SendInput(1, &ip, sizeof(INPUT));
            Sleep(100);
            continue;
        }
        else if ((buffer & 0b00001000) == 1) {
            ip.ki.wVk = 0x45; // virtual-key code for the "D" key
            ip.ki.dwFlags = 0; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));

            // Release the "D" key
            ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            SendInput(1, &ip, sizeof(INPUT));
            Sleep(100);
            continue;
        }
        else {
            std::cout << "The input commmand which is " << buffer << ", is not valid! Please debug" << std::endl;
            exit(1);
        }
    }
    sockClose(sockfd);

    sockQuit();

#ifdef _WIN32
    std::cin.get();
#endif
    // Exit normally
    return 0;
}