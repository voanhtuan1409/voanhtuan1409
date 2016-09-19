#include <stdio.h>

int add(int a, int b){
    return (a+b);
}

int add_three(int (*p)(int a, int b), int c){
    return (p + c);
}

int max(int a, int b){
    if(a>b) return a;
    else return b;
}

int max_three(int (*p)(int a, int b), int c){
    if (p>c) return p;
    else return c;
}

int main()
{
    int a, b, c;
    printf("Enter 3 numbers: ");
    scanf("%d %d %d", &a, &b, &c);
    printf("Sum is %d\n", add_three(add(a, b), c));
    printf("Maximum number is %d", max_three(max(a, b), c));
    return 0;
}
