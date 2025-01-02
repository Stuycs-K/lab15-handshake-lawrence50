#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  char buffer[BUFFER_SIZE];
  // read a byte from client
  read(from_client, buffer, BUFFER_SIZE);

  // write a byte to client 
  char message[BUFFER_SIZE] = "server received message: ";
  strcat(message, buffer);
  write(to_client, message, strlen(message) + 1);
}
