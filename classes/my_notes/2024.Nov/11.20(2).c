#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[]){
	FILE *pSourceFile, *pDestFile;
    int c, numOfBytes;//注意不是char类型，因为getc函数返回的是int类型，char不能表示-1
    printf("mycopy command. version 1.0 \n");
    printf("Developed by: XXXX\n");
    printf("All rights reserved.\n");
    printf("Anyone who uses this program without the author's permission will be punished.\n");
	if(argc != 3){
		fprintf(stderr, "Copy failed. Illegal input.\n");
		exit(1);
	}
	pSourceFile = fopen(argv[1], "rb");
	if(pSourceFile == NULL){
		fprintf(stdout, "Cannot open file %s.\n",argv[1]);//stdout是显示器
		exit(1);
	}
	pDestFile = fopen(argv[2], "wb");
	if(pDestFile == NULL){
		fprintf(stderr, "Cannot open file %s.\n",argv[2]);
		exit(1); 
	}
    numOfBytes = 0;
    while((c = getc(pSourceFile)) != EOF){
        putc(c, pDestFile);
        numOfBytes++;
    }
    fclose(pSourceFile);
    fclose(pDestFile);
    fprintf(stdout," %d bytes copied successfully.\n", numOfBytes);
    exit(0);
}