// Type of the circular queue elements 
 
typedef unsigned long QueueElem; 
typedef struct 
{ 
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
 
void queue_init(CircularQueue **q, unsigned int capacity) // TO DO: change return value 
{ //cenas 2
 *q = (CircularQueue *) malloc(sizeof(CircularQueue)); 
 sem_init(&((*q)->empty), 0, capacity); 
 sem_init(&((*q)->full), 0, 0); 
 pthread_mutex_init(&((*q)->mutex), NULL); 
 (*q)->v = (QueueElem *) malloc(capacity * sizeof(QueueElem)); 
 (*q)->capacity = capacity; 
 (*q)->first = 0; 
 (*q)->last = 0; 
} 
 
//------------------------------------------------------------------------------------------ 
// Inserts 'value' at the tail of queue 'q' 
 
int isFull(CircularQueue *q)
{
	 return((q->last+1)%q->capacity==q->last);
}

void queue_put(CircularQueue *q, QueueElem value) 
{  
	if(isfullqueue(q))
      printf("queue overflow\n");
   else{
      q->last=(q->last+1)%q->capacity;
      q->v[q->last]=x;
      if(q->first == 0) {
         q->first=q->last;
      }
   }
}
 
QueueElem queue_get(CircularQueue *q) 
{ 
	QueueElem data;

  
      data=q->v[q->first];
      if(q->first==q->last)
         q->first=q->last=0;
      else
         q->first=(q->first+1)%q->capacity;
 

   return data;
} 
 
//------------------------------------------------------------------------------------------ 
// Frees space allocated for the queue elements and auxiliary management data 
// Must be called when the queue is no more needed 
 
void queue_destroy(CircularQueue *q) 
{ 
 // TO DO BY STUDENTS 
} 
//
