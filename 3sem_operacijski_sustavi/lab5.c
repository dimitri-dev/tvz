#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 5
#define NUM_FOODS 8 // čokolada, čevapi, janjetina, štrukle, grah, keksi, kelj, jabuka
#define NUM_FOODS_TO_COOK 5 // number of random foods our maid will cook

int food_consumed[NUM_THREADS] = { 0 };
int food_rejected[NUM_THREADS] = { 0 };
int food_empty[NUM_THREADS] = { 0 };

pthread_mutex_t food_table_mutex = PTHREAD_MUTEX_INITIALIZER;
int food_table[NUM_FOODS] = { 0 };

int food_cooked = 0;

void* tin(void* arg);
void* davor(void* arg);
void* ivica(void* arg);
void* ivan(void* arg);
void* kuharica(void* arg);

// Programming work (calculate a factorial)
int calculate_factorial(int n) {
	int result = 1;
	for (int i = 1; i <= n; i++) {
		result *= i;
	}
	return result;
}

void my_sleep(int timeMs) {
    usleep(timeMs * 1000); // usleep takes microsecond sleep
}


// Helper method to do random sleep
void sleep_random(int minSecond, int maxSecond) {
    if (minSecond == maxSecond) {
        my_sleep(minSecond * 1000); // my sleep function takes miliseconds
    }

	int duration = rand() % (maxSecond * 1000 - minSecond * 1000) + minSecond * 1000;
	my_sleep(duration);
}

// 1 if true, 0 if false
int is_food_table_empty()
{
	int table_empty = 1;
	for (int i = 0; i < NUM_FOODS; ++i) {
		if (food_table[i] != 0) {
			table_empty = 0;
		}
	}

	return table_empty;
}

// -1 if contains everything, otherwise returns index of the first empty food spot.
int first_empty_food_table() {
	for (int i = 0; i < NUM_FOODS; ++i) {
		if (food_table[i] == 0) {
			return i;
		}
	}

	return -1;
}

// Returns -2 if table is empty, or -1 if no eligible food is found.
// Otherwise returns an index of the food consumed.
int consume_food(int* eatableFoods, int n) {
	int table_empty = is_food_table_empty();

	if (table_empty) {
		return -2;
	}

	for (int i = 0; i < n; ++i) {
		int foodIdx = eatableFoods[i];

		if (food_table[foodIdx] > 0) {
			pthread_mutex_lock(&food_table_mutex);

			if (food_table[foodIdx] > 0) {
				food_table[foodIdx]--;
                pthread_mutex_unlock(&food_table_mutex);
				return foodIdx;
			}

			pthread_mutex_unlock(&food_table_mutex);
		}
	}

	return -1;
}

int main() {
	srand(time(NULL));

	pthread_t threads[NUM_THREADS];

	int* tin_id = malloc(sizeof(int));
	*tin_id = 0;

	int* davor_id = malloc(sizeof(int));
	*davor_id = 1;

	int* ivica_id = malloc(sizeof(int));
	*ivica_id = 2;

	int* ivan_id = malloc(sizeof(int));
	*ivan_id = 3;

	int* kuharica_id = malloc(sizeof(int));
	*kuharica_id = 4;

	pthread_create(&threads[0], NULL, tin, tin_id);
	pthread_create(&threads[1], NULL, davor, davor_id);
	pthread_create(&threads[2], NULL, ivica, ivica_id);
	pthread_create(&threads[3], NULL, ivan, ivan_id);
	pthread_create(&threads[4], NULL, kuharica, kuharica_id);

    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

	printf("\nActivity Summary:\n");
	printf("Meals Eaten: Tin=%d, Davor=%d, Ivica=%d, Ivan=%d\n",
		food_consumed[*tin_id], food_consumed[*davor_id], food_consumed[*ivica_id], food_consumed[*ivan_id]);
	printf("Meals Rejected: Tin=%d, Davor=%d, Ivica=%d, Ivan=%d\n",
		food_rejected[*tin_id], food_rejected[*davor_id], food_rejected[*ivica_id], food_rejected[*ivan_id]);
	printf("Table Found Empty: Tin=%d, Davor=%d, Ivica=%d, Ivan=%d\n",
		food_empty[*tin_id], food_empty[*davor_id], food_empty[*ivica_id], food_empty[*ivan_id]);

	return 0;
}

// spava->programira->jede->vozi_auto_na_servis
void* tin(void* arg) {
	int thread_id = *((int*)arg);

	while (food_cooked != NUM_FOODS_TO_COOK) {
		printf("Tin is sleeping...\n");
		sleep_random(7, 10); // Sleep between 7 and 10 seconds. Tin sleeps longer.

		printf("Tin is doing programmer work...\n");
		int num = rand() % 10 + 1;
		calculate_factorial(num);

		// Food Table: čokolada, čevapi, janjetina, štrukle, grah, keksi, kelj, jabuka
		// Tin eats: 0, -, -, 3, 4, 5, 6, 7 (no čevapi or janjetina)
		int eatableFoods[] = { 0, 3, 4, 5, 6, 7 };
		int eatableFoodsSize = sizeof(eatableFoods) / sizeof(eatableFoods[0]);

		printf("Tin wants to eat...\n");

		int food_result = consume_food(eatableFoods, eatableFoodsSize);

		if (food_result >= 0) {
			printf("Tin ate.\n");
            food_consumed[thread_id]++;
		}
		else if (food_result == -1) {
			printf("Tin found no eligible food for him.\n");
			food_rejected[thread_id]++;
		}
		else {
			printf("Tin found the table empty.\n");
			food_empty[thread_id]++;
		}

		printf("Tin is taking the car to be serviced...\n");
		int chance = rand() % 100;
		if (chance < 25) {
			printf("Tin's car has clogged fuel injectors.\n");
			my_sleep(4000);
		}
		else if (chance < 45) {
			printf("Tin's car has an electronic fault.\n");
			my_sleep(2000);
		}
		else {
			printf("Tin's car has a random fault. The diagnosis will take time.\n");
			sleep_random(2, 10);
		}
	}

	return NULL;
}

// spava->programira->jede->gleda_tv
void* davor(void* arg) {
	int thread_id = *((int*)arg);

	while (food_cooked != NUM_FOODS_TO_COOK) {
		printf("Davor is sleeping...\n");
		sleep_random(5, 10); // Sleep between 5 and 10 seconds.

		printf("Davor is doing programmer work...\n");
		int num = rand() % 10 + 1;
		calculate_factorial(num);

		// Food Table: čokolada, čevapi, janjetina, štrukle, grah, keksi, kelj, jabuka
		// Davor eats: -, 1, 2, 3, 4, -, 6, 7 (no sweets) - we assume "štrukle" aren't sweet here.
		int eatableFoods[] = { 1, 2, 3, 4, 6, 7 };
		int eatableFoodsSize = sizeof(eatableFoods) / sizeof(eatableFoods[0]);

		printf("Davor wants to eat...\n");

		int food_result = consume_food(eatableFoods, eatableFoodsSize);

		if (food_result >= 0) {
			printf("Davor ate.\n");
            food_consumed[thread_id]++;
		}
		else if (food_result == -1) {
			printf("Davor found no eligible food for him.\n");
			food_rejected[thread_id]++;
		}
		else {
			printf("Davor found the table empty.\n");
			food_empty[thread_id]++;
		}

		printf("Davor is watching the TV...\n");
		sleep_random(2, 4);
	}

	return NULL;
}

// spava->igra_tenis->jede->programira
void* ivica(void* arg) {
	int thread_id = *((int*)arg);

	while (food_cooked != NUM_FOODS_TO_COOK) {
		printf("Ivica is sleeping...\n");
		sleep_random(5, 10); // Sleep between 5 and 10 seconds.

		printf("Ivica is playing Tennis...\n");
		sleep_random(2, 4);

		// Food Table: čokolada, čevapi, janjetina, štrukle, grah, keksi, kelj, jabuka
		// Ivica eats: 0, 1, 2, 3, -, 5, -, 7 (no čušpajz food)
		int eatableFoods[] = { 0, 1, 2, 3, 5, 7 };
		int eatableFoodsSize = sizeof(eatableFoods) / sizeof(eatableFoods[0]);

		printf("Ivica wants to eat...\n");

		int food_result = consume_food(eatableFoods, eatableFoodsSize);

		if (food_result >= 0) {
			printf("Ivica ate.\n");
            food_consumed[thread_id]++;
		}
		else if (food_result == -1) {
			printf("Ivica found no eligible food for him.\n");
			food_rejected[thread_id]++;
		}
		else {
			printf("Ivica found the table empty.\n");
			food_empty[thread_id]++;
		}

		printf("Ivica is doing programmer work...\n");
		int num = rand() % 10 + 1;
		calculate_factorial(num);
	}

	return NULL;
}

// spava->slusa_sviranje_klavira->jede->programira
void* ivan(void* arg) {
	int thread_id = *((int*)arg);

	while (food_cooked != NUM_FOODS_TO_COOK) {
		printf("Ivan is sleeping...\n");
		sleep_random(5, 10); // Sleep between 5 and 10 seconds.

		printf("Ivan is listening to Piano music...\n");
		sleep_random(2, 4);

		// Food Table: čokolada, čevapi, janjetina, štrukle, grah, keksi, kelj, jabuka
		// Ivan eats: 0, 1, 2, -, 4, 5, 6, 7 (no štrukli)
		int eatableFoods[] = { 0, 1, 2, 4, 5, 6, 7 };
		int eatableFoodsSize = sizeof(eatableFoods) / sizeof(eatableFoods[0]);

		printf("Ivan wants to eat...\n");

		int food_result = consume_food(eatableFoods, eatableFoodsSize);

		if (food_result >= 0) {
			printf("Ivan ate.\n");
            food_consumed[thread_id]++;
		}
		else if (food_result == -1) {
			printf("Ivan found no eligible food for him.\n");
			food_rejected[thread_id]++;
		}
		else {
			printf("Ivan found the table empty.\n");
			food_empty[thread_id]++;
		}

		printf("Ivan is doing programmer work...\n");
		int num = rand() % 10 + 1;
		calculate_factorial(num);
	}

	return NULL;
}

void* kuharica(void* arg) {
	int thread_id = *((int*)arg);

	// Maid lifecycle depends on the number of food to cook.
	while (food_cooked != NUM_FOODS_TO_COOK) {
		printf("The maid is cooking...\n");
		sleep_random(1, 2);

		while (!0) {
			int food_spot = first_empty_food_table();

			if (food_spot == -1) {
				continue;
			}

			pthread_mutex_lock(&food_table_mutex);

			if (food_table[food_spot] == 0) {
				printf("The maid put more food on the table...\n");
				food_table[food_spot]++;
                ++food_cooked;
			}

			pthread_mutex_unlock(&food_table_mutex);

			break;
		}

		printf("The maid is resting...\n");
		sleep_random(1, 2);
	}

	return NULL;
}
