#include "pipe_networking.h"


int to_server;
int from_server;


void sighandler(int signo) {
  close(to_server);
  close(from_server);
  printf("\nclient disconnected\n");
  exit(0);
}

// basic
// int main() {
//   signal(SIGINT, sighandler);
//   from_server = client_handshake(&to_server);
//   while (1) {
//     char buffer[BUFFER_SIZE];
//     int bytesRead = read(from_server, buffer, BUFFER_SIZE - 1);
//     if (bytesRead < 0) {
//       perror("client failed to read from server");
//       exit(1);
//     }
//     else if (bytesRead == 0) {
//       printf("server disconnected\n");
//       exit(0);
//     }
//     printf("client received %s\n", buffer);
//   }


//   close(to_server);
//   close(from_server);
// }

int main() {
  signal(SIGINT, sighandler);
  from_server = client_handshake(&to_server);
  while (1) {
    char message[BUFFER_SIZE] = "hello from ";
    char pid[20];
    sprintf(pid, "%d", getpid());
    strcat(message, pid);
    int bytesWritten = write(to_server, message, strlen(message) + 1);
    if (bytesWritten < 0) {
      perror("client fail to write to server\n");
      exit(1);
    }
    sleep(1);
  }
  close(to_server);
  close(from_server);
}
