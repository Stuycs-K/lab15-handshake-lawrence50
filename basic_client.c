#include "pipe_networking.h"

int main() {

  int to_server;
  int from_server;

  from_server = client_handshake( &to_server );
  
  while (1) {
    char buffer[BUFFER_SIZE];
    int bytesRead = read(from_server, buffer, BUFFER_SIZE - 1);
    if (bytesRead < 0) {
      perror("client failed to read from server");
      exit(1);
    }
    printf("client received %s\n", buffer);
  }


  close(to_server);
  close(from_server);
}

