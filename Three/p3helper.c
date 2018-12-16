/* p3helper.c
 * Taylor Ewertz
 * Program 3
 * CS570
 * Carroll
 * SDSU
 * Fall 2018
 * 11/02/18
 *
 * This is the only file you may change. (In fact, the other files should
 * be symbolic links to:
 *   ~cs570/Three/p3main.c
 *   ~cs570/Three/p3.h
 *   ~cs570/Three/Makefile
 *   ~cs570/Three/CHK.h    )
 *
 */
#include "p3.h"

/* You may put any semaphore (or other global) declarations/definitions here: */


int scount, jcount;
static sem_t shooterlock, joggerlock;

/* General documentation for the following functions is in p3.h
 * Here you supply the code:
 */
void initstudentstuff(void) {

	CHK(sem_init(&shooterlock, 0, 1));
	CHK(sem_init(&joggerlock, 0, 1));
}

void prolog(int kind) {
	
	if(kind == JOGGER){

		// locks the shooters out if there are any joggers in the gym.		
		
		CHK(sem_wait(&joggerlock));
		jcount++;
		if(jcount == 1)
			CHK(sem_wait(&shooterlock));
		CHK(sem_post(&joggerlock));

	}
	else if(kind == SHOOTER){

		// locks the joggers out if there are any shooters in the gym.

		CHK(sem_wait(&shooterlock));
		scount++;
		if(scount == 1)
			CHK(sem_wait(&joggerlock));
		CHK(sem_post(&shooterlock));
	}
	else{
		perror("Customer needs to be either a SHOOTER or JOGGER!\n");
	}

}

void epilog(int kind) {
	
	if(kind == JOGGER){

		// allows the shooters in the gym if there aren't any joggers in it

		CHK(sem_wait(&joggerlock));
		jcount--;		
		if(jcount == 0)
			CHK(sem_post(&shooterlock));
		CHK(sem_post(&joggerlock));

	}
	else if(kind == SHOOTER){

		// allows the joggers in the gym if there aren't any shooters in it.
		
		CHK(sem_wait(&shooterlock));
		scount--;
		if(scount == 0)
			CHK(sem_post(&joggerlock));
		CHK(sem_post(&shooterlock));

	}
	else{
		perror("Customer needs to be either a SHOOTER or JOGGER!\n");
	}

}  

