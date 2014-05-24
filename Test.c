#include "Queue.c"
#define QUEUE_SIZE 10 

void *func1(void *arg);
void *func2(void *arg);

CircularQueue *q; 


void main(int argc, char *argv[])
{
	pthread_t thrd1,thrd2;
	int t1,t2;
	char *msg1 = "First Thread";
	char *msg2 = "Second thread";
	queue_init(&q,QUEUE_SIZE); 

	t1 = pthread_create(&thrd1,	NULL, func1,(void *) msg1);
	t2 = pthread_create(&thrd2,	NULL, func2,(void *) msg2);

pthread_join(thrd1,NULL);
pthread_join(thrd2,NULL);

return 0;
}

void *func1(void *arg)
{
	printf("Inserting\n");
	queue_put(q, (QueueElem) 1);
	queue_put(q, (QueueElem) 2);  
}
void *func2(void *arg);
{
	printf("%d\n", queue_get(q));
	printf("%d\n", queue_get(q));
}