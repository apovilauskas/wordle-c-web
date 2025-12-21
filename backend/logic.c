#include <stdio.h>
#include <string.h>

int main() {
    char target[] = "hello";
    char input[6];
    int result[5] = {0};

    printf("Enter a 5-letter word: ");
    scanf("%5s", input);

    for (int i = 0; i < 5; i++)
    {
        if (input[i] == target[i])
        {
            result[i] = 2;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        if (result[i] == 0) { // Only check if not already a perfect match
            for (int j = 0; j < 5; j++)
            {
                if (i != j && input[i] == target[j] && result[j] != 2)
                {
                    result[i] = 1;
                    break;
                }
            }
        }
    }

    printf("Result:");
    for (int i = 0; i < 5; i++)
    {
        printf("%d", result[i]);
    }

    return 0;
}
