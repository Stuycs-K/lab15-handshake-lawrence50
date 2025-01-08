#include "pipe_networking.h"


int to_server;
int from_server;


void sighandler(int signo) {
  close(to_server);
  close(from_server);
  printf("\nclient disconnected\n");
  exit(0);
}


int main() {
  signal(SIGINT, sighandler);
  from_server = client_handshake(&to_server);
  while (1) {
    char buffer[BUFFER_SIZE];
    int bytesRead = read(from_server, buffer, BUFFER_SIZE - 1);
    if (bytesRead < 0) {
      perror("client failed to read from server");
      exit(1);
    }
    else if (bytesRead == 0) {
      printf("server disconnected\n");
      exit(0);
    }
    if (strlen(buffer) > 0) {
      printf("client received %s\n", buffer);
    }
  }


  close(to_server);
  close(from_server);
}

