#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    char buffer[128];
    FILE *fp = fopen("words.txt", "r");
    if (fp==NULL)
    {
        perror("File open failed");
        return 1;
    }

    int capacity = 10;
    int count = 0;

    char **words = malloc(capacity * sizeof(char*));

    while (fgets(buffer, sizeof(buffer), fp))
        {
            buffer[strcspn(buffer, "\n")] = '\0';

            if (strlen(buffer) == 5)
                {
                    if (count == capacity)
                    {
                        capacity++;
                        char **temp = realloc(words, capacity * sizeof(char*));
                        words = temp;
                    }

                 words[count] = malloc(strlen(buffer) + 1);
                 strcpy(words[count], buffer);
                 count++;
                }
        }
        fclose(fp);

        FILE *fp2 = fopen("words.txt", "w");

        for(int x=0;x<count;x++)
        {
            fprintf(fp2,"%s\n",words[x]);
            free(words[x]);
        }
        free(words);
        fclose(fp);
}
