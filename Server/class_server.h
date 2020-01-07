#pragma once

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>

class Server {
public:
    Server(int clientServerAccept);
    void ReceiveStringsDecoderWriteConsolAndOutFile();
    void CloseSocketAcceptFd();

private:
    char MaskOneHashSumA (char ch);
    char MaskOneHashSumB (char ch);
    char MaskTwoHashSumA (char ch);
    char MaskTwoHashSumB (char ch);
    char MaskThreeHashSumA (char ch);
    char MaskThreeHashSumB (char ch);
    char MaskFourHashSumA (char ch);
    char MaskFourHashSumB (char ch);
    char MaskFiveHashSumB (char ch);
    bool SumBitCharIsOdd(char ch);

    int socketAcceptFd;
    std::ofstream output;
};

