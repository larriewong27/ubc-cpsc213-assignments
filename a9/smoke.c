#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create (agent->mutex);
  agent->match   = uthread_cond_create (agent->mutex);
  agent->tobacco = uthread_cond_create (agent->mutex);
  agent->smoke   = uthread_cond_create (agent->mutex);
  return agent;
}

//
// TODO
// You will probably need to add some procedures and struct etc.
//

struct Smoker {
	int t_count;
	int p_count;
	int m_count;
	uthread_cond_t t_smoke;
	uthread_cond_t p_smoke;
	uthread_cond_t m_smoke;
	struct Agent* a;
};

struct Smoker* createSmoker(struct Agent* agent) {
	struct Smoker* smoker = malloc (sizeof (struct Smoker));
	smoker->a		= agent;
	smoker->t_count = 0;
	smoker->p_count = 0;
	smoker->m_count = 0;
	smoker->t_smoke = uthread_cond_create (agent->mutex);
	smoker->p_smoke = uthread_cond_create (agent->mutex);
	smoker->m_smoke = uthread_cond_create (agent->mutex);
	return smoker;
}

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

void* tobacco_count(void* sv) {
	struct Smoker* s = sv;
	uthread_mutex_lock(s->a->mutex);
	while(1) {
		uthread_cond_wait(s->a->tobacco);
		s->t_count++;
		if (s->p_count > 0 || s->m_count > 0) {
			s->t_count--;
			if (s->p_count > 0) {
				s->p_count--;
				uthread_cond_signal(s->m_smoke);
			}
			if (s->m_count > 0) {
				s->m_count--;
				uthread_cond_signal(s->p_smoke);
			}
		}
	}
	uthread_mutex_unlock(s->a->mutex);
	return NULL;
}

void* paper_count(void* sv) {
	struct Smoker* s = sv;
	uthread_mutex_lock(s->a->mutex);
	while(1) {
		uthread_cond_wait(s->a->paper);
		s->p_count++;
		if (s->t_count > 0 || s->m_count > 0) {
			s->p_count--;
			if (s->t_count > 0) {
				s->t_count--;
				uthread_cond_signal(s->m_smoke);
			}
			if (s->m_count > 0) {
				s->m_count--;
				uthread_cond_signal(s->t_smoke);
			}
		}
	}
	uthread_mutex_unlock(s->a->mutex);
	return NULL;
}

void* match_count(void* sv) {
	struct Smoker* s = sv;
	uthread_mutex_lock(s->a->mutex);
	while(1) {
		uthread_cond_wait(s->a->match);
		s->m_count++;
		if (s->t_count > 0 || s->p_count > 0) {
			s->m_count--;
			if (s->t_count > 0) {
				s->t_count--;
				uthread_cond_signal(s->p_smoke);
			}
			if (s->p_count > 0) {
				s->p_count--;
				uthread_cond_signal(s->t_smoke);
			}
		}
	}
	uthread_mutex_unlock(s->a->mutex);
	return NULL;
}

void* tobacco_smoke(void* sv) {
	struct Smoker* s = sv;
	uthread_mutex_lock(s->a->mutex);
	while(1) {
		uthread_cond_wait(s->t_smoke);
		smoke_count[TOBACCO]++;
		uthread_cond_signal(s->a->smoke);	
	}
	uthread_mutex_unlock(s->a->mutex);
	return NULL;
}

void* paper_smoke(void* sv) {
	struct Smoker* s = sv;
	uthread_mutex_lock(s->a->mutex);
	while(1) {
		uthread_cond_wait(s->p_smoke);
		smoke_count[PAPER]++;
		uthread_cond_signal(s->a->smoke);
	}
	uthread_mutex_unlock(s->a->mutex);
	return NULL;
}

void* match_smoke(void* sv) {
	struct Smoker* s = sv;
	uthread_mutex_lock(s->a->mutex);
	while(1) {
		uthread_cond_wait(s->m_smoke);
		smoke_count[MATCH]++;
		uthread_cond_signal(s->a->smoke);
	}
	uthread_mutex_unlock(s->a->mutex);
	return NULL;
}

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can re-write it if you like, but be sure that all it does
 * is choose 2 random reasources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};
  
  uthread_mutex_lock (a->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      int r = random() % 3;
      signal_count [matching_smoker [r]] ++;
      int c = choices [r];
      if (c & MATCH) {
        VERBOSE_PRINT ("match available\n");
        uthread_cond_signal (a->match);
      }
      if (c & PAPER) {
        VERBOSE_PRINT ("paper available\n");
        uthread_cond_signal (a->paper);
      }
      if (c & TOBACCO) {
        VERBOSE_PRINT ("tobacco available\n");
        uthread_cond_signal (a->tobacco);
      }
      VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
      uthread_cond_wait (a->smoke);
    }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (7);
  struct Agent*  a = createAgent();
  // TODO
  
  struct Smoker* s = createSmoker(a);
  
  uthread_t tobacco_counter = uthread_create(tobacco_count, s);
  uthread_t paper_counter   = uthread_create(paper_count, s);
  uthread_t matches_counter = uthread_create(match_count, s);
  uthread_t tobacco_smoker  = uthread_create(tobacco_smoke, s);
  uthread_t paper_smoker    = uthread_create(paper_smoke, s);
  uthread_t match_smoker  = uthread_create(match_smoke, s);
  
  uthread_join (uthread_create (agent, a), 0);
  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}