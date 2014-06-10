/*************************************************************************
	> File Name: Client-FIFO.c
	> Author: jerboa
	> Mail: a.jerboa@gmail.com
	> Created Time: 2014年06月09日 星期一 23时53分11秒
 ************************************************************************/
/*
 * 运行时，要首先在一个shell启动Server-FIFO，然后在另一个shell启动Client-FIFO ./doc/Client-Server-Pipe
 */
#include <stdio.h>
#include "unpipc.h"

#define FIFO_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)
#define FIFO1 "/home/jerboa/fifo.1"
#define FIFO2 "/home/jerboa/fifo.2"

void client(int, int, char *);
int main(int argc, char **argv)
{
	int readfd, writefd;
	writefd = Open(FIFO1, O_WRONLY, 0);
	readfd = Open(FIFO2, O_RDONLY, 0);

	client(readfd, writefd, argv[1]);

	Close(readfd);
	Close(writefd);

	Unlink(FIFO1);
	Unlink(FIFO2);

	exit(0);
}

void client(int readfd, int writefd, char *p)
{
	size_t len;
	ssize_t n;
	char buff[MAXLINE];
//	Fgets(buff, MAXLINE, stdin);
	strcpy(buff, p);
	len = strlen(buff);
	if(buff[len-1] == '\n')
		len--;
	Write(writefd, buff, len);
	while((n = Read(readfd, buff, MAXLINE)) > 0)
		Write(STDOUT_FILENO, buff, n);
}
