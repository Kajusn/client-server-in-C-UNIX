#include "client.h"

void start_client() {

  // Server parameters
  int serverSocket;
  struct sockaddr_in serverAddress;

  // Operation data
  char buffer[MAX_BUFFER_LEN];

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
  memset(&serverAddress, 0, sizeof(serverAddress));       // Clearing
  serverAddress.sin_family = AF_INET;                     // IPv4
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); // Local
  serverAddress.sin_port = htons(SERVER_PORT);            // Accepting port

  // Connecting to the server
  int connectStatus = connect(
    serverSocket,
    (struct sockaddr*) &serverAddress,
    sizeof(serverAddress));

  if (connectStatus < 0) {
    perror("Failed to connect to server");
    exit(1);
  }

  // Asking server to receive time
  strcpy(buffer, "time-request");
  send(
    serverSocket,   // Socket to send to
    buffer,         // Buffer which is sent
    strlen(buffer), // Amount of letters being sent
    0);             // Flags
  memset(buffer, 0, sizeof(buffer));

  // Getting the time and printing it out
  recv(
    serverSocket,   // Socket to receive from
    buffer,         // Buffer to which the response is written
    MAX_BUFFER_LEN, // The amount of letters read as response
    0);             // Flags
  printf("\n\nThe server time is: %s\n", buffer);

  // Finishing
  close(serverSocket);
}
