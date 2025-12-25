#include <cstring>  
#include <iostream>  

#ifdef _WIN32  
#include <winsock2.h>  
#include <ws2tcpip.h>  
#pragma comment(lib, "Ws2_32.lib")  
#define close closesocket  
#else  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <unistd.h>  
#endif  
#include <list>
#include <filesystem>
#include <regex>
#include <deque>
#include <fstream> 






namespace fs = std::filesystem;

void portandsend(const std::string& filePath, const std::string& port)
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return;
    }
#endif

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "socket fail\n";
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    inet_pton(AF_INET, port.c_str(), &serverAddress.sin_addr);

    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "connect fail\n";
        close(clientSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }

  
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "can't open zip to send\n";
        return;
    }

    
    file.seekg(0, std::ios::end);
    std::streamsize fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    uint64_t sizeNet = htonl((uint32_t)fileSize); 

    send(clientSocket, reinterpret_cast<char*>(&sizeNet), sizeof(sizeNet), 0);

 
    const size_t bufferSize = 4096;
    char buffer[bufferSize];
    while (file) {
        file.read(buffer, bufferSize);
        std::streamsize bytesRead = file.gcount();
        if (bytesRead > 0) {
            send(clientSocket, buffer, (int)bytesRead, 0);
        }
    }

    std::cout << "push sent \n";

    file.close();
    close(clientSocket);
#ifdef _WIN32
    WSACleanup();
#endif
}







std::list<std::string> dirsToCommit;


int push(std::string pos)  
{  

    pos = pos + "\\.COMMIT";
 ////set the list of files
   
	fs::path base = pos + ""; //the main dir to look for commits
    int highestnum = -1;
    fs::path highest;


    std::regex numFinder(R"((\d+))");

    for (auto const& dir_entry : fs::directory_iterator(base))
    {
        if (dir_entry.is_directory())//if it exists
        {
            std::string folderName = dir_entry.path().filename().string();


            std::smatch match;

            if (std::regex_search(folderName, match, numFinder))
            {
                int num = std::stoi(match[1].str());

                if (num > highestnum)
                {
                    highestnum = num;
                    highest = dir_entry.path();
                }
            }
        }
    }

    if (!highest.empty())
    {
        std::cout << "Latest folder" << highest << std::endl;
        dirsToCommit.push_front(highest.string());
    }
    else {
        std::cout << "Nothing found" << std::endl;
    }

    // 10.0.0.202 -- IP
    
    std::string output = highest.parent_path().string() + "\\" + std::to_string(highestnum);

    //super lazy way
    std::string cmd = "powershell Compress-Archive -Path \"" + highest.string() + "\" -DestinationPath \"" + output + "\" -Force";
    system(cmd.c_str());


    std::string answer;

    std::cout << "Input port ip to push too:" << std::endl;
    std::getline(std::cin >> std::ws, answer);std::cout << std::endl;
    std::cout << output << std::endl;
    portandsend(output, answer);



 






  return 0;  
}
