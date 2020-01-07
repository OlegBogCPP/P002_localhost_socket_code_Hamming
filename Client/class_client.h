#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <filesystem>

class Client {
public:
    Client(int sockFd, std::string pathFile);
    void ReadAndSendFileWithHashSum();
    void CloseInputFile();
private:
    char MaskOneHashSumA(char ch);
    char MaskOneHashSumB(char ch);
    char MaskTwoHashSumA(char ch);
    char MaskTwoHashSumB(char ch);
    char MaskThreeHashSumA(char ch);
    char MaskThreeHashSumB(char ch);
    char MaskFourHashSumA(char ch);
    char MaskFourHashSumB(char ch);
    char MaskFiveHashSumB(char ch);
    bool SumBitCharIsOdd(char ch);
    void CreateCoderStringWithHashSum(std::string& buff);
    void AddErrorsInString(std::string& buff);

    int sockFd;
    std::string buf;
    std::string bufSynchronization = {'0'};
    std::fstream input;
};

