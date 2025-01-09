#include "pipe_networking.h"
//UPSTREAM = to the server / from the client
//DOWNSTREAM = to the client / from the server
/*=========================
  server_setup

  creates the WKP and opens it, waiting for a  connection.
  removes the WKP once a connection has been made

  returns the file descriptor for the upstream pipe.
  =========================*/
int server_setup() {
  if (mkfifo(WKP, 0666) == -1) {
    perror("server create WKP fail");
    exit(1);
  }

  int from_client = open(WKP, O_RDONLY);
  if (from_client == -1) {
    perror("open WKP fail");
    exit(1);
  }

  remove(WKP);

  return from_client;
}

/*=========================
  server_handshake
  args: int * to_client

  Performs the server side pipe 3 way handshake.
  Sets *to_client to the file descriptor to the downstream pipe (Client's private pipe).

  returns the file descriptor for the upstream pipe (see server setup).
  =========================*/
int server_handshake(int *to_client) {
  int from_client = server_setup();
  server_handshake_half(to_client, from_client);
}


void server_handshake_half(int *to_client, int from_client) {
  char PP_name[HANDSHAKE_BUFFER_SIZE];
  if (read(from_client, PP_name, HANDSHAKE_BUFFER_SIZE) < 0) {
    perror("server couldn't read PP name from client");
    exit(1);
  }

  // send SYN ACK to client
  *to_client = open(PP_name, O_WRONLY);
  if (*to_client == -1) {
    perror("server open PP failed");
    exit(1);
  }

  char buffer[HANDSHAKE_BUFFER_SIZE];
  int rand_int = rand(); 
  sprintf(buffer, "%d", rand_int);
  write(*to_client, buffer, strlen(buffer) + 1);

  // read 2nd acknowledgement 
  if (read(from_client, buffer, HANDSHAKE_BUFFER_SIZE) < 0) {
    perror("server couldn't read final acknowledgement");
    exit(1);
  }
  // verify
  int ack;
  sscanf(buffer, "%d", &ack);
  if (ack != rand_int + 1) {
    perror("something went wrong");
    exit(1);
  }
}

/*=========================
  client_handshake
  args: int * to_server

  Performs the client side pipe 3 way handshake.
  Sets *to_server to the file descriptor for the upstream pipe.

  returns the file descriptor for the downstream pipe.
  =========================*/
int client_handshake(int *to_server) {

  // client creates private pipe with PID as name 
  char PP_name[HANDSHAKE_BUFFER_SIZE];
  sprintf(PP_name, "%d", getpid());
  if (mkfifo(PP_name, 0666) == -1) {
    perror("create PP fail");
    exit(1);
  }

  // open WKP (upstream) to write
  *to_server = open(WKP, O_WRONLY);
  if (*to_server == -1) {
    perror("client fail to open WKP");
    exit(1);
  }

  // send PP name (SYN) to server 
  write(*to_server, PP_name, strlen(PP_name) + 1);

  // open PP and block
  int from_server = open(PP_name, O_RDONLY);
  if (from_server == -1) {
    perror("client fail to open PP");
    exit(1);
  }

  remove(PP_name);


  // client receives SYN ACK from server 
  char buffer[HANDSHAKE_BUFFER_SIZE];
  if (read(from_server, buffer, HANDSHAKE_BUFFER_SIZE) < 0) {
    perror("client fail to read SYN ACK from server");
    exit(1);
  }


  // send final acknowledgement to server (same rand_int + 1)
  int rand_int;
  sscanf(buffer, "%d", &rand_int);
  sprintf(buffer, "%d", rand_int + 1);
  write(*to_server, buffer, strlen(buffer) + 1);

  return from_server;
}


/*=========================
  server_connect
  args: int from_client

  handles the subserver portion of the 3 way handshake

  returns the file descriptor for the downstream pipe.
  =========================*/
int server_connect(int from_client) {
  int to_client  = 0;
  return to_client;
}
