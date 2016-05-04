#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "queue.h"
#include "disk.h"
#include "uthread.h"

unsigned long sum = 0;

/**
 * Queue of threads blocked awaiting a block
 */
queue_t prq;

/**
 * Layout of pending read struct that stores information about reads
 * that have been requested but have not yet completed.
 */
struct pending_read {
  char* buf;
  int   blockno;
  int numBytes;
};

/**
 * Called automatically when an interrupt occurs to signal that a read has completed.
 */
void interrupt_service_routine () {
  // TODO
  uthread_unblock(queue_dequeue(&prq));
}

/**
 * Handle read by ensuring the buffer stores the correct data by
 *  (1) asserting that the first 4 bytes store the correct blockno; and
 *  (2) summing the value in the next 4 bytes as indicated
 * Marking is based partially on having the right value of sum when the
 * program completes.  Note that value written in the second 4 bytes may
 * be different when the assignment is marked.
 */
void handle_read (char* buf, int blockno) {
  assert (*((int*) buf) == blockno);
  sum = sum * 1.1 + *(((int*) buf) + 1);
}

/**
 * Read block number blockno and process the block by calling handle_read
 *
 * This is the inner part of the run loop in the other two versions,
 * abstracted here into a procedure ... why?
 */
void* read (void* blockno_v) {
  // TODO
  long pr = (long) blockno_v;
  char buf[8];
  disk_schedule_read(buf, sizeof(buf), pr);
  queue_enqueue(&prq, uthread_self());
  uthread_block();
  handle_read(buf, pr);
  return NULL;
}

/**
 * Read a count of num_blocks and wait for all reads to complete
 */
void run (int num_blocks) {
  // TODO
  uthread_t threads[num_blocks];
  for (long blockno = 0; blockno < num_blocks; blockno++) {
	  threads[blockno] = uthread_create(read, (void*)blockno);
  }
  
  for (int i = 0; i < num_blocks; i++)
  	uthread_join(threads[i], 0);
}

int main (int argc, char** argv) {
  static const char* usage = "usage: tRead numBlocks";
  int num_blocks = 0;

  if (argc == 2)
    num_blocks = strtol (argv [1], NULL, 10);
  if (argc != 2 || (num_blocks == 0 && errno == EINVAL)) {
    printf ("%s\n", usage);
    return EXIT_FAILURE;
  }
  
  uthread_init (1);
  disk_start   (interrupt_service_routine);
  queue_init   (&prq);
  
  run (num_blocks);
  
  printf ("%ld\n", sum);
}
