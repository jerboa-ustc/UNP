/*************************************************************************
	> File Name: Client-Server-FIFO.c
	> Author: jerboa
	> Mail: a.jerboa@gmail.com
	> Created Time: 2014年06月09日 星期一 23时32分14秒
 ************************************************************************/

#include <stdio.h>
#include "unpipc.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define FIFO_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define FIFO1 "/home/jerboa/fifo.1"
#define FIFO2 "/home/jerboa/fifo.2"

void server(int, int);

int main(int argc, char **argv)
{
	int readfd, writefd;
	pid_t childpid;

	if((mkfifo(FIFO1, FIFO_MODE) < 0) && (errno != EEXIST))
	{
		err_sys("cant create %s", FIFO1);
	}
	if((mkfifo(FIFO2, FIFO_MODE) < 0) && (errno != EEXIST))
	{
		unlink(FIFO1);
		err_sys("cant create %s", FIFO2);
	}

	readfd = Open(FIFO1, O_RDONLY, 0);
	writefd = Open(FIFO2, O_WRONLY, 0);

	server(readfd, writefd);
	exit(0);
}

void server(int readfd, int writefd)
{
	int fd;
	ssize_t n;
	char buff[MAXLINE];
	if((n = Read(readfd, buff, MAXLINE)) == 0)
		err_quit("end of file while reading pathname");
	buff[n] = '\0';
	if((fd = open(buff, O_RDONLY, 0)) < 0)
	{
		snprintf(buff+n, sizeof(buff)-n, ": cant open, %s\n", strerror(errno));
		n = strlen(buff);
		Write(writefd, buff, n);
	}
	else
	{
		while((n = Read(fd, buff, MAXLINE)) > 0)
			Write(writefd, buff, n);
		Close(fd);
	}
}
