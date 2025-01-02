#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  // write a byte to server
  char message[] = "hello to server\n";
  write(to_server, message, strlen(message) + 1);
  // read a byte from server 
  char buffer[BUFFER_SIZE];
  read(from_server, buffer, BUFFER_SIZE);

  printf("client received message: %s\n", buffer);
}
