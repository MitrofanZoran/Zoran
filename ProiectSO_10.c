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

void createDirectory(const char *path) 
{
    char newDirName[100];
    printf("Introduceti numele noului director: ");
    scanf("%s", newDirName);
    char newDirPath[200];
    sprintf(newDirPath, "%s/%s", path, newDirName);
    if (mkdir(newDirPath, 0777) == 0)
        printf("Directorul \"%s\" a fost creat cu succes.\n", newDirPath);
    else
        perror("Eroare la crearea directorului");
}

void deleteFileOrDirectory(const char *path) 
{
    if (remove(path) == 0)
        printf("%s a fost sters cu succes.\n", path);
    else
        perror("Eroare la stergerea fisierului/directorului");
}

void copyFile(const char *sourcePath, const char *destinationPath) 
{
    FILE *sourceFile = fopen(sourcePath, "rb");
    FILE *destinationFile = fopen(destinationPath, "wb");
    if (sourceFile == NULL || destinationFile == NULL) 
    {
        perror("Eroare la deschiderea fisierelor");
        return;
    }
    char buffer[1024];
    size_t bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), sourceFile)) > 0) 
    {
        fwrite(buffer, 1, bytesRead, destinationFile);
    }
    fclose(sourceFile);
    fclose(destinationFile);
    printf("Fisierul \"%s\" a fost copiat cu succes in \"%s\".\n", sourcePath, destinationPath);
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
        printf("3. Creare director nou\n");
        printf("4. Stergere fisier/director\n");
        printf("5. Copiere fisier\n");
        printf("6. Iesire\n");
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
                createDirectory(path);
                break;
            case 4:
                deleteFileOrDirectory(path);
                break;
            case 5:
                {
                    char sourceFilePath[100], destinationFilePath[100];
                    printf("Introduceti calea fisierului sursa: ");
                    scanf("%s", sourceFilePath);
                    printf("Introduceti calea fisierului destinatie: ");
                    scanf("%s", destinationFilePath);
                    copyFile(sourceFilePath, destinationFilePath);
                }
                break;
            case 6:
                printf("Iesire din program.\n");
                break;
            default:
                printf("Optiune invalida. Va rugam sa introduceti o optiune valida.\n");
                break;
        }
    } while (choice != 6);
    return 0;
}
