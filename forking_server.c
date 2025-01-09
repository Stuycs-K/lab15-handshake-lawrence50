#include "pipe_networking.h"


int to_client;
int from_client;

void sighandler(int signo) {
  close(to_client);
  close(from_client);
  if (signo == SIGINT) {
    remove(WKP);
    printf("\nbye bye server\n");
    exit(0);
  }
  if (signo == SIGPIPE) {
    printf("client disconnected\n");
  }
}


int main() {
  srand(time(NULL));
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);
  while (1) {
    from_client = server_setup();

    int f = fork();
    if (f < 0) {
      perror("fork failed\n");
      exit(1);
    }
    if (f == 0) {
      // subserver
      server_handshake_half(&to_client, from_client);
      while (1) {
        char buffer[BUFFER_SIZE];
        int bytesRead = read(from_client, buffer, BUFFER_SIZE - 1);
        if (bytesRead < 0) {
          perror("subsever fail to read from client\n");
          exit(1);
        }
        else if (bytesRead == 0) {
          printf("a client disconnected\n");
          break;
        }
        if (strlen(buffer) > 0) {
          printf("received message: '%s'\n", buffer);
        }
      }
      close(to_client);
      close(from_client);
      exit(0);
    }
  }
  
  
}





