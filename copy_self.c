#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

#define BUFSIZE 4096
#define FILE_PERM_MODE (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)


// 复制本身
int main(int argc, char *argv[]) {
	int infd; // fd 用于读取文件
	int outfd; // fd 用于写文件
	int n;  // 用于存储IO函数的值
	int buf[BUFSIZE]; // 用于复制文件的buffer
	char *self_real_path = realpath(argv[0],NULL); // 当前文件的绝对路径
	char *to_filename = argv[1]; // 要复制到的路径
	
	if ((infd = open(self_real_path,O_RDONLY)) == -1) {
		// 出错处理
		fprintf(stderr,"error: infd\n");
		exit(1);
	}
	// 如果文件不存在
	while ((outfd = open(to_filename,O_WRONLY | O_CREAT | O_TRUNC,FILE_PERM_MODE)) == -1) {
		// 出错处理
		if (chmod(to_filename,FILE_PERM_MODE) == -1) {
			fprintf(stderr,"error: chmod outfd\n");
                } else {
                        fprintf(stderr,"error: outfd\n");
			break;
		}
	}
	while ((n = read(infd, buf, BUFSIZE)) > 0) {
		if (write(outfd, buf,n) != n) {
			// 出错处理
			fprintf(stderr,"error: write\n");
		}
	}
	close(infd);
	close(outfd);
	return 0;
}
