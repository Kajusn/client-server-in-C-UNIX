#include "server.h"

void start_server() {

    fd_set active_fd_set, read_fd_set;

    int serverSocket;
    struct sockaddr_in serverAddress;

    int clientSocket;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength;

    char buffer[1024];
    int serverRequestLength;
    int serverResponseLength;

    // Initializing server socket
    serverSocket = socket(
        AF_INET,     // IPv4 internet protocols
        SOCK_STREAM, // Sequenced, reliable, two-way connection-based byte streams
        0);          // Default protocol

    if (serverSocket < 0) {
        perror("Failed to initialize server socket");
        exit(1);
    }

    // Initializing server address
    memset(&serverAddress, 0, sizeof(serverAddress));  // Clearing
    serverAddress.sin_family = AF_INET;                // IPv4
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // Any address accepted
    serverAddress.sin_port = htons(SERVER_PORT);       // Accepting port

    // Binding server's address to server's socket
    int bindStatus = bind(
        serverSocket,
        (struct sockaddr *) &serverAddress,
        sizeof(serverAddress));

    if (bindStatus < 0) 
    {
        perror("Failed to bind server's address to server's socket");
        exit(2);
    }

    // Starting to listen to clients to connect
    int startListeningStatus = listen(serverSocket, 1);

    if (startListeningStatus < 0) 
    {
        perror("Error starting to listen");
        exit(3);
    }

    FD_ZERO (&active_fd_set);
    FD_SET (serverSocket, &active_fd_set);

    while (1) {

        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) 
        {
            perror ("select");
            exit (EXIT_FAILURE);
        }

        // Refreshing buffer and client's address
        memset(&buffer, 0, sizeof(buffer));
        memset(&clientAddress, 0, sizeof(clientAddress));

        for (int i = 0; i < FD_SETSIZE; ++i)
        if (FD_ISSET (i, &read_fd_set))
        {
            if (i == serverSocket)
            {
                /* Connection request on original socket. */
                int new;
                clientAddressLength = sizeof(clientAddress);
                new = accept (serverSocket,
                              (struct sockaddr *) &clientAddress,
                              &clientAddressLength);
                if (new < 0){
                    perror ("Accept error");
                    exit (EXIT_FAILURE);
                }
                fprintf (stderr,
                    "Server: connection accepted from host %s, port %d.\n",
                    inet_ntoa (clientAddress.sin_addr),
                    ntohs (clientAddress.sin_port));
                FD_SET (new, &active_fd_set);
            }
            else
            {
                /* Data arriving on an already-connected socket. */
                clientSocket = i;
                // Once connection has been received, reading input
                serverRequestLength = recv(clientSocket, buffer, sizeof(buffer), 0);

                if (!strncmp(buffer, "disconnect", 10)) 
                {
                    // Disconnecting from the clientSocket
                    close(clientSocket);
                    FD_CLR (clientSocket, &active_fd_set);
                    // Print status
                    printf(
                        "got: %d - sent: %d\n",
                        serverRequestLength,
                        serverResponseLength);

                } else if (!strncmp(buffer, "time-request", 12)) 
                {
                    // Process the received message
                    char *timeString = calloc(MAX_BUFFER_LEN, sizeof(char));
                    get_time(timeString, MAX_BUFFER_LEN);

                    serverResponseLength = send(clientSocket, timeString, strlen(timeString), 0);

                    // Print status
                    printf(
                        "got: %d - sent: %d\n",
                        serverRequestLength,
                        serverResponseLength);
                }
            }
        }
    }
}
