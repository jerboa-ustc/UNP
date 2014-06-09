/*************************************************************************
	> File Name: Client-Server-Pipe.c
	> Author: jerboa
	> Mail: a.jerboa@gmail.com
	> Created Time: 2014年06月09日 星期一 15时21分12秒
 ************************************************************************/
/*
 *创建2个PIPE做双向传输管道，父进程（客户端）读取标准输入然后通过管道传递给子进程（服务器），子进程（服务器）从管道读取父进程（客户端）传来的命令打开指定文件，然后将文件内容逐行通过管道传递给父进程（客户端），最后父进程（客户端）将接受的数据输出到标准输出。
 */

#include <stdio.h>
#include "unpipc.h"

void client(int, int);
void server(int, int);

int main(int argc, char **argv)
{
	int pipe1[2], pipe2[2];
	pid_t childpid;

	//create pipe
	Pipe(pipe1);
	Pipe(pipe2);

	if((childpid = Fork()) == 0)
	{
		Close(pipe1[1]);//关闭写
		Close(pipe2[0]);//关闭读

		server(pipe1[0], pipe2[1]);
		exit(0);
	}

	Close(pipe1[0]);
	Close(pipe2[1]);

	client(pipe2[0], pipe1[1]);

	Waitpid(childpid, NULL, 0);
	exit(0);
}

void client(int readfd, int writefd)
{
	size_t len;
	ssize_t n;
	char buff[MAXLINE];

	//读取标准输入
	Fgets(buff, MAXLINE, stdin);
	len = strlen(buff);
	if(buff[len-1] == '\n')
		len--;
	//向管道写文件名or路径
	Write(writefd, buff, len);
	//从管道读取文件内容
	while((n = Read(readfd, buff, MAXLINE)) > 0)
		Write(STDOUT_FILENO, buff, n);
}

void server(int readfd, int writefd)
{
	int fd;
	ssize_t n;
	char buff[MAXLINE];

	//从管道读取路经or文件名
	if((n = Read(readfd, buff, MAXLINE)) == 0)
		err_quit("end of file while reading filename or dir");
	buff[n] = '\0';

	if((fd = open(buff, O_RDONLY)) < 0)	//打开出错，向管道写出错消息
	{
		snprintf(buff+n, sizeof(buff)-n, ": cant open, %s\n", strerror(errno));
		n= strlen(buff);
		Write(writefd, buff, n);
	}
	else
	{
		while((n = Read(fd, buff, MAXLINE)) > 0)
			Write(writefd, buff, n);
		Close(fd);
	}
}
