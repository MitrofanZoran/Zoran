#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

void listFilesAndDirectories(const char *path) 
{
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) 
    {
        perror("Eroare la deschiderea directorului");
        return;
    }
    printf("Fisiere si directoare in directorul %s:\n", path);
    while ((entry = readdir(dir)) != NULL) 
    {
        printf("%s\n", entry->d_name);
    }
    closedir(dir);
}

void analyzeFiles(const char *path) 
{
    struct dirent *entry;
    DIR *dir = opendir(path);
    if (dir == NULL) 
    {
        perror("Eroare la deschiderea directorului");
        return;
    }
    printf("\nAnaliza detaliata a fisierelor:\n");
    while ((entry = readdir(dir)) != NULL) 
    {
        if (entry->d_type == DT_REG) 
        {
            char filePath[100];
            strcpy(filePath, path);
            strcat(filePath, "/");
            strcat(filePath, entry->d_name);
            FILE *file = fopen(filePath, "rb");
            if (file != NULL) 
            {
                printf("\nFisier: %s\n", entry->d_name);
                if (strcmp(strrchr(entry->d_name, '.'), ".txt") == 0) 
                { 
                    int words = 0, lines = 0, characters = 0;
                    char ch;
                    while ((ch = fgetc(file)) != EOF) 
                    {
                        characters++;
                        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') 
                        {
                            words++;
                        }
                        if (ch == '\n') 
                        {
                            lines++;
                        }
                    }
                    printf("Cuvinte: %d\n", words);
                    printf("Linii: %d\n", lines);
                    printf("Caractere: %d\n", characters);
                } 
                else 
                { 
                    fseek(file, 0L, SEEK_END);
                    long fileSize = ftell(file);
                    printf("Dimensiune: %ld octeti\n", fileSize);
                    printf("Primii 10 octeti in format hexazecimal:\n");
                    rewind(file);
                    unsigned char buffer[10];
                    fread(buffer, sizeof(unsigned char), 10, file);
                    for (int i = 0; i < 10; i++) 
                    {
                        printf("%02X ", buffer[i]);
                    }
                    printf("\n");
                }
                fclose(file);
            } 
            else 
            {
                perror("Eroare la deschiderea fisierului");
            }
        }
    }
    closedir(dir);
}

int main() 
{
    char path[100];
    int choice;
    printf("Introduceti calea directorului: ");
    scanf("%s", path);
    do 
    {
        printf("\nMeniu:\n");
        printf("1. Listare fisiere/directoare\n");
        printf("2. Analiza detaliata a fisierelor\n");
        printf("3. Iesire\n");
        printf("Introduceti optiunea dvs.: ");
        scanf("%d", &choice);
        switch (choice) 
        {
            case 1:
                listFilesAndDirectories(path);
                break;
            case 2:
                analyzeFiles(path);
                break;
            case 3:
                printf("Iesire din program.\n");
                break;
            default:
                printf("Optiune invalida. Va rugam sa introduceti o optiune valida.\n");
                break;
        }
    } while (choice != 3);
    return 0;
}
