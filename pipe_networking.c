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
  printf("server making pipe\n");
  if (mkfifo(WKP, 0666) == -1) {
    perror("server create WKP fail");
    exit(1);
  }

  printf("server opening WKP (blocking)\n");
  int from_client = open(WKP, O_RDONLY);
  if (from_client == -1) {
    perror("open WKP fail")
    exit(1);
  }

  printf("server removing WKP\n");
  remove(WKP);

  printf("-- end server_setup() --\n");
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
  // server gets message from client (PP name)
  printf("server reading SYN (pid)\n");
  char buffer[HANDSHAKE_BUFFER_SIZE];
  if (read(from_client, buffer, HANDSHAKE_BUFFER_SIZE) < 0) {
    perror("server couldn't read PP name from client");
    exit(1);
  }


  // send SYN ACK to client
  printf("server opening PP (unblock client)");
  *to_client = open(buffer, O_WRONLY);
  if (*to_client == -1) {
    perror("server open PP failed");
    exit(1);
  }

  printf("server sending SYN_ACK\n");
  int rand_int = rand(); 
  sprintf(buffer, "%d", rand_int);
  write(*to_client, buffer, strlen(buffer) + 1);

  // read 2nd acknowledgement 
  printf("server reading final ACK\n");
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
  printf("server received ACK, handshake complete\n");
  
  printf("-- end server_handshake() --\n");
  return from_client;
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
  printf("client making PP\n");
  char PP_name[HANDSHAKE_BUFFER_SIZE];
  sprintf(PP_name, "%d", getpid());
  if (mkfifo(PP_name, 0666) == -1) {
    perror("create PP fail");
    exit(1);
  }

  // open WKP (upstream) to write
  printf("client opening WKP (unblock server)\n");
  to_server = open(WKP, O_WRONLY);
  if (to_server == -1) {
    perror("client fail to open WKP");
    exit(1);
  }

  // send PP name (SYN) to server 
  printf("client writing PP to WKP\n");
  write(to_server, PP_name, strlen(PP_name) + 1);

  // open PP and block
  printf("client opening PP (blocks)\n");
  int from_server = open(PP_name, O_RDONLY);
  if (from_server == -1) {
    perror("client fail to open PP");
    exit(1);
  }

  printf("client deleting PP\n");
  remove(PP_name);


  // client receives SYN ACK from server 
  printf("client reading SYN_ACK\n");
  char buffer[HANDSHAKE_BUFFER_SIZE];
  if (read(from_server, buffer, HANDSHAKE_BUFFER_SIZE) < 0) {
    perror("client fail to read SYN ACK from server");
    exit(1);
  }


  // send final acknowledgement to server (same rand_int + 1)
  printf("client sending ACK\n");
  int rand_int;
  sscanf(buffer, "%d", &rand_int);
  sprintf(buffer, "%d", rand_int + 1);
  write(to_server, buffer, strlen(buffer) + 1);

  printf("-- end client_handshake() --\n");
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
