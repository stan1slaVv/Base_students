#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "base.h"

#define PORT    5555
#define BUFLEN  5

int read_from_client(int fd, char* buf);
void write_to_client(int fd, char* buf, student_data* db);

int main(void) {
    int sock, new_sock;
    int opt = 1;
    struct sockaddr_in addr, client;
    char buf[BUFLEN];
    socklen_t size;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("server: cannot create socket");
        exit(EXIT_FAILURE);
    }

    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("server: cannot bind socket");
        close(sock);
        exit(EXIT_FAILURE);
    }

    if (listen(sock, 5) < 0) {
        perror("server: listen queue failure");
        close(sock);
        exit(EXIT_FAILURE);
    }

    std::cout << "server started\n";

    student_data students;

    while (1) {
        size = sizeof(client);
        new_sock = accept(sock, (struct sockaddr*)&client, &size);
        if (new_sock < 0) {
            perror("server: accept failure");
            close(sock);
            exit(EXIT_FAILURE);
        }

        fprintf(stdout, "server: connect from host %s, port %hu.\n",
            inet_ntoa(client.sin_addr),
            ntohs(client.sin_port));

        pid_t pid = fork();
        if (pid < 0) {
            perror("server: fork failure");
            close(new_sock);
            continue;
        }

        if (pid == 0) {
            // Дочерний процесс
            close(sock);
            while (1) {
                int err = read_from_client(new_sock, buf);
                if (err < 0) {
                    close(new_sock);
                    exit(EXIT_FAILURE);
                }
                if (strstr(buf, "stop")) {
                    close(new_sock);
                    exit(EXIT_SUCCESS);
                }
                else {
                    write_to_client(new_sock, buf, &students);
                }
            }
        }
        else {
            // Родительский процесс
            close(new_sock);
            waitpid(-1, NULL, WNOHANG); // Ждем завершения дочерних процессов
        }
    }

    close(sock);
    return 0;
}

int read_from_client(int fd, char* buf) {
    int nbytes;
    std::ofstream fin("base_work_server.txt");
    fin.close();
    FILE* ptrFile = fopen("base_work_server.txt", "a");
    if (!ptrFile) {
        perror("server: file open failure");
        return -1;
    }

    while (true) {
        memset(buf, 0, BUFLEN);
        nbytes = read(fd, buf, BUFLEN);
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

void write_to_client(int fd, char* buf, student_data* db) {
    int nbytes;
    std::ifstream commands("base_work_server.txt");
    std::ofstream fout("result_serv.txt");
    db->work(commands, fout);
    commands.close();
    fout.close();

    std::ifstream fin("result_serv.txt");

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
        }
        pos += to_write;
    }
    // Send end-of-data signal
    nbytes = write(fd, "*", 1);
}
