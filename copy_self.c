#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define _FILE_OFFSET_BITS 64
#define BUFSIZE 4096
#define FILE_PERM_MODE (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)

// 复制本身
int main(int argc, char *argv[]) {
	int infd; // fd 用于读取文件
	int outfd; // fd 用于写文件
	long int n;  // 读入的字节
	int buf[BUFSIZE]; // 用于复制文件的buffer
	char *self_real_path = realpath(argv[0],NULL); // 当前文件的绝对路径
	char *to_filename = argv[1]; // 要复制到的路径
	
	if ((infd = open(self_real_path,O_RDONLY)) == -1) {
		// 出错处理
		perror("open() failed: ");
		exit(1);
	}

	// 打开文件，如果文件不存在则创建文件
	for (;(outfd = open(to_filename,O_WRONLY | O_CREAT | O_TRUNC, FILE_PERM_MODE)) == -1;) {
		// 尝试解决问题
		if ( errno == EACCES ) { // 解决权限造成的问题
			if (chmod(to_filename,FILE_PERM_MODE) == -1) {
				perror("chmod() failed: ");
				exit(1);
			}
		}
	}
	// 如果仍未解决
	if (outfd == -1) {
		perror("open() failed: ");
		exit(1);
	}

	// 开始拷贝
	while ((n = read(infd, buf, BUFSIZE)) > 0) {
		if (write(outfd, buf,n) != n) {
			// 出错处理
			perror("write() failed: ");
		}
	}
	close(infd);
	close(outfd);
	return 0;
}
