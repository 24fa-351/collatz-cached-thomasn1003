#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct CacheEntry {
    int number;
    int steps;
} CacheEntry;

// Globals for cache, policy, and hit statistics
CacheEntry *cache;
int cacheSize = 0;
int cachePolicy = 0; // 0 = None, 1 = LRU, 2 = FIFO
int cacheHits = 0, cacheAccesses = 0;
int cachePointer = 0; // For FIFO pointer

// Function to calculate Collatz steps
int collatz_steps(int n) {
    int steps = 0;
    while (n != 1) {
        if (n % 2 == 0) {
            n /= 2;
        } else {
            n = 3 * n + 1;
        }
        steps++;
    }
    return steps;
}

// Cache lookup function
int check_cache(int number) {
    for (int i = 0; i < cacheSize; i++) {
        if (cache[i].number == number) {
            cacheHits++;
            return cache[i].steps;
        }
    }
    return -1; // Not in cache
}

// Add to cache with FIFO or LRU policy
void add_to_cache(int number, int steps) {
    if (cacheSize == 0) return; // No cache
    
    if (cachePolicy == 1) { // LRU
        // Simply replace the first element (oldest) for simplicity
        for (int i = cacheSize - 1; i > 0; i--) {
            cache[i] = cache[i - 1]; // Shift all elements
        }
        cache[0].number = number;
        cache[0].steps = steps;
    } else if (cachePolicy == 2) { // FIFO
        // Replace entry in round-robin fashion
        cache[cachePointer].number = number;
        cache[cachePointer].steps = steps;
        cachePointer = (cachePointer + 1) % cacheSize; // Circular buffer
    }
}

// Function to process a number with caching
int process_number(int number) {
    cacheAccesses++;
    int steps = check_cache(number);
    if (steps != -1) return steps; // Cache hit
    
    // Cache miss, calculate Collatz steps
    steps = collatz_steps(number);
    add_to_cache(number, steps);
    return steps;
}

int main(int argc, char *argv[]) {
    
    // Parse arguments
    int N = atoi(argv[1]);
    int MIN = atoi(argv[2]);
    int MAX = atoi(argv[3]);

    if (MIN >= MAX) {
        printf("MIN must be less than MAX.\n");
        return 1;
    }

    // Cache policy and size
    printf("Enter cache policy (0 = none, 1 = LRU, 2 = FIFO): ");
    scanf("%d", &cachePolicy);
    printf("Enter cache size: ");
    scanf("%d", &cacheSize);

    // Initialize cache if needed
    if (cachePolicy != 0) {
        cache = (CacheEntry *)malloc(cacheSize * sizeof(CacheEntry));
        for (int i = 0; i < cacheSize; i++) {
            cache[i].number = -1;
            cache[i].steps = 0;
        }
    }

    srand(time(NULL));
    FILE *fp = fopen("collatz_results.csv", "w");

    fprintf(fp, "Number,Steps\n");

    for (int i = 0; i < N; i++) {
        int RN = (rand() % (MAX - MIN + 1)) + MIN;
        int steps = process_number(RN);
        fprintf(fp, "%d,%d\n", RN, steps);
    }

    fclose(fp);

    // Report cache statistics
    printf("Cache hit percentage: %.2f%%\n", (100.0 * cacheHits) / cacheAccesses);

    if (cachePolicy != 0) {
        free(cache);
    }

    return 0;
}
