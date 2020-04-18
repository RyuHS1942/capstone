#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>
 
void ErrorHandling(char *message);
int main(int argc, char** argv)
{
    WSADATA wsadata;
    SOCKET ServSock, CliSock;
    SOCKADDR_IN servAddr, cliAddr;
    int szCliAddr;
 
    char message[] = "hello ! Welcome !";
 
    if (argc != 2)
        ErrorHandling("Port ERROR\n");
 
 
    //윈도우 소켓 초기화, 윈도우 소켓을 이용하기 위해서는 반드시 초기화 해주어야 한다.
    //윈도우즈에서 소켓 프로그래밍에서 제일 먼저 호출 하게 되는것이 WSAStartup 함수이다.
    //이 함수는  UNIX 소켓 프로그램에는 없는것으로 WSACleanup 함수와 쌍을 이뤄 소켓 프로그램의
    //시작과 끝을 나타낸다.이 함수가 하는 일은 윈속 동적 연결 라이브러리를 초기화하고
    //윈속 구현이 애플리케이션 요구사항을 충족하는지 확인한다.
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
        ErrorHandling("WSAStartup Error\n");
 
    //소켓 생성 : socket(int domain, int type, int protocol)
    //PF_INET : IPv4 인터넷 프로토콜  , PF_INET6 : IPv6 인터넷 프로토콜  , PF_LOCAL : 같은 시스템 내의 프로세스끼리 통신
    //SOCK_STREAM : TCP/IP 프로토콜  , SOCK_DGRAM : UDP/IP 프로토콜
    //통신에 있어 특정 프로토콜 사용하기 위한 변수로 보통 0 사용
    ServSock = socket(PF_INET, SOCK_STREAM, 0);
 
    if (ServSock == INVALID_SOCKET)
        ErrorHandling("socket 생성 실패\n");
 
    memset(&servAddr, 0, sizeof(servAddr));
    //PF_INET은 프로토콜 체계, AF_INET은 주소 체계
    //AF_INET 은 IPv4 , AF_INET6 는 IPv6, AF_LOCAL 은 LOCAL 통신용
    servAddr.sin_family = AF_INET;
    //사용할 포트 번호 지정
    servAddr.sin_port = htons(atoi(argv[1]));
 
    //INADDR_ANY 는 서버의 IP주소를 자동으로 찾아서 대입해주는 함수이다.
    //서버는 NIC을 2개 이상 가지고 있는 경우가 많은데 만일 특정 NIC의 IP주소를 sin_addr.s_addr에 지정하면 다른 NIC에서 연결된 요청은
    //서비스 할 수 없게 된다. 이때 INADDR_ANY를 이용하면 두 NIC를 모두 바인딩해주므로 어느 IP를 통해 접속해도 서비스가 가능해짐.
    //IP주소를 INADDR_ANY 로, 포트 번호를 9000으로 할 경우 => 현재 서버 컴퓨터의 9000번 포트를 목적지로 하는 모든 연결 요청을 해당 서버
    //응용프로그램에서 처리하겠다는 의미.
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
 
    //소켓에 IP주소와 PORT번호 할당
    //bind 함수의 인자로 소켓객체, 소켓 객체에 부여할 주소정보를 포함한 구조체, 데이터 길이
    if (bind(ServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
        ErrorHandling("bind() ERROR\n");
 
    //생성한 소켓을 서버 소켓으로 완성
    //listen 함수의 인자는 socket 객체와 backlog 연결대기의 크기(his130.tistroy.com 블로그 참고자료)
    if (listen(ServSock, 5) == SOCKET_ERROR)
        ErrorHandling("listen() ERROR\n");
 
    szCliAddr = sizeof(cliAddr);
    //클라이언트 연결 요청 수락을 위해 accept()함수 호출
    CliSock = accept(ServSock, (SOCKADDR*)&cliAddr, &szCliAddr);
    if (CliSock == INVALID_SOCKET)
        ErrorHandling("accept() ERROR\n");
    else
        printf("CONNECTION SUCCESS . . . . . . .\n");
 
    //send()함수를 통해서 클라이언트에 메세지 전송
    send(CliSock, message, sizeof(message), 0);
 
    closesocket(CliSock);
    closesocket(ServSock);
    WSACleanup();
    return;
 
}
 
 
void ErrorHandling(const char *message)
{
    printf("%s", message);
    //fputs(message, stderr);
    //fputs("\n", stderr);
    exit(1);
}

