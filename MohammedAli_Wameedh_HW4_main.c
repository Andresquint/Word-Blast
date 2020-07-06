
/**************************************************************
* Class:  CSC-415-0# Spring 2020
* Name: Wameedh Mohammed Ali
* Student ID: 920678405
* Project: Assignment 4 - Word Blast
*
* File: MohammedAli_Wameedh_HW4_main.c
*
* Description: Counting all the words that are 6 or more characters long. We will only use Linux file functions, i.e. open, close, read, lseek.
*
**************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
/***********************************************
Data Structure and its variables Declared/defined here
************************************************/
struct Word{ char word[50]; int frequency;}; // This struct would be the object that would store each word with its frequency.
struct Word* wordsArray; // This would be the array that would holds structs of words
int indexOfArray = 0; // this integer will keep track of the wordsArray size.
/***********************************************
 I found this sorting method online
 Source: https://www.educba.com/sorting-in-c/
************************************************/

void quicksortMethod(struct Word element_list[], int low, int high)
{
    int pivot, value1, value2;
    struct Word temp;

    if (low < high){
        pivot = low;
        value1 = low;
        value2 = high;
        while (value1 < value2){
            while (element_list[value1].frequency <= element_list[pivot].frequency && value1 <= high){
                value1++;
            }
            while (element_list[value2].frequency > element_list[pivot].frequency && value2 >= low){
                value2--;
            }
            if (value1 < value2){
                temp = element_list[value1];
                element_list[value1] = element_list[value2];
                element_list[value2] = temp;
            }
        }
        temp = element_list[value2];
        element_list[value2] = element_list[pivot];
        element_list[pivot] = temp;
        quicksortMethod(element_list, low, value2 - 1);
        quicksortMethod(element_list, value2 + 1, high);
    }
}

/***********************************************
END OF QUICK SORT
************************************************/

/***********************************************
Global variables Declared/defined here
************************************************/
// You may find this Useful
char * delim = "\"\'.“”‘’?:;-,—*($%)! \t\n\x0A\r";
int NumberOfThreads; // This Variable will store the number of threads
int fd; // file descriptor
size_t fileSize; // This Variable will store the total file size
size_t chunk; // This Variable will store a size that is equals to [file size / Number of threads]
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //initializing a mutex lock variable

/***********************************************
Functions Declared/defined here
************************************************/
// This function add words to the array of structs
// if the word does not exists then it will just increase the frequency of the word
// Mutex is used in here to protect the shared resources.
void addingWords(char* token){
//    pthread_mutex_lock( &mutex1 );	//Starts the critical section
    for (int i = 0; i <= indexOfArray; i++) {
        if(i == indexOfArray){
            pthread_mutex_lock( &mutex );//Starts the critical section
            strcpy(wordsArray[i].word,token);
            wordsArray[i].frequency += 1;
            indexOfArray += 1;
            // Dynamically re-allocate memory for wordsArray using realloc()
            wordsArray = realloc(wordsArray, sizeof(wordsArray) + sizeof(struct Word));
            pthread_mutex_unlock( &mutex );  //Ends the critical section
            break;
        }else if(strcmp(wordsArray[i].word, token) == 0){
            pthread_mutex_lock( &mutex );	//Starts the critical section
            wordsArray[i].frequency += 1;
            pthread_mutex_unlock( &mutex );  //Ends the critical section
            break;
        }
    }
}
/*Thread function*/
//this function will be passed into thread is created
//and will scan through each chunk of the file
void* readWordsFromFile(void* arg){

    char* buffer =  (char *) malloc(chunk); // Each thread would allocated a buffer of size "chunk"

    off_t* bufferEnd = (off_t*) arg;   //get buffer end for each chunk
<<<<<<< HEAD
    printf("\nbufferEnd %ld.\n",*bufferEnd);
    //int x = 1679774 + 2519661 + 839887;

    off_t buffer_start = *bufferEnd - chunk; //get buffer start for each chunk
    //printf("\nbuffer_start %lld.\n",buffer_start);

    // lseek(fd, buffer_start, SEEK_SET);
    pread(fd, buffer, chunk, buffer_start);
    pthread_mutex_lock( &mutex1 );	//Starts the critical section
    //code to scan chunk here

=======
    off_t bufferStartOffset = *bufferEnd - chunk; //get buffer start for each chunk
    pread(fd, buffer, chunk, bufferStartOffset); // read from a file descriptor at a given offset
>>>>>>> 36d3f0997de34b40b41cb80eb514f64cc296d872
    char* token;
    while ((token = strtok_r(buffer , delim, &buffer))) {
        if (strlen(token) >= 6) { // The word is equals or larger than 6 char
            addingWords(token);
        }
    }
<<<<<<< HEAD
    //free(buffer);
    pthread_mutex_unlock( &mutex1 );  //Ends the critical section
    
    //indicates end of thread and can be terminated
    pthread_exit(0);
   free(buffer);
=======
    pthread_exit(NULL);    // End of thread
>>>>>>> 36d3f0997de34b40b41cb80eb514f64cc296d872
}

int main (int argc, char *argv[])
    {
    //***TO DO***  Look at arguments, open file, divide by threads
    //             Allocate and Initialize and storage structures
        //allocating an initial size for our array of structs
        wordsArray =  malloc(1 * sizeof(struct Word));

//        indexOfArray = 0;

        if(!argv[1] || !argv[2]){
            printf("File or number of threads was not specified.\n");
            exit(0);
        }
        fd = open(argv[1], O_RDONLY);	//opening file descriptor for the .txt file
        //HANDEL NOT OPEN
        NumberOfThreads = atoi(argv[2]); // gets the number of threads we want to create, also converts from ASCII to integer
        off_t current = lseek(fd, 0, SEEK_CUR);
        fileSize = lseek(fd, 0, SEEK_END); //this will get the number of bytes in the file
        chunk = (fileSize / NumberOfThreads); //this will tell us the max # of bytes each thread scans
        lseek(fd, current, SEEK_SET); //reset the position back to beginning of file

    off_t* bufferPositionsArray[NumberOfThreads]; // This array will have number of positions that is equal to the number of theards
    for (int i = 0; i < NumberOfThreads; i++){
        bufferPositionsArray[i] = (off_t*) ((i + 1) * chunk);
    }

    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Time stamp start
    struct timespec startTime;
    struct timespec endTime;

    clock_gettime(CLOCK_REALTIME, &startTime);
    //**************************************************************
    // *** TO DO ***  start your thread processing
    //                wait for the threads to finish

    //Creating an array of thread IDs
    pthread_t tids[NumberOfThreads];
    //Creating each thread
    for(int i = 0; i < NumberOfThreads; i++){
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tids[i], &attr, readWordsFromFile, &bufferPositionsArray[i]);
    }
    //waiting for each thread to finish before the program
    //exits
    for(int i = 0; i < NumberOfThreads; i++){
        pthread_join(tids[i], NULL);
    }
    // ***TO DO *** Process TOP 10 and display
    quicksortMethod(wordsArray,0,indexOfArray);
    //This quick sort method sort items by having the TOP 10 words at the end of the array,
    //so I am printing from the end of the array 10 items
    int x = 1;
    for (int j = indexOfArray; j > indexOfArray - 10; j--) {
        printf("\nNumber %d is %s with a count of %d.\n",x,wordsArray[j].word,wordsArray[j].frequency );
        x++;
    }

    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Clock output
    clock_gettime(CLOCK_REALTIME, &endTime);
    time_t sec = endTime.tv_sec - startTime.tv_sec;
    long n_sec = endTime.tv_nsec - startTime.tv_nsec;
    if (endTime.tv_nsec < startTime.tv_nsec)
        {
        --sec;
        n_sec = n_sec + 1000000000L;
        }

    printf("Total Time was %ld.%09ld seconds\n", sec, n_sec);
    //**************************************************************
        close(fd);
        free(wordsArray);
    // ***TO DO *** cleanup
    }
