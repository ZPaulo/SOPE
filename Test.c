#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include "Queue.c"


void *filter(void *arg);
void *start(void *arg);

int* primes;
int pos;
int N;

int compare(const void * a, const void * b) 
{
 return (*(int*) a - *(int*) b);
}

int main(int argc, char *argv[]) 
{
 N = strtol(argv[1], NULL, 0);
 int nprimes = N * log(n);

 primes = (int*) malloc(sizeof(int) * nprimes);
 pos = 0;

 pthread_t f;
 char *msg1 = "First Filter";

 pthread_create(&f, NULL, start, (void *) msg1);

 qsort(primes, nprimes, sizeof(int), compare);

 while (primes != '\0') 
 {
  printf("%d\n", *primes++);
 }
 free(primes);

 return 0;

}

void *start(void *arg) 
{

 primes[pos++] = 2;
 if (N > 2) 
 {
  pthread_t f;

  CircularQueue *q;
  queue_init(&q, QUEUE_SIZE);

  pthread_create(&f, NULL, filter, q);

  int i = 3;
  while (i < N) {
   queue_put(q, i);
   i += 2;
  }
  queue_put(q, 0);

  //queue_destroy(q); acho que fica melhor a nova thread destruir a queue que lhe passam, assim esta so e destruida quando ja ninguem lhe ta a fazer acessos
 }
 else
 {

 }

 return NULL;
}

void *filter(void *arg) 
{
	QueueElem newPrime = queue_get(arg);
	if(newPrime > (int) (sqrt(N)))
	{
		 primes[pos++] = newPrime;
		 newPrime = queue_get(arg);
		 if(newPrime != 0)
		 {
		 	do
		 	{
		 		primes[pos++] = newPrime;
		 		newPrime = queue_get(arg);

		 	}while(newPrime != 0);
		 }
		 	//semaforo
		 
	}
	else
	{
		CircularQueue *q; 
		queue_init(&q, QUEUE_SIZE);
		

		pthread_t f;
  		pthread_create(&f, NULL, filter, q);

		QueueElem nonMult = queue_get(arg);
  		if(nonMult != 0)
  		{
  			do
  			{
  				if( nonMult % newPrime != 0)
  					queue_put(q, nonMult);

  				nonMult = queue_get(arg);
  			}while(nonMult != 0);

  		}
  		queue_destroy(arg);
  		queue_put(q, 0);
  		
  		primes[pos++] = newPrime;
	}
 	return NULL;
}