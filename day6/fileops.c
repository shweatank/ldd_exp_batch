#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(){
	
	FILE *fptr = NULL;
	fptr = fopen("tmp.txt","w");
	fprintf(fptr, "%s","hello world");
	fclose(fptr);

	fptr = fopen("tmp.txt","r");
	
	char tmp_buf[100];
	while(fgets(tmp_buf, sizeof(tmp_buf), fptr) != NULL){
		printf("%s", tmp_buf);
	}

	fclose(fptr);

//	int wr_fd = 0;
//
//	wr_fd = open("tmp1.txt",O_RDWR | O_CREAT, 0666);
//	char buf[] = "devashreekatarkar";
//	write(wr_fd,buf,sizeof(buf),strlen(buf));
//	read(wr_fd, tmp_buf, (sizeof(tmp_buf) - 1));
//	printf("%s\n",tmp_buf);
//	//write(1,"deva",5,NULL);
//	close(wr_fd);
}
