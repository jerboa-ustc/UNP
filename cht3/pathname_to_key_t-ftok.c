/*************************************************************************
	> File Name: pathname_to_key_t-ftok.c
	> Author: jerboa
	> Mail: a.jerboa@gmail.com
	> Created Time: 2014年06月08日 星期日 10时17分02秒
 ************************************************************************/

#include <stdio.h>
#include "unp.h"

int main(int argc, char **argv)
{
	struct stat stat;
	if(argc != 2)
		err_quit("usage: ftok <pathname>");
	Stat(argv[1], &stat);
	printf("st_dev: %lx, st_ino: %lx, key%x\n", (u_long)stat.st_dev, (u_long)stat.st_ino, Ftok(argv[1], 1));

	exit(0);
}
