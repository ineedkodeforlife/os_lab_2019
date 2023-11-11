#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// #define SERV_PORT 10050
// #define BUFSIZE 100
#define SADDR struct sockaddr

int main(int argc, char *argv[]) {
  const size_t kSize = sizeof(struct sockaddr_in);

  int lfd, cfd;
  int nread;
  int buf_size = atoi(argv[1]);
  if (buf_size <= 0)
  {
      printf("Size of buffer must be positive number\n");
      exit(1);
  }
  char buf[buf_size];
  
  int serv_port = atoi(argv[2]);
  if (serv_port <= 1024)
  {
      printf("Port of the server must be > 1024\n");
      exit(1);
  }
  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr;

  if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(1);
  }

  memset(&servaddr, 0, kSize);
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(serv_port);

  if (bind(lfd, (SADDR *)&servaddr, kSize) < 0) {
    perror("bind");
    exit(1);
  }

  if (listen(lfd, 5) < 0) {
    perror("listen");
    exit(1);
  }

  while (1) {
    unsigned int clilen = kSize;

    if ((cfd = accept(lfd, (SADDR *)&cliaddr, &clilen)) < 0) {
      perror("accept");
      exit(1);
    }
    printf("connection established\n");

    while ((nread = read(cfd, buf, buf_size)) > 0) {
      write(1, &buf, nread);
    }

    if (nread == -1) {
      perror("read");
      exit(1);
    }
    close(cfd);
  }
}
