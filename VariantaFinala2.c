#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 100
#define MAX_LINE_LEN 1000

// Structura pentru a memora un cuvant si numarul de aparitii
typedef struct {
    char word[MAX_WORD_LEN];
    int count;
} WordCount;

// Functie pentru compararea a doua structuri WordCount (pentru qsort)
int compareWordCount(const void *a, const void *b) {
    return ((WordCount *)b)->count - ((WordCount *)a)->count;
}

int main(int argc, char *argv[]) {
    FILE *file;
    char filename[100];
    char buffer[MAX_LINE_LEN];
    WordCount wordCounts[MAX_LINE_LEN]; // vector pentru a memora cuvintele si numarul lor de aparitii
    int wordCount = 0;
    int isTextFile = 0; // variabila pentru a verifica daca fisierul este text

    // Citeste numele fisierului de la utilizator
    printf("Introduceti numele fisierului: ");
    scanf("%s", filename);

    // Deschide fisierul pentru citire
    file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Eroare la deschiderea fisierului");
        return 1;
    }

    // Verifică tipul fișierului (text sau binar)
    while (fgets(buffer, MAX_LINE_LEN, file) != NULL) {
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isprint(buffer[i]) && !isspace(buffer[i]) && buffer[i] != '\t' && buffer[i] != '\n' && buffer[i] != '\r') {
                isTextFile = 0;
                break;
            }
        }
        if (!isTextFile) {
            break;
        }
    }

    // Repositionează cursorul la începutul fișierului
    rewind(file);

    if (isTextFile) {
        // Analiză detaliată pentru fișierele text
        printf("Analiza detaliata a fisierului text:\n");
        
        int lineCount = 0, charCount = 0;

        // Numără cuvintele și caracterele și memorează frecvența cuvintelor
        while (fscanf(file, "%s", buffer) != EOF) {
            lineCount++;
            charCount += strlen(buffer) + 1; // +1 pentru spațiul sau newline-ul dintre cuvinte
            // Verifică dacă cuvantul există deja în vectorul de frecvențe
            int found = 0;
            for (int i = 0; i < wordCount; i++) {
                if (strcmp(wordCounts[i].word, buffer) == 0) {
                    wordCounts[i].count++;
                    found = 1;
                    break;
                }
            }
            // Dacă cuvântul nu există, îl adaugă la vectorul de frecvențe
            if (!found) {
                strcpy(wordCounts[wordCount].word, buffer);
                wordCounts[wordCount].count = 1;
                wordCount++;
            }
        }

        // Sortează cuvintele în ordine descrescătoare a frecvențelor
        qsort(wordCounts, wordCount, sizeof(WordCount), compareWordCount);

        // Afisează rezultatele pentru fișierele text
        printf("Numar de cuvinte: %d\n", wordCount);
        printf("Numar de linii: %d\n", lineCount);
        printf("Numar de caractere: %d\n", charCount);
        printf("Cuvinte frecvente:\n");
        for (int i = 0; i < wordCount && i < 10; i++) { // afișează primele 10 cuvinte frecvente
            printf("%s: %d\n", wordCounts[i].word, wordCounts[i].count);
        }
    } else {
        // Analiză detaliată pentru fișierele binare
        printf("Analiza detaliata a fisierului binar:\n");
        
        // Determină dimensiunea totală a fișierului în octeți
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        printf("Dimensiunea totala a fisierului: %ld octeti\n", fileSize);

        // Repositionează cursorul la începutul fișierului
        rewind(file);

        // Afișează primii 10 octeți din fișier sub forma de valori hexazecimale
        printf("Primii 10 octeti in format hexazecimal:\n");
        unsigned char buffer[10];
        fread(buffer, sizeof(unsigned char), 10, file);
        for (int i = 0; i < 10; i++) {
            printf("%02X ", buffer[i]);
        }
        printf("\n");
    }

    // Inchide fisierul
    fclose(file);

    return 0;
}