#include <at/core.h>
#include <stdlib.h>

/*===========================================================================
 * PRIVATE API
 *===========================================================================*/
static AtBucketQueue_uint64_t*
at_bucketqueue_new(){
  return (AtBucketQueue_uint64_t*) malloc(sizeof(AtBucketQueue_uint64_t));
}

static void
at_bucketqueue_set_node_prev(AtBucketQueue_uint64_t* queue, uint64_t elem, uint64_t prev){
  queue->nodes[elem].base.prev = prev;
}
static void
at_bucketqueue_set_node_next(AtBucketQueue_uint64_t* queue, uint64_t elem, uint64_t next){
  queue->nodes[elem].base.next = next;
}
static void
at_bucketqueue_set_bucket_first(AtBucketQueue_uint64_t* queue, uint64_t bucket, uint64_t first){
  queue->buckets.first[bucket] = first;
}
static void
at_bucketqueue_set_bucket_last(AtBucketQueue_uint64_t* queue, uint64_t bucket, uint64_t last){
  queue->buckets.last[bucket] = last;
}
static void
at_bucketqueue_set_state(AtBucketQueue_uint64_t* queue, uint64_t elem, AtBucketQueueNodeState state){
  queue->nodes[elem].base.state = state;
}
static void
at_bucketqueue_get_non_nil(AtBucketQueue_uint64_t* queue){
  uint64_t last = queue->buckets.current;
  do{
    // TODO: for dual formulation (maximization), the bucket should be
    // descending (current--)
    if(queue->optimization == AT_OPTIMIZATION_MAX)
      queue->buckets.current++;
    else{
      if(queue->buckets.current == 0)
        queue->buckets.current += queue->buckets.nbuckets;
      queue->buckets.current--;
    }
    queue->buckets.current %= queue->buckets.nbuckets;
  }while(queue->buckets.first[queue->buckets.current] == UINT64_MAX &&
         queue->buckets.current != last);
}
/*===========================================================================
 * PUBLIC API
 *===========================================================================*/

void
at_bucketqueue_new_with_size(AtBucketQueue_uint64_t** queue_ptr, uint64_t nbuckets, uint64_t nelems, AtOptimization optimization){
  *queue_ptr = at_bucketqueue_new();
  AtBucketQueue_uint64_t* queue = *queue_ptr;
  queue->buckets.first    = malloc(nbuckets * sizeof(uint64_t));
  queue->buckets.last     = malloc(nbuckets * sizeof(uint64_t));
  queue->nodes            = malloc(nelems * sizeof(AtBucketQueueNode_uint64_t));
  queue->buckets.nbuckets = nbuckets;
  queue->nnodes           = nelems;
  queue->optimization     = optimization;
  at_bucketqueue_reset(queue);
}

void
at_bucketqueue_destroy(AtBucketQueue_uint64_t** queue_ptr){
  if(*queue_ptr != NULL){
    AtBucketQueue_uint64_t* queue = *queue_ptr;
    if(queue->buckets.first != NULL) free(queue->buckets.first);
    if(queue->buckets.last  != NULL) free(queue->buckets.last);
    if(queue->nodes         != NULL) free(queue->nodes);
    free(queue);
    *queue_ptr = NULL;
  }
}

void
at_bucketqueue_reset(AtBucketQueue_uint64_t* queue){
  // TODO: for dual formulation (maximization), current begins with maximum value (last one)
  if(queue->optimization == AT_OPTIMIZATION_MAX)
    queue->buckets.current = 0;
  else
    queue->buckets.current = queue->buckets.nbuckets-1;

  queue->buckets.tiebreak = AT_TIEBREAK_FIFO;
  uint64_t i;
  for(i = 0; i < queue->buckets.nbuckets; i++){
    queue->buckets.first[i] = UINT64_MAX;
    queue->buckets.last[i] = UINT64_MAX;
  }
  for(i = 0; i < queue->nnodes; i++)
  {
    at_bucketqueue_set_node_next(queue, i, UINT64_MAX);
    at_bucketqueue_set_node_prev(queue, i, UINT64_MAX);
    at_bucketqueue_set_state(queue,i,AT_BUCKETQUEUE_NODE_NOT_INSERTED);
  }
}

uint8_t
at_bucketqueue_is_empty(AtBucketQueue_uint64_t* queue){
  uint64_t last;
  if(queue->buckets.first[queue->buckets.current] != UINT64_MAX)
    return FALSE;
  at_bucketqueue_get_non_nil(queue);
  return queue->buckets.first[queue->buckets.current] == UINT64_MAX;
}

void
at_bucketqueue_insert(AtBucketQueue_uint64_t* queue, uint64_t bucket, uint64_t elem){
  if(queue->buckets.first[bucket] == UINT64_MAX){
    queue->buckets.first[bucket] = elem;
    queue->nodes[elem].base.prev = UINT64_MAX;
  }else{
    queue->nodes[queue->buckets.last[bucket]].base.next = elem;
    queue->nodes[elem].base.prev = queue->buckets.last[bucket];
  }
  queue->buckets.last[bucket] = elem;
  queue->nodes[elem].base.next = UINT64_MAX;
  at_bucketqueue_set_state(queue,elem,AT_BUCKETQUEUE_NODE_INSERTED);
}

uint64_t
at_bucketqueue_remove(AtBucketQueue_uint64_t* queue){
  uint64_t elem = UINT64_MAX;
  uint64_t next;
  uint64_t prev;
  uint64_t last;

  // Moves to next element or returns empty queue
  if(queue->buckets.first[queue->buckets.current] == UINT64_MAX){
    at_bucketqueue_get_non_nil(queue);
    if(queue->buckets.first[queue->buckets.current] == UINT64_MAX)
      return UINT64_MAX;
  }

  // LIFO: remove the last value
  if(queue->buckets.tiebreak == AT_TIEBREAK_LIFO){
    elem = queue->buckets.last[queue->buckets.current];
    prev = queue->nodes[elem].base.prev;
    if(prev == UINT64_MAX){
      queue->buckets.first[queue->buckets.current] = UINT64_MAX;
      queue->buckets.last[queue->buckets.current]  = UINT64_MAX;
    }else{
      queue->buckets.last[queue->buckets.current] = prev;
      queue->nodes[prev].base.next = UINT64_MAX;
    }
  // FIFO: remove the first value
  }else{
    elem = queue->buckets.first[queue->buckets.current];
    next = queue->nodes[elem].base.next;
    if(next == UINT64_MAX){
      queue->buckets.first[queue->buckets.current] = UINT64_MAX;
      queue->buckets.last[queue->buckets.current] = UINT64_MAX;
    }else{
      queue->buckets.first[queue->buckets.current] = next;
      queue->nodes[next].base.prev = UINT64_MAX;
    }
  }
  at_bucketqueue_set_state(queue, elem, AT_BUCKETQUEUE_NODE_REMOVED);
  return elem;
}

void
at_bucketqueue_remove_elem(AtBucketQueue_uint64_t* queue, uint64_t bucket, uint64_t elem){
  uint64_t prev, next;
  prev = queue->nodes[elem].base.prev;
  next = queue->nodes[elem].base.next;

  // If elem is the first element...
  if(queue->buckets.first[bucket] == elem){
    queue->buckets.first[bucket] = next;
    if(next == UINT64_MAX)
      queue->buckets.last[bucket] = UINT64_MAX;
    else
      queue->nodes[next].base.prev = UINT64_MAX;
  // ... or not
  }else{
    queue->nodes[prev].base.next = next;
    if(next == UINT64_MAX)
      queue->buckets.last[bucket] = prev;
    else
      queue->nodes[next].base.prev = prev;
  }
  at_bucketqueue_set_state(queue,elem,AT_BUCKETQUEUE_NODE_REMOVED);
}

void
at_bucketqueue_update(AtBucketQueue_uint64_t* queue, uint64_t bucket, uint64_t elem, uint64_t to_bucket){
  at_bucketqueue_remove_elem(queue,bucket,elem);
  at_bucketqueue_insert(queue,to_bucket,elem);
}

AtBucketQueueNodeState
at_bucketqueue_get_state(AtBucketQueue_uint64_t* queue, uint64_t elem){
  return queue->nodes[elem].base.state;
}

uint64_t
at_bucketqueue_get_nbuckets(AtBucketQueue_uint64_t* queue){
  return queue->buckets.nbuckets;
}
