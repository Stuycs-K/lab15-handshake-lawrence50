#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  char buffer[BUFFER_SIZE];
  // read a byte from client
  int bytes = read(from_client, buffer, BUFFER_SIZE - 1);
  if (bytes < 0) {
    perror("fail to read from client");
    exit(1);
  }
  printf("server read %d bytes from '%s'\n", bytes, buffer);

  // write a byte to client 
  printf("length: %d\n", strlen(buffer));
  

  close(to_client);
  close(from_client);
}
