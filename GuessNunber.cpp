#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define FALSE  0
#define TRUE   1

void createRandomNumber(char num[]);
int isValidNumber(char num[]);
int isMatch(char target[], char num[]);

int main()
{
	char num[4], target[4];
	int i, j;
	createRandomNumber(target);
	for(i=0; i<4; i++) printf("%d", target[i]);
	do{
		int guessnumber;
		int hit = 0, blow = 0;
		printf("Please enter your guess number: ");
		scanf("%d", &guessnumber);
		num[3] = (guessnumber%10);
		num[2] = (guessnumber%100)/10;
		num[1] = (guessnumber%1000)/100;
		num[0] = (guessnumber%10000)/1000;
		if(isValidNumber(num)){
			if(isMatch(target, num)) printf("Good!\n");
			else{
				for(i=0; i<4; i++){
					if(target[i] == num[i]) hit++;	
					for(j=i+1; j<4; j++){
						if(target[i] == num[j]) blow++;
					}
				}
				printf("%d%d%d%d is %d hits and %d blows!\n", num[0], num[1], num[2], num[3], hit, blow);
			}
		}
	} while(!isMatch(target, num));
	system("PAUSE");
	return 0;
}

void createRandomNumber(char target[]){
	srand(time(NULL));
	target[0] = rand()%10; 
	do{
		(target[1] = rand()%10);
	} while (target[1] == target[0]);
	do{
		(target[2] = rand()%10);
	} while ((target[2] == target[1]) || (target[2] == target[0]));
	do{
		(target[3] = rand()%10);
	} while ((target[3] == target[2]) || (target[3] == target[1]) || (target[3] == target[0]));
}

int isValidNumber(char num[]){
	int i, j;
	for(i=0; i<4; i++){
		for(j=i+1; j<4; j++){
			if(num[i]==num[j]) return FALSE;
		}
	}
	return TRUE;
}

int isMatch(char target[], char num[]){
	if((target[0] == num[0]) && (target[1] == num[1]) && (target[2] == num[2]) && (target[3] == num[3])) return TRUE;
	else return FALSE;
}