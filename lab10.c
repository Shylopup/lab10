#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ALPHABET_SIZE 26

// Trie structure
struct Trie
{
    struct Trie *children[ALPHABET_SIZE];
    int isEndOfWord;
    int count;
};

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *node = (struct Trie *)malloc(sizeof(struct Trie));
    if (node)
    {
        node->isEndOfWord = 0;
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    struct Trie *current = pTrie;
    for (int i = 0; word[i] != '\0'; i++)
    {
        char c = tolower(word[i]); // Convert character to lowercase
        int index = c - 'a';
        if (index < 0 || index >= ALPHABET_SIZE)
        {
            continue; // Skip non-alphabet characters
        }
        if (!current->children[index])
        {
            current->children[index] = createTrie();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
    current->count++; // Increment the count even for duplicate words
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    struct Trie *current = pTrie;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
        {
            return 0; // Word not found
        }
        current = current->children[index];
    }
    if (current != NULL && current->isEndOfWord)
    {
        return current->count;
    }
    else
    {
        return 0; // Word not found
    }
}

// Deallocate the trie structure
void deallocateTrie(struct Trie *pTrie)
{
    if (pTrie)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            deallocateTrie(pTrie->children[i]);
        }
        free(pTrie);
    }
}

// This function will return the number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Failed to open file.\n");
        exit(1);
    }
    int numWords = 0;
    char word[100]; // Assuming maximum word length is 100
    while (fscanf(file, "%s", word) != EOF)
    {
        while (fscanf(file, "%s", word) != EOF)
        {
            //printf("Read word: %s\n", word); // Debugging statement
            pInWords[numWords] = strdup(word); // Change: Allocating memory for each word before copying
            numWords++;
        }

    }
    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }
    // Parse line by line, and insert each word to the trie data structure
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }
    deallocateTrie(pTrie);
    printf("Trie deallocated successfully.\n");
    return 0;
}
