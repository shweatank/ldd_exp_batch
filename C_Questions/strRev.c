//Reverse a string using pointer
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void rev(char *);

int main(){
	
	char *ss = malloc(10);
	scanf("%s",ss);
	rev(ss);
	printf("rev string : %s\n",ss);
}

void rev(char *str){
	int len = strlen(str);
	char *tmp = str;
	char *head = str;
	while(*tmp != '\0'){
		tmp++;
	}
	tmp--;
	printf("%c\n",*tmp);

	int i = 0; 
	while(i < len/2){
		char s = *str;
		
		*str = *tmp;
		printf("error\n");
		*tmp = s;
		str++;
		tmp--;
		i++;
	}
	//printf("%s\n",*head);
}
