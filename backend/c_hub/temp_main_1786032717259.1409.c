#include <stdio.h>

#include <stdio.h>
__attribute__((constructor)) void auto_flush_init() { setvbuf(stdout, NULL, _IONBF, 0); setvbuf(stderr, NULL, _IONBF, 0); }
int main()
{
    int n, i;

    printf("Enter a number: ");
    scanf("%d", &n);

    for(i = 1; i <= n; i++)
    {
        printf("%d\n", i);
    }

    return 0;
}