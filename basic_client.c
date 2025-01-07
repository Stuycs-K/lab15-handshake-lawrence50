#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  // write a byte to server
  //char message = 'w';
  //int bytesWritten = write(to_server, &message, sizeof(char));
  //if (bytesWritten < 0) {
  //  perror("client fail to write to server");
  //  exit(1);
  //}
  //printf("client wrote %d bytes to server\n", bytesWritten);

  // read a byte from server 
  char buffer[BUFFER_SIZE];
  int bytesRead = read(from_server, buffer, BUFFER_SIZE - 1);
  if (bytesRead < 0) {
    perror("client failed to read from server");
    exit(1);
  }
  printf("client read %d bytes from server\n", bytesRead);

  close(to_server);
  close(from_server);
}
