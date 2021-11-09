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
	FILE *in;
	FILE *out;
	int outfd;
	char *buf[BUFSIZE];
	size_t n;
	
	char *self_real_path = realpath(argv[0],NULL); // 当前文件的绝对路径
	char *to_filename = argv[1]; // 要复制到文件

	if (argc < 2) {
		fprintf(stderr,"No output file name\n");
		exit(1);
	}

	// 打开文件，如果文件不存在则创建文件
	for (;(in = fopen(self_real_path,"r")) == NULL;) {
		perror("fopen() failed: ");
		exit(1);
	}

	// 打开文件，如果文件不存在则创建文件
	for (;(outfd = open(to_filename,O_WRONLY | O_CREAT | O_TRUNC, FILE_PERM_MODE)) == -1;) {
		// 尝试解决问题
		if (chmod(to_filename,FILE_PERM_MODE) == -1) {
			perror("chmod() failed: ");
			exit(1);
		}
	}
	if ((out = fdopen(outfd,"w")) == NULL) {
		perror("fdopen() failed: ");
	}

	for (;(n = fread(buf,1,BUFSIZE,in)) > 0;) {
		if (fwrite(buf,1,n,out) != n) {
			perror("fwrite() failed: ");
		}
	}
	fclose(in);
	fclose(out);
	close(outfd);
	return 0;
}
