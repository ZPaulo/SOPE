#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long QueueElem;
typedef struct {
	QueueElem *v; // pointer to the queue buffer unsigned
	int capacity; // queue capacity
	unsigned int first; // head of the queue
	unsigned int last; // tail of the queue
	sem_t empty; // semaphores and mutex for implementing the
	sem_t full; // producer-consumer paradigm
	pthread_mutex_t mutex;
} CircularQueue;

//------------------------------------------------------------------------------------------
// Allocates space for circular queue 'q' having 'capacity' number of elements
// Initializes semaphores & mutex needed to implement the producer-consumer paradigm
// Initializes indexes of the head and tail of the queue
// TO DO BY STUDENTS: ADD ERROR TESTS TO THE CALLS & RETURN a value INDICATING (UN)SUCESS

int queue_init(CircularQueue **q, unsigned int capacity)
{
	*q = (CircularQueue *) malloc(sizeof(CircularQueue));

	int error;
	if(error = sem_init(&((*q)->empty), 0, capacity))
		return error;

	if(error = sem_init(&((*q)->full), 0, 0))
		return error;

	if(error = pthread_mutex_init(&((*q)->mutex), NULL))
		return error;

	(*q)->v = (QueueElem *) malloc(capacity * sizeof(QueueElem));
	(*q)->capacity = capacity;
	(*q)->first = 0;
	(*q)->last = 0;
	
	return 0;
}

void queue_put(CircularQueue *q, QueueElem value) {

	sem_wait(&(q->empty));
	pthread_mutex_lock(&(q->mutex));
	q->v[q->last++] = value;
	if (q->last == q->capacity)
		q->last = 0;
	pthread_mutex_unlock(&(q->mutex));
	sem_post(&(q->full));
}

QueueElem queue_get(CircularQueue *q) {
	QueueElem data;

	sem_wait(&(q->full));
	pthread_mutex_lock(&(q->mutex));
	data = q->v[q->first++];
	if (q->first == q->capacity)
		q->first = 0;

	pthread_mutex_unlock(&(q->mutex));
	sem_post(&(q->empty));
	return data;
}

void queue_destroy(CircularQueue *q) {
	pthread_mutex_lock(&(q->mutex));
	free(q->v);

	sem_destroy(&(q->empty));
	sem_destroy(&(q->full));
	pthread_mutex_unlock(&(q->mutex));
	pthread_mutex_destroy(&(q->mutex));
}
