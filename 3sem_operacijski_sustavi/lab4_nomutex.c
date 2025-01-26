#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_ROWS 10
#define NUM_COLS 10
#define NUM_THREADS (NUM_ROWS * NUM_COLS)

// Seat array, where 0 means empty/free and 1 means occupied
int seats[NUM_ROWS][NUM_COLS] = {0};

void* user_simulation(void* arg);

void my_sleep(int timeMs) {
    usleep(timeMs * 1000); // usleep takes microsecond sleep
}

void sleep_random(int minSecond, int maxSecond) {
    if (minSecond == maxSecond) {
        my_sleep(minSecond * 1000); // my sleep function takes miliseconds
    }

	int duration = rand() % (maxSecond * 1000 - minSecond * 1000) + minSecond * 1000;
	my_sleep(duration);
}

int main() {
    srand(time(NULL));

    pthread_t threads[NUM_THREADS];
    clock_t startTime = clock();

    for (int i = 0; i < NUM_THREADS; i++) {
        int* user_id = malloc(sizeof(int));
        *user_id = i + 1;

        pthread_create(&threads[i], NULL, user_simulation, user_id);
    }

    // Wait for all threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t endTime = clock();

    // Count the number of occupied and empty seats
    int occupied_count = 0;
    int empty_count = 0;
    for (int row = 0; row < NUM_ROWS; row++) {
        for (int col = 0; col < NUM_COLS; col++) {
            if (seats[row][col]) {
                ++occupied_count;
            } else {
                ++empty_count;
            }
        }
    }

    // Calculate time taken in milliseconds
    double timeElapsed = 1000.0 * (endTime - startTime) / CLOCKS_PER_SEC;

    printf("Boarding status:\n");
    printf("Occupied seats: %d\n", occupied_count);
    printf("Empty seats: %d\n", empty_count);

    printf("Time taken: %.3f ms\n", timeElapsed);

    return 0;
}

void* user_simulation(void* arg) {
    int user_id = *((int*)arg);

    // WiFi connectivity delay. (1-5 seconds)
    sleep_random(1, 5);
    
    // Random delay to get seat availability array. (1-3 seconds)
    sleep_random(1, 3);

    int selected_row = -1;
    int selected_col = -1;
    int seat_accepted = 0;

    while (!seat_accepted) {
        // Randomly choose a row (0-9) and column (0-9)
        selected_row = rand() % NUM_ROWS;
        selected_col = rand() % NUM_COLS;

        if (!seats[selected_row][selected_col]) {
            int user_accepts_seat = rand() % 2;
            if (user_accepts_seat) {
                // Process API request delay. (1-2 seconds)
                sleep_random(1, 2);

                seat_accepted = seats[selected_row][selected_col] = 1;
                printf("User %d selected seat (%d, %d)\n", user_id, ++selected_row, ++selected_col);
            } else {
                printf("User %d rejected seat (%d, %d), trying again...\n", user_id, ++selected_row, ++selected_col);
            }
        }
    }

    return NULL;
}