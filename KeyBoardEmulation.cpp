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
    unsigned char buffer;
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
        int n = recvfrom(sockfd, (char*)&buffer, 1, 0, (struct sockaddr*)&from, &fromlen);
        //int n = 1;
        //buffer = 0b11000000;
        if (n < 0)
            error("recvfrom");
        //buffer[n] = 0;  // Null terminate
        printf("Received a datagram: %hhx\n", buffer);
        //n = sendto(sockfd, "Got your message\n", 17, 0, (struct sockaddr*)&from, fromlen);
        //if (n < 0)
        //    error("sendto");

        //If the command is a W
        //int d = (buffer & 0b01000000) >> 6;
        //std::cout << d << std::endl;
        if ((buffer >> 7) == 1) {
            ip.ki.wVk = 0x0; // virtual-key code for the "W" key
            ip.ki.wScan = 0x11;
            ip.ki.dwFlags = 0x8; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
            // Release the "W" key
            //Sleep(50);
            //ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            //SendInput(1, &ip, sizeof(INPUT));
        }
        if (((buffer & 0b01000000) >> 6) == 1) {
            ip.ki.wVk = 0x0; // virtual-key code for the "A" key
            ip.ki.wScan = 0x1E;
            ip.ki.dwFlags = 0x8; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
            // Release the "A" key
            //Sleep(50);
            //ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            //SendInput(1, &ip, sizeof(INPUT));
        }
        if (((buffer & 0b00100000) >> 5) == 1) {
            ip.ki.wVk = 0x0; // virtual-key code for the "S" key
            ip.ki.wScan = 0x1F;
            ip.ki.dwFlags = 0x8; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
            // Release the "S" key
            //Sleep(50);
            //ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            //SendInput(1, &ip, sizeof(INPUT));
        }
        if (((buffer & 0b00010000) >> 4) == 1) {
            ip.ki.wVk = 0x0; // virtual-key code for the "D" key
            ip.ki.wScan = 0x20;
            ip.ki.dwFlags = 0x8; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
            // Sleep(50);
            // Release the "D" key
            //ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            //SendInput(1, &ip, sizeof(INPUT));
        }
        if (((buffer & 0b00001000) >> 3) == 1) {
            ip.ki.wVk = 0x0; // virtual-key code for the "E" key
            ip.ki.wScan = 0x12;
            ip.ki.dwFlags = 0x8; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
            //Sleep(50);
            // Release the "E" key
            //ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            //SendInput(1, &ip, sizeof(INPUT));
        }
        if (((buffer & 0b00000100) >> 2) == 1) {
            ip.ki.wVk = 0x0; // virtual-key code for the "space" key
            ip.ki.wScan = 0x39;
            ip.ki.dwFlags = 0x8; // 0 for key press
            SendInput(1, &ip, sizeof(INPUT));
            //Sleep(50);
            // Release the "space" key
            //ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
            //SendInput(1, &ip, sizeof(INPUT));
        }
        //else {
        //    std::cout << "The input commmand which is " << buffer << ", is not valid! Please debug" << std::endl;
        //    exit(1);
        //}
        Sleep(50);
    }
    sockClose(sockfd);

    sockQuit();

#ifdef _WIN32
    std::cin.get();
#endif
    // Exit normally
    return 0;
}