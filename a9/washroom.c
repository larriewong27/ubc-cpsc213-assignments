#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

/**
 * You might find these declarations useful.
 */
enum Sex {MALE = 0, FEMALE = 1, NONE=-1};
const static enum Sex otherSex [] = {FEMALE, MALE, NONE};

#define bool int
#define true 1
#define false 0

int males_inside;
int females_inside;
int males_waiting;
int females_waiting;
int num_females = 0;
int num_males = 0;

int occupancy = 0;
bool ready = false;

struct Washroom {
  // TODO
	uthread_mutex_t mutex;
	uthread_cond_t male;
	uthread_cond_t female;
	uthread_cond_t someone_entered;
	uthread_cond_t ready;
};

struct Washroom* createWashroom() {
  struct Washroom* washroom = malloc (sizeof (struct Washroom));
  // TODO
	washroom->mutex = uthread_mutex_create();
	washroom->male = uthread_cond_create(washroom->mutex);
	washroom->female = uthread_cond_create(washroom->mutex);
	washroom->someone_entered = uthread_cond_create(washroom->mutex);
	washroom->ready = uthread_cond_create(washroom->mutex);
	return washroom;
}

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int entryTicker;  // incremented with each entry
int waitingHistogram[WAITING_HISTOGRAM_SIZE];
int waitingHistogramOverflow;
uthread_mutex_t waitingHistogrammutex;
int occupancyHistogram[2][MAX_OCCUPANCY + 1];

void recordWaitingTime(int waitingTime) {
	uthread_mutex_lock(waitingHistogrammutex);
	if (waitingTime < WAITING_HISTOGRAM_SIZE)
		waitingHistogram[waitingTime] ++;
	else
		waitingHistogramOverflow++;
	uthread_mutex_unlock(waitingHistogrammutex);
}

void enterWashroom(struct Washroom* washroom, enum Sex Sex) {
	// TODO
	if (Sex == MALE) {
		int wait_counter = entryTicker;
		males_waiting++;
		while ((occupancy == 3) || (females_inside > 0)) {
			wait_counter++;
			uthread_cond_wait(washroom->male);
		}
		males_waiting--;
		wait_counter = entryTicker - wait_counter;
		recordWaitingTime(wait_counter);
		assert(occupancy < 4);
		assert(females_inside == 0);
		males_inside++;
		occupancy++;
		occupancyHistogram[MALE][males_inside] ++;
	}
	else {
		int wait_counter = entryTicker;
		females_waiting++;
		while ((occupancy == 3) || (males_inside > 0)) {
			wait_counter++;
			uthread_cond_wait(washroom->female);
		}
		females_waiting--;
		wait_counter = entryTicker - wait_counter;
		recordWaitingTime(wait_counter);
		assert(occupancy < 4);
		assert(males_inside == 0);
		females_inside++;
		occupancy++;
		occupancyHistogram[FEMALE][females_inside]++;
	}
}

void male_leave(struct Washroom* wc) {
	males_inside--;
	if (females_waiting > 0 && males_inside == 0) {
		for (int i = 0; i < 3; i++) {
			uthread_cond_signal(wc->female);
		}
	}
	else {
		uthread_cond_signal(wc->male);
	}
}

void female_leave(struct Washroom* wc) {
	females_inside--;
	if (males_waiting > 0 && females_inside == 0) {
		for (int i = 0; i < 3; i++) {
			uthread_cond_signal(wc->male);
		}
	}
	else {
		uthread_cond_signal(wc->female);
	}
}

void leaveWashroom (struct Washroom* washroom) {
  // TODO
	uthread_mutex_lock(washroom->mutex);
	if (males_inside >= 1) male_leave(washroom);
	else female_leave(washroom);

	occupancy--;
	uthread_mutex_unlock(washroom->mutex);
}

//
// TODO
// You will probably need to create some additional produres etc.
//


void iter_yield(int n) {
	for (int i = 0; i < n; i++) {
		uthread_yield();
	}
}

void* person(void* wcv) {
	struct Washroom* wc = wcv;
	int s = random() % 2;
	if (s == FEMALE) num_females++;
	else num_males++;
	for (int i = 0; i < NUM_ITERATIONS; i++) {
		uthread_mutex_lock(wc->mutex);
		if (s == FEMALE) enterWashroom(wc, FEMALE);
		else enterWashroom(wc, MALE);
		entryTicker++;
		while (!ready) {
			uthread_cond_wait(wc->ready);
		}
		ready = false;
		uthread_cond_signal(wc->someone_entered);
		uthread_mutex_unlock(wc->mutex);
		iter_yield(NUM_PEOPLE);
		leaveWashroom(wc);
		iter_yield(NUM_PEOPLE);
	}
}

void* gatekeeper(void* wcv) {
	struct Washroom* wc = wcv;
	for (int i = 0; i < NUM_ITERATIONS * NUM_PEOPLE; i++) {
		uthread_mutex_lock(wc->mutex);
		ready = true;
		uthread_cond_signal(wc->ready);
		uthread_cond_wait(wc->someone_entered);
		uthread_mutex_unlock(wc->mutex);
	}
}

int main (int argc, char** argv) {
  uthread_init (1);
  struct Washroom* washroom = createWashroom();
  uthread_t pt [NUM_PEOPLE];
  waitingHistogrammutex = uthread_mutex_create ();

  // TODO
	males_inside = 0;
	females_inside = 0;
	males_waiting = 0;
	females_waiting = 0;

	for (int i = 0; i < NUM_PEOPLE; i++) {
		pt[i] = uthread_create(person, washroom);
	}
	uthread_t gk = uthread_create(gatekeeper, washroom);
	uthread_join(gk, 0);
  
  printf ("Times with 1 male    %d\n", occupancyHistogram [MALE]   [1]);
  printf ("Times with 2 males   %d\n", occupancyHistogram [MALE]   [2]);
  printf ("Times with 3 males   %d\n", occupancyHistogram [MALE]   [3]);
  printf ("Times with 1 female  %d\n", occupancyHistogram [FEMALE] [1]);
  printf ("Times with 2 females %d\n", occupancyHistogram [FEMALE] [2]);
  printf ("Times with 3 females %d\n", occupancyHistogram [FEMALE] [3]);
  printf ("Waiting Histogram\n");
  for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Number of times people waited for %d %s to enter: %d\n", i, i==1?"person":"people", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Number of times people waited more than %d entries: %d\n", WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}