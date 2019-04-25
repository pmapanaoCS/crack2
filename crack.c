#include <stdio.h>
#include <sys/stat.h> 
#include <sys/types.h> 
#include <stdlib.h>
#include <string.h>

#include "md5.h"

const int PASS_LEN=20;        // Maximum any password will be
const int HASH_LEN=33;        // Length of MD5 hash strings

// Given a hash and a plaintext guess, return 1 if
// the hash of the guess matches the given hash.
// That is, return 1 if the guess is correct.

// Function prototypes
int tryguess(char *hash, char *guess);
char **read_dictionary(char *filename, int *size);
void checkFiles(int n);
void checkOpen(FILE *f, char *fName);
int fileLength(char * fName);


int tryguess(char *hash, char *guess)
{
    // Hash the guess using MD5
    // Compare the two hashes
    // Free any malloc'd memory
    
    char * guessHash = md5(guess, strlen(guess));
    if (*hash == *guessHash) return 1;
    else return 0;

}

// Read in the dictionary file and return the array of strings
// and store the length of the array in size.
// This function is responsible for opening the dictionary file,
// reading from it, building the data structure, and closing the
// file.
char **read_dictionary(char *filename, int *size)
{
    // Get the length of the file to put into the dictionary 
    int len = fileLength(filename);
    printf("This is the length of the file of passwords: %d\n", len);
    
    // Allocate space in the heap for all the contents 
    char *contents = malloc(len); 
    
    // Open file for reading, and check if it can be opened otherwise end program
    FILE *f = fopen(filename,"r");
    checkOpen(f, "DICTIONARY FILE");
    printf("****DICTIONARY FILE opened sucessfully****\n");
    
    // Read entire file into memory all at once, and close the file
    fread(contents, 1, len, f);
    
    //Close dictionary file
    fclose(f); 
    
    // Replace newlines with null chars, and keep track of number of hashes
    *size = 0;
    for (int i = 0; i < len; i++)
    {
        if (contents[i] == '\n')
        {
            *size += 1;  
            contents[i] = '\0';
        }
    }
    
    // Allocate space for array of pointers to strings
    char **d = malloc(*size * sizeof(char*));
    
    // Populate dictionary 
    d[0] = contents; 
    int j = 1; 
    for (int i = 0; i < len-1; i++)
    {
        if (contents[i] == '\0')
        {
            d[j] = &contents[i+1];
            j++;
        }
        
    }
    
    //free(d[0]);
    //free(d);
    
    return d;
}

//My new functions: to check all files for opening and get length of file 
// Checks number of files
void checkFiles(int n)
{
    if (n < 3){
        printf("You need to supply two filenames\n");
        exit(1);
    }
} 

// Checks if file can be opened 
void checkOpen(FILE *f, char *fName)
{
    if(!f){
        printf("Cannot open %s for reading \n", fName);
        printf("Program will now close. . . \n");
        exit(1); 
    }
}

// Using stat, this function gets file length 
int fileLength(char * fName)
{
    struct stat info;
    if (stat(fName, &info) == -1) return -1; 
    else return info.st_size; 
}



int main(int argc, char *argv[])
{
    //Checks for two file args
    checkFiles(argc);

    // Read the dictionary file into an array of strings.
    int dlen;
    char **dict = read_dictionary(argv[2], &dlen);
    printf("****dlen after read_dict is now: %d ****\n", dlen);

    // Open the hash file for reading.
    FILE * hashFile = fopen(argv[1], "r");
    
    // Checks if it can be opened 
    checkOpen(hashFile, "HASH FILE");
    printf("****HASH FILE opened sucessfully****\n");
    
    // For each hash, try every entry in the dictionary.
    // Print the matching dictionary entry.
    // Need two nested loops.
    char line[100];
    
    // This will keep track of the number of correct guesses. 
    int correctHashes = 0; 
    // Print each dictionary word 
    while(fgets(line, 100, hashFile) != NULL)
    {
        for (int i = 0; i < dlen; i++)
        {
            printf("Testing: dict[%d] %s\n", i, dict[i]);
            if(tryguess(line,dict[i]))
            {
                printf("!! MATCH FOUND: [%d] %s !!\n", i, dict[i]);
                correctHashes++; 
                //free(dict[i]);
            }
            else{
                printf("NO MATCH: [%d] %s\n", i, dict[i]);
                //free(dict[i]);
            }
            
        }
    }
    
    printf("The number of cracked passwords is: %d\n",correctHashes);
    
    // Free allocated mem
    free(dict[0]);
    free(dict);
    fclose(hashFile);
    
    // Ensure proper pop of main stack
    return 0; 
    
}

/*
 *  Malloc for entire file
 *  Malloc for array 
 *  Return 2 things
 *  Pointer to an integer, so that we can find our way back to other param 
 */ 