#include "class_server.h"

int main() {
    std::cout << "Входящие строки (с ошибками) выводятся в консоль.\n"
                 "Результат (восстановленная/исправленная строка) сохраняется в файл  output.txt"
                 " (директория /Users/username/ )\n\n";
    sockaddr_in structServerSockAddrIn;
    memset(&structServerSockAddrIn, 0, sizeof(structServerSockAddrIn));
    int socketListenFd = socket(PF_INET, SOCK_STREAM, 0);
    inet_aton("127.0.0.1", &structServerSockAddrIn.sin_addr);
    structServerSockAddrIn.sin_port = htons(1234);
    structServerSockAddrIn.sin_family = AF_INET;
    bind(socketListenFd, reinterpret_cast<sockaddr*>(&structServerSockAddrIn), sizeof(structServerSockAddrIn));
    listen(socketListenFd, 5);
    int socketAcceptFd = accept(socketListenFd, NULL, NULL);
    Server obj(socketAcceptFd);
    obj.ReceiveStringsDecoderWriteConsolAndOutFile();
    obj.CloseSocketAcceptFd();
    close(socketListenFd);
    return 0;
}
