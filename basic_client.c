#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  // write a byte to server
  char message[] = "hello to server";
  int bytes = write(to_server, message, sizeof(message));
  if (bytes < 0) {
    perror("client fail to write to server");
    exit(1);
  }
  printf("client wrote '%s' (%d bytes)\n", message, bytes);
  // read a byte from server 
  //char buffer[BUFFER_SIZE];
  //read(from_server, buffer, BUFFER_SIZE - 1);

  //printf("client received message: %s\n", buffer);

  close(to_server);
  close(from_server);
}
