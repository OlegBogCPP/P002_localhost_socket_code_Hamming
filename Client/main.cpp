#include "class_client.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Ошибка: не указан файл входных данных\n"
                     "Для получения справки введите в Терминале имя исполняемого файла и \"--help\" в следующем "
                     "формате:\n"
                     " <client_exe> --help\n";
        return 1;
    }
    std::string pathFile = argv[1];
    if (pathFile == "--help") {
        std::cout << "Для запуска программы необходимо ввести в Терминал имя исполняемого файла"
                     " и аргумент программы в виде абсолютного пути к файлу с данными.\n"
                     "Формат ввода (путь обязательно указывать в \" \"):\n"
                     "<client_exe> \"/Users/username/Documents/Client/input.txt\"\n";
        return 1;
    }
    if(!std::filesystem::exists(pathFile)) {
        std::cout << "Ошибка: введенный абсолютный путь к файлу с данными не существует.\n"
                     "Введите корректный абсолютный путь к файлу (путь обязательно указывать в \" \") "
                     "и повторите запуск программы.";
        return 1;
    }
    sockaddr_in structClientSockAddrIn;
    memset(&structClientSockAddrIn, 0, sizeof(structClientSockAddrIn));
    int sockFd = socket(PF_INET, SOCK_STREAM, 0);
    inet_aton("127.0.0.1", &structClientSockAddrIn.sin_addr);
    structClientSockAddrIn.sin_port = htons(1234);
    structClientSockAddrIn.sin_family = AF_INET;
    connect(sockFd, reinterpret_cast<sockaddr*>(&structClientSockAddrIn), sizeof(structClientSockAddrIn));
    Client obj(sockFd, pathFile);
    obj.ReadAndSendFileWithHashSum();
    obj.CloseInputFile();
    close(sockFd);
    return 0;
}
