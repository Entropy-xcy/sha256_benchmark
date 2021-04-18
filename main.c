#include <stdio.h>
#include "openssl/sha.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

void sha256_string(char *string, char outputBuffer[65])
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string, strlen(string));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

void *worker_thread(void *vargp)
{
    int *num_hashes = (int *)vargp;
    printf("Thread Got %d Hashes to run\n", *num_hashes);

    char hash_buffer[65];
    for(int i = 0; i < *num_hashes; i++)
    {
        sha256_string(hash_buffer, hash_buffer);
    }
}

int main(int argc, char **argv) {
    if(argc != 3)
    {
	    printf("Required 2 Input Arguments: <num_threads> <num_hashes>\n");
	    return 1;
    }

    int num_threads = atoi(argv[1]);
    int num_hashes = atoi(argv[2]);
	
    printf("Benchmarking %d SHA256 Hashes on %d Threads...\n", num_hashes, num_threads);
    
    int num_hashes_per_thread = num_hashes / num_threads;
    
    // Start Parallel Execution
    clock_t t;
    t = clock();

    pthread_t thread_ids[num_threads];
    for(int i = 0; i < num_threads; i++)
    {
        pthread_create(&thread_ids[i], NULL, worker_thread, (void *)&num_hashes_per_thread);
    }

    for(int i = 0; i < num_threads; i++)
    {
        pthread_join(thread_ids[i], NULL);
    }
    t = clock() - t;
    double time_taken = ((double)t)/CLOCKS_PER_SEC;
    printf("Takes %lf Seconds.\n", time_taken);
}


