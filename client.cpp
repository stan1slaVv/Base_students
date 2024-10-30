#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <ctype.h>
#include <arpa/inet.h>
#include "base.h"

#define  SERVER_PORT     5555
#define  SERVER_HOST    "127.0.0.1"
#define  BUFLEN          5

int  write_to_server(int fd);
int  read_from_server(int fd);


int main(int argc, char* argv[]) {
    int err;
    int sock;
    int valread;
    struct sockaddr_in server_addr;
    struct hostent* hostinfo;
    char buffer[1024] = { 0 };
    hostinfo = gethostbyname(SERVER_HOST);
    if (hostinfo == NULL) {
        fprintf(stderr, "unknown host %s.\n", SERVER_HOST);
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr = *(struct in_addr*)hostinfo->h_addr;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("client: socket was not created");
        exit(EXIT_FAILURE);
    }

    err = connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (err < 0) {
        perror("client: connect failure");
        exit(EXIT_FAILURE);
    }
    fprintf(stdout, "connection is ready\n");

    while (1) {
        fprintf(stdout, "enter sql query: ");

        for (int i = 0; i < 10; ++i) {
            fprintf(stdout, "query send: ");
            write_to_server(sock);
            if (read_from_server(sock) < 0) { break; };
            sleep(1);
        }
    }

    fprintf(stdout, "the end of life :(\n");

    close(sock);
    exit(EXIT_SUCCESS);
}



int write_to_server(int fd) {
    int nbytes;
    char buf[100];
    std::cout << "Enter data to send to the server (or '+' to send file): ";
    fgets(buf, 100, stdin);
    if (buf[0] == '+') {
        std::ifstream fin("base_work.txt");
        if (!fin.is_open()) {
            perror("cannot open base_work.txt");
            return -1;
        }
        std::stringstream ss;
        ss << fin.rdbuf();
        fin.close();
        std::string s = ss.str();
        size_t pos = 0;
        while (pos < s.size()) {
            size_t to_write = std::min(static_cast<size_t>(BUFLEN), s.size() - pos);
            nbytes = write(fd, s.c_str() + pos, to_write);
            if (nbytes < 0) {
                perror("write to server failed");
                return -1;
            }
            pos += to_write;
        }
        // Send end-of-data signal
        nbytes = write(fd, "*", 1);
        return 0;
    }
    else {
        buf[strlen(buf) - 1] = 0; // Remove newline character
        std::string s(buf);
        cout << s;
        size_t pos = 0;
        while (pos < s.size()) {
            size_t to_write = std::min(static_cast<size_t>(BUFLEN), s.size() - pos);
            nbytes = write(fd, s.c_str() + pos, to_write);
            if (nbytes < 0) {
                perror("write to server failed");
                return -1;
            }
            pos += to_write;
        }
        // Send end-of-data signal
        nbytes = write(fd, ";*", 2);
        return 0;
    }
}



int read_from_server(int fd) {
    char buf[BUFLEN];
    int nbytes;
    std::ofstream fin("received.txt");
    fin.close();
    FILE* ptrFile = fopen("received.txt", "a");
    if (!ptrFile) {
        perror("server: file open failure");
        return -1;
    }

    while (true) {
        memset(buf, 0, BUFLEN);
        nbytes = read(fd, buf, BUFLEN);
        std::cout << buf;
        if (nbytes < 0) {
            perror("server: read failure");
            fclose(ptrFile);
            return -1;
        }
        else if (nbytes == 0 || buf[0] == '*') {
            break;
        }
        else {
            size_t written = fwrite(buf, 1, nbytes, ptrFile);
            std::cout << "wrote";
            for (int i = 0; i < BUFLEN; i++) {
                if (buf[i] == '*') {
                    fclose(ptrFile);
                    return 0;
                }
            }
            if (written != nbytes) {
                perror("server: write failure");
                fclose(ptrFile);
                return -1;
            }
            std::cout << "server: wrote " << written << " bytes" << std::endl;
        }
    }

    fclose(ptrFile);
    return 0;
}