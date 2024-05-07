#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_LINE_LEN 1000

typedef struct 
{
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

int compareWordCount(const void *a, const void *b) 
{
    return ((WordCount *)b)->count - ((WordCount *)a)->count;
}

int main(int argc, char *argv[]) 
{
    FILE *file;
    char filename[100];
    char buffer[MAX_LINE_LEN];
    WordCount wordCounts[MAX_LINE_LEN]; 
    int wordCount = 0;
    int isTextFile = 0; 
    printf("Introduceti numele fisierului: ");
    scanf("%s", filename);
    file = fopen(filename, "rb");
    if (file == NULL) 
    {
        perror("Eroare la deschiderea fisierului");
        return 1;
    }
    while (fgets(buffer, MAX_LINE_LEN, file) != NULL) 
    {
        for (int i = 0; buffer[i] != '\0'; i++) 
        {
            if (!isprint(buffer[i]) && !isspace(buffer[i]) && buffer[i] != '\t' && buffer[i] != '\n' && buffer[i] != '\r') 
            {
                isTextFile = 0;
                break;
            }
        }
        if (!isTextFile) 
        {
            break;
        }
    }
    rewind(file);
    if (isTextFile) 
    {
        printf("Analiza detaliata a fisierului text:\n");
        int lineCount = 0, charCount = 0;
        while (fscanf(file, "%s", buffer) != EOF) 
        {
            lineCount++;
            charCount += strlen(buffer) + 1; 
            int found = 0;
            for (int i = 0; i < wordCount; i++) 
            {
                if (strcmp(wordCounts[i].word, buffer) == 0) 
                {
                    wordCounts[i].count++;
                    found = 1;
                    break;
                }
            }
            if (!found) 
            {
                strcpy(wordCounts[wordCount].word, buffer);
                wordCounts[wordCount].count = 1;
                wordCount++;
            }
        }
        qsort(wordCounts, wordCount, sizeof(WordCount), compareWordCount);
        printf("Numar de cuvinte: %d\n", wordCount);
        printf("Numar de linii: %d\n", lineCount);
        printf("Numar de caractere: %d\n", charCount);
        printf("Cuvinte frecvente:\n");
        for (int i = 0; i < wordCount && i < 10; i++) 
        {
            printf("%s: %d\n", wordCounts[i].word, wordCounts[i].count);
        }
    } 
    else 
    {
        printf("Analiza detaliata a fisierului binar:\n");
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        printf("Dimensiunea totala a fisierului: %ld octeti\n", fileSize);
        rewind(file);
        printf("Primii 10 octeti in format hexazecimal:\n");
        unsigned char buffer[10];
        fread(buffer, sizeof(unsigned char), 10, file);
        for (int i = 0; i < 10; i++) 
        {
            printf("%02X ", buffer[i]);
        }
        printf("\n");
    }
    fclose(file);
    return 0;
}
