#include "server.h"

void start_server() {

    fd_set active_fd_set, read_fd_set;
    // Server variables
    int serverSocket;
    struct sockaddr_in serverAddress;

    // Client that connects variables
    int clientSocket;
    struct sockaddr_in clientAddress;
    socklen_t clientAddressLength;

    // Operation data
    char buffer[MAX_BUFFER_LEN];
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

    if (bindStatus < 0) {
        perror("Failed to bind server's address to server's socket");
        exit(2);
    }

    // Starting to listen to clients to connect
    int startListeningStatus = listen(serverSocket, 1);

    if (startListeningStatus < 0) {
        perror("Failed to start listening to clients");
        exit(3);
    }

    FD_ZERO (&active_fd_set);
    FD_SET (serverSocket, &active_fd_set);
    // Entering the app lifecycle loop
    while (1) {

        read_fd_set = active_fd_set;
        if (select (FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
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
                new = accept (serverSocket,
                              (struct sockaddr *) &clientAddress,
                              &clientAddressLength);
                if (new < 0){
                    perror ("accept");
                    exit (EXIT_FAILURE);
                }
                fprintf (stderr,
                    "Server: connect from host %s, port %hd.\n",
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

                // Process the received message
                char *timeString = calloc(MAX_BUFFER_LEN, sizeof(char));
                get_time(timeString, MAX_BUFFER_LEN);

                // Sending back the same what was sent
                serverResponseLength = send(
                    clientSocket,
                    timeString,
                    strlen(timeString),
                    0);

                // Disconnecting from the clientSocket
                close(clientSocket);
                FD_CLR (clientSocket, &active_fd_set);

                // Printing status
                printf(
                    "%s - got: %d - sent: %d\n",
                    inet_ntoa(clientAddress.sin_addr),
                    serverRequestLength,
                    serverResponseLength);
              }
          }
    }
}
