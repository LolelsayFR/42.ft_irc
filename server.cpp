#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

void server(int port)
{
	    // creating socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    bind(serverSocket, (struct sockaddr*)&serverAddress,
         sizeof(serverAddress));

    // listening to the assigned socket
    listen(serverSocket, 5);

    // accepting connection request
	while (true)
    {
        int clientSocket
            = accept(serverSocket, nullptr, nullptr);
		while (true)
		{
			char buffer[1024] = { 0 };
    		recv(clientSocket, buffer, sizeof(buffer), 0);
    		cout << "Message from client: " << buffer
              << endl;
		}
	}
    close(serverSocket);

}

int main(int argc, char**argv)
{
	int port;

	port = atoi(argv[1]);
	server(port);
    return 0;
}
