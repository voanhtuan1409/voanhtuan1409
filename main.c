#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct leaf{
    char data[16];
    struct leaf *next;
};
struct leaf *removeElement2(struct leaf *);
struct leaf *removeLeaf(struct leaf *);
struct leaf *insertLeafAfterElement1(struct leaf *);
struct leaf *insertLeaf(struct leaf *); //function pointer
struct leaf *addLeaf(char *, struct leaf *); //function pointer
void printLeaves(struct leaf *p);
void freeLeaves(struct leaf *p);
int length(struct leaf *);

int main()
{
    struct leaf *top;
    char *fruits[3] = {"apple", "organge", "banana"};
    top = NULL;

    top = addLeaf(fruits[0], top);
    top = addLeaf(fruits[1], top);
    top = addLeaf(fruits[2], top);

    top = insertLeafAfterElement1(top);

    top = insertLeaf(top);
    top = insertLeaf(top);
    top = insertLeaf(top);
    top = insertLeaf(top);

    //top = removeElement2(top);

    top = removeLeaf(top);
    top = removeLeaf(top);
    top = removeLeaf(top);
    top = removeLeaf(top);

    printLeaves(top);
    printf("\nNumber of leaves now = %d", length(top));
    freeLeaves(top);
    return 0;
}

int length(struct leaf *top){
    int i=0;
    struct leaf *temp = top;
    while(temp != NULL){
        temp = temp->next;
        i++;
    }
    return i;
}

struct leaf *removeElement2(struct leaf *top){
    struct leaf *tempLeaf = top -> next;
    top -> next = top -> next -> next;
    free(tempLeaf);
    return top;
}

struct leaf *removeLeaf(struct leaf *top){
    struct leaf *tempLeaf, *tempLeaf1;
    int number, i;
    do{
        printf("Please insert number from 1 to %d: ", length(top));
        scanf("%d", &number);
    }
    while(1 > number | number > length(top));
    tempLeaf = top;
    if(number == length(top)){
        top = top -> next;
        free(tempLeaf);
    }
    else if(number == length(top)-1){
        tempLeaf = top -> next;
        top -> next = top -> next -> next;
        free(tempLeaf);
    }
    else{
        for(i=number; i<length(top)-1; i++){
            tempLeaf = tempLeaf -> next;
        }
        tempLeaf1 = tempLeaf->next;
        tempLeaf -> next = tempLeaf -> next -> next;
        free(tempLeaf1);
    }
    return top;
}

struct leaf *insertLeafAfterElement1(struct leaf *top){
    struct leaf *newLeaf;
    newLeaf = (struct leaf *)malloc(sizeof(struct leaf));
    char data[16] = "grage";
    strcpy(newLeaf->data, data);
    top -> next -> next -> next = newLeaf;
    newLeaf -> next = NULL;
    return top;
}

struct leaf *insertLeaf(struct leaf *top){
    struct leaf *newLeaf;
    struct leaf *tempLeaf;
    int number, i;
    char newdata[16];
    do{
        printf("Please insert number from 0 to %d: ", length(top));
        scanf("%d", &number);
    }
    while(0 > number | number > length(top));
    printf("Fruit name: ");
    scanf("%s", &newdata);
    newLeaf = (struct leaf *)malloc(sizeof(struct leaf));
    strcpy(newLeaf -> data, newdata);
    if(number == 0){
        top = addLeaf(newdata, top);
    }
    else{
        tempLeaf = top;
        for(i=number; i<length(top); i++){
            tempLeaf = tempLeaf -> next;
        }
        newLeaf -> next = tempLeaf -> next;
        tempLeaf -> next = newLeaf;
    }
    return top;
}

struct leaf *addLeaf(char *data, struct leaf *top){
    struct leaf *pL;
    pL = (struct leaf *)malloc(sizeof(struct leaf));
    strcpy(pL -> data, data);
    pL -> next = top;
    top = pL;
    return top;
}

void printLeaves(struct leaf *pL){
    while(pL != NULL){
        printf("%s ", pL->data);
        pL = pL -> next;
    }
}

void freeLeaves(struct leaf *pL){
    struct leaf *pLnext;
    while(pL != NULL){
        pLnext = pL -> next;
        free(pLnext);
        pL = pLnext;
    }
}
