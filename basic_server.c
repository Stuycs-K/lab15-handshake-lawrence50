#include "pipe_networking.h"

int main() {
  int to_client;
  int from_client;

  from_client = server_handshake( &to_client );
  char buffer[BUFFER_SIZE];
  // read a byte from client
  //int bytesRead = read(from_client, buffer, BUFFER_SIZE - 1);
  //if (bytesRead < 0) {
  //  perror("fail to read from client");
  //  exit(1);
  //}
  //printf("server read %d bytes from client\n", bytesRead);

  // write a byte to client 
  char message = 'b';
  int bytesWritten = write(to_client, &message, sizeof(char));
  if (bytesWritten < 0) {
    perror("server failed to write to client");
    exit(1);
  }
  printf("server wrote %d bytes to client\n", bytesWritten);

  close(to_client);
  close(from_client);
}
