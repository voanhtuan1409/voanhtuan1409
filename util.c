#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "util.h"
#include "hashtable.h"

struct Word* create_word (const char *w, unsigned long off) {
	struct Word * wobject = NULL;
	wobject = (struct Word*) (malloc(sizeof(struct Word)));								/* Allocate memory for word oject */
	wobject->word = (unsigned char*)(calloc(strlen(w),sizeof(unsigned char)));			/* Allocate memory to store the word */
	strcpy((char *)wobject->word, w);												/* Copy parameter "w" to "word" field */
	wobject->offset = off;																/* Copy parameter "off" to "offset" field */
	

	return wobject; 
}

int hash_word (unsigned char *w) {
	int hnum;
	if ((*w >= '0') && (*w <= '9')) {
		hnum = *w - '0';								/* Hash number = 0 -> 9 */
	} else if ((*w >= 'A') && (*w <= 'Z')) {
		hnum = *w - 'A' + 10;							/* Hash number = 10 -> 35 */
	} else if ((*w >= 'a') && (*w <= 'z')) {
		hnum = *w - 'a' + 36;							/* Hash number = 36 -> 61 */
	} else hnum = 62;								
	return hnum;
}

int insert_word (struct Word **hd, struct Word *wobject) {
	struct Word* tmp = NULL;
	int i = 0;
	struct Word *hdtmp = *hd;							
	/* Check if the word have existed in the hash table */
	while (hdtmp != NULL) {
		if (strcmp((const char*)(hdtmp->word), (const char*)(wobject->word)) == 0) {
			return FALSE;
		}
		hdtmp = hdtmp->next;
	}

	tmp = create_word((const char*)wobject->word, wobject->offset);
	tmp->next = *hd;
	*hd = tmp;

	return TRUE;
}

struct Word* search_word (struct Word **hd, unsigned char *word) {
	int i = 0;
	struct Word* hdtmp = *hd;
	
	while (hdtmp != NULL) {
		if (strcmp((const char*)(hdtmp->word), (const char*)word) == 0) {
			return (struct Word*) hdtmp;
		}
		hdtmp = hdtmp->next;
	}

	return NULL;
}

int get_meaning (unsigned char *buf, struct Word* wobject, FILE *dict_file) {
	int i = 0;
	char ch = 0;
	int isMean = FALSE;

	fseek(dict_file, wobject->offset, SEEK_SET);
	for (i = 0; TRUE; i++) {
		ch = fgetc(dict_file);
		if ((ch == '#') || (ch == EOF)) {
			break;
		}
		else {
			isMean = TRUE;
		}
		*(buf + i) = ch;
	}
	return isMean;
}
