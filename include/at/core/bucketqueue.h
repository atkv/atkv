/**
 ** This file is part of the atkv project.
 ** Copyright 2016 Anderson Tavares <nocturne.pe@gmail.com>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#ifndef AT_ARRAY_BUCKETQUEUE_H
#define AT_ARRAY_BUCKETQUEUE_H
#include <at/core.h>
G_BEGIN_DECLS

typedef enum{
  AT_BUCKETQUEUE_NODE_NOT_INSERTED,
  AT_BUCKETQUEUE_NODE_INSERTED,
  AT_BUCKETQUEUE_NODE_REMOVED,
}AtBucketQueueNodeState;
typedef enum{
  AT_TIEBREAK_FIFO,
  AT_TIEBREAK_LIFO
}AtTieBreakPolicy;
typedef enum{
  AT_OPTIMIZATION_MIN, // Minimization Problems (Primal formulation)
  AT_OPTIMIZATION_MAX, // Maximization Problems (Dual formulation)
}AtOptimization;



typedef struct _AtBucketQueueNodeBase{
  uint64_t prev;
  uint64_t next;
  AtBucketQueueNodeState state;
}AtBucketQueueNodeBase;

typedef struct _AtBucketQueueNode_uint64_t{
  struct _AtBucketQueueNodeBase base;
  uint64_t value;
}AtBucketQueueNode_uint64_t;

typedef struct AtBucketQueueList_uint64_t{
  uint64_t*        first;
  uint64_t*        last;
  uint64_t         nbuckets;
  uint64_t         current;
  AtTieBreakPolicy tiebreak;
}AtBucketQueueList_uint64_t;

typedef struct AtBucketQueue_uint64_t{
  AtBucketQueueList_uint64_t  buckets;
  AtBucketQueueNode_uint64_t* nodes;
  uint64_t nnodes;
  AtOptimization optimization;
}AtBucketQueue_uint64_t;

#define AtBucketQueue(type) AtBucketQueue_##type

/**
 * @brief at_bucketqueue_new_with_size
 * @param queue
 * @param nbuckets
 * @param nelems
 */
void
at_bucketqueue_new_with_size(AtBucketQueue_uint64_t** queue,
                             uint64_t nbuckets,
                             uint64_t nelems,
                             AtOptimization optimization);
/**
 * @brief at_bucketqueue_destroy
 * @param queue
 */
void
at_bucketqueue_destroy(AtBucketQueue_uint64_t** queue);
/**
 * @brief at_bucketqueue_reset
 * @param queue
 */
void
at_bucketqueue_reset(AtBucketQueue_uint64_t* queue);
/**
 * @brief at_bucketqueue_empty
 * @param queue
 */
uint8_t
at_bucketqueue_is_empty(AtBucketQueue_uint64_t* queue);
/**
 * @brief at_bucketqueue_insert
 * @param queue
 * @param bucket
 * @param elem
 */
void
at_bucketqueue_insert(AtBucketQueue_uint64_t* queue, uint64_t bucket, uint64_t elem);
/**
 * @brief at_bucketqueue_remove
 * @param queue
 * @return
 */
uint64_t
at_bucketqueue_remove(AtBucketQueue_uint64_t* queue);
/**
 * @brief at_bucketqueue_remove_elem
 * @param queue
 * @param bucket
 * @param elem
 */
void
at_bucketqueue_remove_elem(AtBucketQueue_uint64_t* queue, uint64_t bucket, uint64_t elem);
/**
 * @brief at_bucketqueue_update
 * @param queue
 * @param bucket
 * @param elem
 * @param to_bucket
 */
void
at_bucketqueue_update(AtBucketQueue_uint64_t* queue, uint64_t bucket, uint64_t elem, uint64_t to_bucket);

/**
 * @brief at_bucketqueue_get_state
 * @param queue
 * @param elem
 * @return
 */
AtBucketQueueNodeState
at_bucketqueue_get_state(AtBucketQueue_uint64_t* queue, uint64_t elem);

/**
 * @brief at_bucketqueue_get_nbuckets
 * @param queue
 * @return
 */
uint64_t
at_bucketqueue_get_nbuckets(AtBucketQueue_uint64_t* queue);

G_END_DECLS
#endif
