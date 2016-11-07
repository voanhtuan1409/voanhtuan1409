#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "util.h"
#include "hashtable.h"
void clrbuf(unsigned char* buf, int len) {
	int i = 0;

	for (i = 0; i < len; i++) {
		*(buf + i) = '\0';
	}
}
