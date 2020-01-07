#include "class_client.h"

Client::Client(int sockFd, std::string pathFile)
: sockFd(sockFd) {
    input.open(pathFile);
}

void Client::ReadAndSendFileWithHashSum() {
    while (getline(input, buf)) {
        int len = buf.size();
        if (len != 0) {
            while (len) {
                if (len <= 500) {
                    buf.resize(len + 1);
                    buf[len] = '\n';
                    CreateCoderStringWithHashSum(buf);
                    AddErrorsInString(buf);
                    send(sockFd, &buf[0], 2 * (len + 1), 0);
                    len = 0;
                    recv(sockFd, &bufSynchronization[0], 2, 0);
                } else {
                    std::string bufPart = buf.substr(0, 500);
                    CreateCoderStringWithHashSum(bufPart);
                    AddErrorsInString(bufPart);
                    send(sockFd, &bufPart[0], 1000, 0);
                    recv(sockFd, &bufSynchronization[0], 2, 0);
                    buf = buf.substr(500, len - 500);
                    len = buf.size();
                }
            }
        } else {
            buf.resize(1, '\n');
            send(sockFd, &buf[0], 1, 0);
            recv(sockFd, &bufSynchronization[0], 2, 0);
        }
    }
}

char Client::MaskOneHashSumA (char ch) {
    return  ch & ((1<<1) | (1<<3) | (1<<4) | (1<<6) | (1<<7));
}

char Client::MaskOneHashSumB (char ch) {
    return  ch & ((1) | (1<<2) | (1<<4) | (1<<5) | (1<<7));
}

char Client::MaskTwoHashSumA (char ch) {
    return  ch & ((1<<1) | (1<<2) | (1<<4) | (1<<5) | (1<<7));
}

char Client::MaskTwoHashSumB (char ch) {
    return  ch & ((1<<2) | (1<<3) | (1<<5) | (1<<6));
}

char Client::MaskThreeHashSumA (char ch) {
    return  ch & (1 | (1<<4) | (1<<5) | (1<<6));
}

char Client::MaskThreeHashSumB (char ch) {
    return  ch & (1 | (1<<1) | (1<<5) | (1<<6) | (1<<7));
}

char Client::MaskFourHashSumA (char ch) {
    return  ch & (1 | (1<<1) | (1<<2) | (1<<3));
}

char Client::MaskFourHashSumB (char ch) {
    return  ch & ((1<<5) | (1<<6) | (1<<7));
}

char Client::MaskFiveHashSumB (char ch) {
    return  ch & (1 | (1<<1) | (1<<2) | (1<<3) | (1<<4));
}

bool Client::SumBitCharIsOdd(char ch) {
    int n = static_cast<int>(ch);
    n = ((n>>1) & 0x55) + (n & 0x55);
    n = ((n>>2) & 0x33) + (n & 0x33);
    n = ((n>>4) & 0x0F) + (n & 0x0F);
    return n % 2;
}

void Client::CreateCoderStringWithHashSum(std::string& buff) {
    int lenPart = buff.size();
    buff.resize(lenPart * 2, static_cast<char>(64));
    int sizeBuf = buff.size();
    bool sizeBufIsOdd = !((sizeBuf / 2) % 2);
    for (int i = 0; i < (sizeBufIsOdd ? (sizeBuf / 2) - 1 : sizeBuf / 2); i += 2) {
        if (SumBitCharIsOdd(MaskOneHashSumA(*(buff.begin() + i)))
            != SumBitCharIsOdd(MaskOneHashSumB(*(buff.begin() + i + 1))))  {
            buff[(sizeBuf / 2) + i] |= 1;
        }
        if (SumBitCharIsOdd(MaskTwoHashSumA(*(buff.begin() + i)))
            != SumBitCharIsOdd(MaskTwoHashSumB(*(buff.begin() + i + 1))))  {
            buff[(sizeBuf / 2) + i] |= (1
                    << 1);
        }
        if (SumBitCharIsOdd(MaskThreeHashSumA(*(buff.begin() + i)))
            != SumBitCharIsOdd(MaskThreeHashSumB(*(buff.begin() + i + 1))))  {
            buff[(sizeBuf / 2) + i] |= (1 << 2);
        }
        if (SumBitCharIsOdd(MaskFourHashSumA(*(buff.begin() + i)))
            != SumBitCharIsOdd(MaskFourHashSumB(*(buff.begin() + i + 1)))) {
            buff[(sizeBuf / 2) + i] |= (1 << 3);
        }
        if (SumBitCharIsOdd(MaskFiveHashSumB(*(buff.begin() + i + 1)))) {
            buff[(sizeBuf / 2) + i] |= (1 << 4);
        }
    }
    if (sizeBufIsOdd) {
        int i = (sizeBuf / 2) - 1;
        if (SumBitCharIsOdd(MaskOneHashSumA(*(buff.begin() + i)))) {
            buff[(sizeBuf / 2) + i] |= 1;
        }
        if (SumBitCharIsOdd(MaskTwoHashSumA(*(buff.begin() + i)))) {
            buff[(sizeBuf / 2) + i] |= (1 << 1);
        }
        if (SumBitCharIsOdd(MaskThreeHashSumA(*(buff.begin() + i)))) {
            buff[(sizeBuf / 2) + i] |= (1 << 2);
        }
        if (SumBitCharIsOdd(MaskFourHashSumA(*(buff.begin() + i)))) {
            buff[(sizeBuf / 2) + i] |= (1 << 3);
        }
    }
}

void Client::AddErrorsInString(std::string& buff) {
    int sizeBuf = buff.size();
    bool sizeBufIsOdd = ((sizeBuf / 2) % 2) != 0;
    for (int i = 0; i < (sizeBufIsOdd ? ((sizeBuf / 2) - 1) : sizeBuf / 2); i += 2) {
        int j = 0;
        if (i % 2 == 0) {
            j = (i <= 7 ? i : i % 8);
        } else {
            j = ((i + 1) <= 7 ? i + 1 : (i + 1) % 8);
        }
        buff[i] ^= (1 << j);
    }
}

void Client::CloseInputFile(){
    input.close();
}

