/*************************************************************************
	> File Name: Popen.c
	> Author: jerboa
	> Mail: a.jerboa@gmail.com
	> Created Time: 2014年06月09日 星期一 22时16分20秒
 ************************************************************************/

#include <stdio.h>
#include "unpipc.h"

int main(int argc, char **argv)
{
	size_t n;
	char buff[MAXLINE], command[MAXLINE];
	FILE *fp;

	//read from shell "cat or xxx"
	
	//read from stdin
	//Fgets(buff, MAXLINE, stdin);
	strcpy(buff, argv[1]);
	n = strlen(buff);
	if(buff[n-1] == '\n')
		n--;
	snprintf(command, sizeof(command), "cat %s", buff);
	fp = Popen(command, "r");

	//read from pipe
	while(Fgets(buff, MAXLINE, fp) != NULL)
		Fputs(buff, stdout);

	Pclose(fp);
	exit(0);
}
