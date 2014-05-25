#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include "Queue.c"
#define QUEUE_SIZE 10

void *filter(void *arg);
void *start(void *arg);

QueueElem* primes;
int pos;
int N;
pthread_mutex_t mutex;
sem_t final;
int end;

int compare(const void * a, const void * b) {
	return (*(int*) a - *(int*) b);
}

void insertPrime(QueueElem newPrime) {
	pthread_mutex_lock(&mutex);
	primes[pos++] = newPrime;
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[]) {

	int i;

	if (argc != 2) {
		printf("Usage: ./primes n\n");
		exit(E2BIG);
	}
	for (i = 0; argv[1][i] != '\0'; ++i) {
		if(argv[1][i]<'0' || argv[1][i]>'9')
		{
			printf("Number must be a value!\n");
			exit(EINVAL);
		}
	}
	N = strtol(argv[1], NULL, 0);
	if(N<2)
	{
		printf("Number must be atleast 2!(no primes lower than that)\n");
		exit(ERANGE);
	}
	int nprimes = ceil(1.2 * (N / log(N))) + 1;
	sem_init(&final, 1, end);
	pthread_mutex_init(&mutex, NULL);

	primes = (QueueElem*) malloc(sizeof(QueueElem) * nprimes);
	end = pos = 0;

	pthread_t f;
	char *msg1 = "First Filter";

	pthread_create(&f, NULL, start, (void *) msg1);

	sem_wait(&final);
	sem_destroy(&final);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);

	qsort(primes, pos, sizeof(QueueElem), compare);

	printf("Primes:\n");
	for (i = 0; i < pos; ++i)
		printf("%lu\n", primes[i]);

	printf("That makes a total of %d prime(s)\n", i);
	free(primes);
	return 0;

}

void *start(void *arg) {

	insertPrime(2);
	if (N > 2) {
		pthread_t f;

		CircularQueue *q;
		queue_init(&q, QUEUE_SIZE);

		pthread_create(&f, NULL, filter, q);

		QueueElem i = 3;
		while (i <= N) {
			queue_put(q, i);
			i += 2;
		}
		queue_put(q, 0);
	} else {
		sem_post(&final);
	}

	return NULL;
}

void *filter(void *arg) {
	QueueElem newPrime = queue_get(arg);
	if (newPrime > (QueueElem) (sqrt(N))) {
		insertPrime(newPrime);
		while ((newPrime = queue_get(arg)) != 0) {
			insertPrime(newPrime);
		}

		queue_destroy(arg);
		sem_post(&final);

	} else {
		insertPrime(newPrime);
		CircularQueue *q;
		queue_init(&q, QUEUE_SIZE);

		pthread_t f;
		pthread_create(&f, NULL, filter, q);
		QueueElem nonMult;
		while ((nonMult = queue_get(arg)) != 0) {
			if ((nonMult % newPrime) != 0) {
				queue_put(q, nonMult);
			}
		}
		queue_put(q, 0);
		queue_destroy(arg);
	}
	return NULL;
}
