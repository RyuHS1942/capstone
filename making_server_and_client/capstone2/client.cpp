#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>
 
void ErrorHandling(char *message);
 
int main(int argc, char** argv)
{
    WSADATA wsadata;
    SOCKET sock;
    SOCKADDR_IN ServAddr;
    char message[100];
    int len;
 
    if (argc != 3)
        ErrorHandling("ERROR OCCUR\n");
 
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
        ErrorHandling("WSAStartup Error\n");
 
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
        ErrorHandling("Socket Error\n");
 
    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = inet_addr(argv[1]);
    ServAddr.sin_port = htons(atoi(argv[2]));
 
    //생성된 소켓을 이용하여 서버에 연결 요청
    if (connect(sock, (SOCKADDR*)&ServAddr, sizeof(ServAddr)) == SOCKET_ERROR)
        ErrorHandling("connect Error\n");
 
    //recv 함수를 통해서 서버로 부터 전송되는 데이터 수신
    len = recv(sock, message, sizeof(message) - 1, 0);
    if (len == -1)
        ErrorHandling("recv Error\n");
 
    printf("MESSAGE FROM SERVER : %s", message);
 
    printf("SOCKET OVER\n");
    closesocket(sock);
    WSACleanup();
 
    return 0;
}
 
void ErrorHandling(const char *message)
{
    printf("%s", message);
    //fputs(message, stderr);
    //fputs("\n", stderr);
    exit(1);
}

