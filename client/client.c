#include "client.h"

void start_client() {

  int clientSocket;
  struct sockaddr_in serverAddress;

  char buffer[1024];

  clientSocket = socket(
      AF_INET,     // IPv4 internet protocol
      SOCK_STREAM, // Sequenced, reliable, two-way connection-based byte streams
      0);          // Default protocol

  if (clientSocket < 0) {
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
      clientSocket,
      (struct sockaddr*) &serverAddress,
      sizeof(serverAddress));

  if (connectStatus < 0) {
      perror("Failed to connect to server");
      exit(1);
  }

  strcpy(buffer, "time-request");
  send(
      clientSocket,   // Socket to send to
      buffer,         // Buffer which is sent
      strlen(buffer), // Amount of bytes being sent
      0);             // Flags
  memset(buffer, 0, sizeof(buffer));

  recv(
      clientSocket,   // Socket to receive from
      buffer,         // Buffer to which the response is written
      MAX_BUFFER_LEN, // The amount of bytes read as response
      0);             // Flags

  printf("\n\nThe server time is: %s\n", buffer);
  memset(buffer, 0, sizeof(buffer));

  sleep(7);

  strcpy(buffer, "disconnect");
  send(
      clientSocket,   // Socket to send to
      buffer,         // Buffer which is sent
      strlen(buffer), // Amount of bytes being sent
      0);             // Flags
  memset(buffer, 0, sizeof(buffer));

  close(clientSocket);
}
