#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_sem.h"

#define MAX_ITEMS 10
const int NUM_ITERATIONS = 200;
const int NUM_CONSUMERS  = 2;
const int NUM_PRODUCERS  = 2;

int producer_wait_count;     // # of times producer had to wait
int consumer_wait_count;     // # of times consumer had to wait
int histogram [MAX_ITEMS+1]; // histogram [i] == # of times list stored i items

struct Pool {
  // TODO
  uthread_sem_t notEmpty;
  uthread_sem_t notFull;
  uthread_sem_t semaphore;
  int           items;
};

struct Pool* createPool() {
  struct Pool* pool = malloc (sizeof (struct Pool));
  // TODO
  pool->notEmpty = uthread_sem_create(0);
  pool->notFull = uthread_sem_create(MAX_ITEMS);
  pool->semaphore = uthread_sem_create(1);
  pool->items     = 0;
  return pool;
}

void produce(struct Pool* pool) {
	uthread_sem_wait(pool->notFull);
	uthread_sem_wait(pool->semaphore);
    pool->items++;
    histogram[pool->items]++;
    uthread_sem_signal(pool->semaphore);
    uthread_sem_signal(pool->notEmpty);
}

void consume(struct Pool* pool) {
	uthread_sem_wait(pool->notEmpty);
	uthread_sem_wait(pool->semaphore);
    pool->items--;
    histogram[pool->items]++;
    uthread_sem_signal(pool->semaphore);
    uthread_sem_signal(pool->notFull);
}

void* producer (void* pv) {
  struct Pool* p = pv;
  
  // TODO
  for (int i=0; i<NUM_ITERATIONS; i++) {
    produce(p);
  }

  return NULL;
}

void* consumer (void* pv) {
  struct Pool* p = pv;
  
  // TODO
  for (int i=0; i<NUM_ITERATIONS; i++) {
    consume(p);
  }
  
  return NULL;
}

int main (int argc, char** argv) {
  uthread_t t[4];

  uthread_init (4);
  struct Pool* p = createPool();
  
  // TODO
  
  for (int i = 0; i < 2; i++)
  	t[i] = uthread_create(producer, p);
  	
  for (int i = 2; i < 4; i++)
  	t[i] = uthread_create(consumer, p);
  	
  for (int i = 0; i < 4; i++)
  	uthread_join(t[i], 0);
  
  printf ("producer_wait_count=%d, consumer_wait_count=%d\n", producer_wait_count, consumer_wait_count);
  printf ("items value histogram:\n");
  int sum=0;
  for (int i = 0; i <= MAX_ITEMS; i++) {
    printf ("  items=%d, %d times\n", i, histogram [i]);
    sum += histogram [i];
  }
  assert (sum == sizeof (t) / sizeof (uthread_t) * NUM_ITERATIONS);
}