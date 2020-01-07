#include "class_server.h"

Server::Server(int socketAcceptFd)
: socketAcceptFd(socketAcceptFd) {
    output.open("output.txt", std::ios::app);
}

void Server::CloseSocketAcceptFd() {
    close(socketAcceptFd);
}

void Server::ReceiveStringsDecoderWriteConsolAndOutFile() {
    int lenRecv = 0;
    char buf[BUFSIZ];
    char bufSynchronization[2] = {'0', '\0'};
    while ((lenRecv = recv(socketAcceptFd, buf, BUFSIZ, 0)) != 0) {
        buf[lenRecv] = '\0';
        send(socketAcceptFd, bufSynchronization, 2, 0);
        int sizeBuf = strlen(buf);
        bool sizeBufIsOdd = !((sizeBuf / 2) % 2);
        char temp = buf[sizeBuf / 2 - 1];

        buf[sizeBuf / 2 - 1] = '\0';
        std::cout << buf << '\n';
        buf[sizeBuf / 2 - 1] = temp;

        if (!(sizeBuf == 1 && buf[0] == '\n')) {
            int arrayValuesPositionHashSum[] = {0, 0, 0, 7, 0, 6, 5, 4, 0, 3, 2, 1, 0, 7, 6, 5, 0, 4, 3, 2, 1, 0};
            for (int i = 0; i < (sizeBufIsOdd ? (sizeBuf / 2) - 1 : sizeBuf / 2); i += 2) {
                int positionHashSum = 0;
                if (((SumBitCharIsOdd(MaskOneHashSumA(buf[i]))
                                != SumBitCharIsOdd(MaskOneHashSumB(buf[i + 1]))))
                                != ((buf[(sizeBuf / 2) + i] & 1) != 0)) {
                    positionHashSum += 1;
                }
                if (((SumBitCharIsOdd(MaskTwoHashSumA(buf[i]))
                                != SumBitCharIsOdd(MaskTwoHashSumB(buf[i + 1]))))
                                != ((buf[(sizeBuf / 2) + i] & (1 << 1)) != 0)) {
                    positionHashSum += 2;
                }
                if (((SumBitCharIsOdd(MaskThreeHashSumA(buf[i]))
                                != SumBitCharIsOdd(MaskThreeHashSumB(buf[i + 1]))))
                                != ((buf[(sizeBuf / 2) + i] & (1 << 2)) != 0)) {
                    positionHashSum += 4;
                }
                if (((SumBitCharIsOdd(MaskFourHashSumA(buf[i]))
                                != SumBitCharIsOdd(MaskFourHashSumB(buf[i + 1]))))
                                != ((buf[(sizeBuf / 2) + i] & (1 << 3)) != 0)) {
                    positionHashSum += 8;
                }
                if (SumBitCharIsOdd(MaskFiveHashSumB(buf[i + 1]))
                                != ((buf[(sizeBuf / 2) + i] & (1 << 4)) != 0)) {
                    positionHashSum += 16;
                }
                if (positionHashSum != 0) {
                    if (positionHashSum <= 12) {
                        buf[i] ^= (1 << arrayValuesPositionHashSum[positionHashSum]);
                    } else {
                        buf[i + 1] ^= (1 << arrayValuesPositionHashSum[positionHashSum]);
                    }
                }
            }
            if (sizeBufIsOdd) {
                int positionHashSum = 0;
                int i = (sizeBuf / 2) - 1;
                if (SumBitCharIsOdd(MaskOneHashSumA(buf[i])) != ((buf[(sizeBuf / 2) + i] & 1) != 0)) {
                    positionHashSum += 1;
                }
                if (SumBitCharIsOdd(MaskTwoHashSumA(buf[i])) != ((buf[(sizeBuf / 2) + i] & (1 << 1)) != 0)) {
                    positionHashSum += 2;
                }
                if (SumBitCharIsOdd(MaskThreeHashSumA(buf[i])) != ((buf[(sizeBuf / 2) + i] & (1 << 2)) != 0)) {
                    positionHashSum += 4;
                }
                if (SumBitCharIsOdd(MaskFourHashSumA(buf[i])) != ((buf[(sizeBuf / 2) + i] & (1 << 3)) != 0)) {
                    positionHashSum += 8;
                }
                if (positionHashSum != 0) {
                    buf[(sizeBuf / 2) + i] ^= (1 << arrayValuesPositionHashSum[positionHashSum]);
                }
            }
            buf[lenRecv / 2] = '\0';

        } else {
            buf[lenRecv] = '\0';
        }
        output << buf;
    }
}

char Server::MaskOneHashSumA (char ch) {
    return  ch & ((1<<1) | (1<<3) | (1<<4) | (1<<6) | (1<<7));
}

char Server::MaskOneHashSumB (char ch) {
    return  ch & ((1) | (1<<2) | (1<<4) | (1<<5) | (1<<7));
}

char Server::MaskTwoHashSumA (char ch) {
    return  ch & ((1<<1) | (1<<2) | (1<<4) | (1<<5) | (1<<7));
}

char Server::MaskTwoHashSumB (char ch) {
    return  ch & ((1<<2) | (1<<3) | (1<<5) | (1<<6));
}

char Server::MaskThreeHashSumA (char ch) {
    return  ch & (1 | (1<<4) | (1<<5) | (1<<6));
}

char Server::MaskThreeHashSumB (char ch) {
    return  ch & (1 | (1<<1) | (1<<5) | (1<<6) | (1<<7));
}

char Server::MaskFourHashSumA (char ch) {
    return  ch & (1 | (1<<1) | (1<<2) | (1<<3));
}

char Server::MaskFourHashSumB (char ch) {
    return  ch & ((1<<5) | (1<<6) | (1<<7));
}

char Server::MaskFiveHashSumB (char ch) {
    return  ch & (1 | (1<<1) | (1<<2) | (1<<3) | (1<<4));
}

bool Server::SumBitCharIsOdd(char ch) {
    int n = static_cast<int>(ch);
    n = ((n>>1) & 0x55) + (n & 0x55);
    n = ((n>>2) & 0x33) + (n & 0x33);
    n = ((n>>4) & 0x0F) + (n & 0x0F);
    return n % 2;
}


