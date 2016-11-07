#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "util.h"
#include "hashtable.h"

#define MEAN_LENGTH		700								/* The longest word's meaning has 660 characters. */
#define WORD_LENGTH		31								/* The longest word has 29 characters (include '#'). We need 1 more for '\n' and I use 1 more for reserve */

static unsigned long offset = 0;						/* Uesd as a cursor when read file */

void main(void) {
	unsigned char word[WORD_LENGTH];					/* Store the Word */
	unsigned char mean[MEAN_LENGTH];					/* Store the meaning of word */
	unsigned char ch = 0;								/* Variable used to store character read from file */
	int i = 0;											/* Used as a counter */
	int dict_size = 0;									/* Size of "dictionary.txt" */
	struct Word* wobj = NULL;
	FILE* dict_file = fopen("dictionary.txt","r");
	if (dict_file == NULL) {
	printf("File can not be opened!\n");				
	}
	/* Calculate the size of file*/
	fseek(dict_file, 0, SEEK_END);						/* Find the end of file */
	dict_size = ftell(dict_file);						/* Store the size */
	
	rewind(dict_file);									/* Back to the beginning of file */

	/* Scan the dictionary file */
	clrbuf(&word[0], WORD_LENGTH);						/* Clear the word before read */
	clrbuf(&mean[0], MEAN_LENGTH);						/* Clear the meaning */
	
	for (i = 0; i < dict_size; i++) {					/* Start scan */
		ch = fgetc(dict_file);							/* Read a character */
		
		if (ch == '#') {								/* Beginning of word */
			int num = 0;								/* Number of word in file */
			
			while (1) {					
				if (offset < 1) {						/* Prevent deleting the first word in file */
					offset = 1;
				}
			
				fseek(dict_file, offset, SEEK_SET);
				ch = fgetc(dict_file);
				offset += 1;							
				if (ch == '\n') {						/* End of word */
					break;
				}
				word[num] = ch;
				num += 1;
			}
			offset += 2;								/* Store the offset. +2 for Tab character '\t' and the first character of the meaning */ 
			wobj = (struct Word*)(malloc(sizeof(struct Word)));			/* Allocate memory for new word object */
			wobj->word = (unsigned char*)(calloc(strlen((const char*)word),sizeof(unsigned char)));
			wobj->offset = offset;
			strcpy((char *)wobj->word, (const char *)word);

			/* Insert word and its meaning */
			if (insert_word(&hashdict[hash_word(word)], wobj) == FALSE) {
				printf("Have a duplicate word in file\n");
			}

			clrbuf(&word[0], WORD_LENGTH);
		}
		offset += 1;
		i = offset;
	}
	printf("Initialized Dictionary!\n");
loop:
	/* Get word from user */
	printf("\nInput the word you wat to search: ");
	scanf("%[^\n]", word);
	getc(stdin);
	/* Show the result to user */
	if (search_word(&hashdict[hash_word(&word[0])], &word[0]) == NULL) {
		printf("\nCan not find \"%s\" in dictionary\n", word);
	}
	else { 
		if (get_meaning(&mean[0], search_word(&hashdict[hash_word(&word[0])], &word[0]), dict_file) == 1) {
			printf("\n%s", mean);
		}
		else {
			printf ("\nThe meaning of \"%s\" hasn't added!\n", word);
		}
	}
	/*Clear word and its meaning buffer */
	clrbuf(&word[0], WORD_LENGTH);
	clrbuf(&mean[0], MEAN_LENGTH);
	goto loop;

	fclose(dict_file);
	getchar();
}