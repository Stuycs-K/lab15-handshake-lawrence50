#include "pipe_networking.h"


int to_client;
int from_client;
int connected = 0;

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
    printf("waiting for new client...\n");
    connected = 0;
  }
}


int main() {
  srand(time(NULL));
  signal(SIGINT, sighandler);
  signal(SIGPIPE, sighandler);
  from_client = server_handshake(&to_client);
  connected = 1;

  while (1) {
    if (!connected) {
      from_client = server_handshake(&to_client);
      connected = 1;
    }
    char buffer[BUFFER_SIZE];
    int rand_int = rand() % 101;
    sprintf(buffer, "%d", rand_int);
    int bytesWritten = write(to_client, buffer, strlen(buffer) + 1);
    if (bytesWritten < 0) {
      // perror("server failed to write to client");
      continue;
    }
    printf("server sent %d\n", rand_int);
    sleep(1);
  }
  close(to_client);
  close(from_client);
  
}




