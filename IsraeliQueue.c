#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#include "IsraeliQueue.h"



/**Error clarification:
 * ISRAELIQUEUE_SUCCESS: Indicates the function has completed its task successfully with no errors.
 * ISRAELIQUEUE_ALLOC_FAILED: Indicates memory allocation failed during the execution of the function.
 * ISRAELIQUEUE_BAD_PARAM: Indicates an illegal parameter was passed.
 * ISRAELI_QUEUE_ERROR: Indicates any error beyond the above.
 * */

typedef struct node
{
    void* data;
    int friends;
    int rivals;
    struct node* next;
    struct node* prev;
}Node_t;

struct IsraeliQueue_t
{
    Node_t* head;
    Node_t* tail;
    FriendshipFunction* friendsFunctions;
    ComparisonFunction compareFunction;
    int friendship_th;
    int rivalry_th;
    int size;
};
typedef struct IsraeliQueue_t IsraeliQueue_t;
void copyFunctionToArray(FriendshipFunction* source, FriendshipFunction* dest);
int friendsArraySize(FriendshipFunction* friendsArr);
IsraeliQueueError IsraeliQueueClassicEnqueue (IsraeliQueue queue, Node_t* node);
bool isFriends(IsraeliQueue queue, Node_t* node1, Node_t* node2);
bool isRivals(IsraeliQueue queue, Node_t* node1, Node_t* node2);
void IsraeliQueueInsertAfterNode(IsraeliQueue queue, Node_t* friend, Node_t* nodeNew);
void addNode(IsraeliQueue queue, Node_t* newNode);
Node_t* dequeueFromTail(IsraeliQueue queue);
/**Creates a new IsraeliQueue_t object with the provided friendship functions, a NULL-terminated array,
 * comparison function, friendship threshold and rivalry threshold. Returns a pointer
 * to the new object. In case of failure, return NULL.*/
IsraeliQueue IsraeliQueueCreate(FriendshipFunction *friendsArray, ComparisonFunction compare, int friendship_th, int rivalry_th)
{
    IsraeliQueue ptrIsraeliQueue = (IsraeliQueue)malloc(sizeof(IsraeliQueue_t));
    int len = friendsArraySize(friendsArray);
    FriendshipFunction* newArr = malloc(sizeof (FriendshipFunction) * (len + 1));
    newArr[len] = NULL;
    copyFunctionToArray(friendsArray,newArr);
    if(ptrIsraeliQueue!=NULL && newArr!=NULL)
    {
        ptrIsraeliQueue->friendsFunctions = newArr;
        ptrIsraeliQueue->compareFunction = compare;
        ptrIsraeliQueue->friendship_th = friendship_th;
        ptrIsraeliQueue->rivalry_th = rivalry_th;
        ptrIsraeliQueue->head = NULL;
        ptrIsraeliQueue->tail = NULL;
        ptrIsraeliQueue->size = 0;
    }
    return ptrIsraeliQueue;
}

int friendsArraySize(FriendshipFunction* friendsArr)
{
    int i = 0;
    while (friendsArr[i]!=NULL)
    {
        i++;
    }
    return i;
}
void copyFunctionToArray(FriendshipFunction* source, FriendshipFunction* dest)
{
    assert(friendsArraySize(source) == friendsArraySize(dest));

    for(int i = 0; i< friendsArraySize(source);i++)
    {
        dest[i] = source[i];
    }
}
/**Returns a new queue with the same elements as the parameter. If the parameter is NULL or any error occured during
 * the execution of the function, NULL is returned.*/
IsraeliQueue IsraeliQueueClone(IsraeliQueue q)
{
    if(q==NULL)
    {
        return NULL;
    }

    IsraeliQueue clone = IsraeliQueueCreate(q->friendsFunctions,q->compareFunction,q->friendship_th,q->rivalry_th);
    if(clone!=NULL)
    {
        Node_t* current = q -> head;
        while (current)
        {
            if (IsraeliQueueClassicEnqueue(clone, current) == ISRAELIQUEUE_ALLOC_FAILED)
            {
                return NULL;
            }
            current = current -> next;
        }
    }

    return clone;

}


/**@param IsraeliQueue: an IsraeliQueue created by IsraeliQueueCreate
 *
 * Deallocates all memory allocated by IsraeliQueueCreate for the object pointed to by
 * the parameter.*/
void IsraeliQueueDestroy(IsraeliQueue q)
{
    if(q==NULL)
    {
        return;
    }
    while(q -> head){
        IsraeliQueueDequeue(q);
    }
    free (q);
}

/**@param IsraeliQueue: an IsraeliQueue in which to insert the item.
 * @param item: an item to enqueue
 *
 * Places the item in the foremost position accessible to it.*/
IsraeliQueueError IsraeliQueueEnqueue(IsraeliQueue queue, void *item)
{
    Node_t* newNode;


    if(queue == NULL || item == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }

    newNode = (Node_t*)malloc(sizeof (Node_t));
    if(newNode == NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    newNode->data = item;
    newNode->friends = 0;
    newNode->rivals = 0;

    addNode(queue,newNode);
    return ISRAELIQUEUE_SUCCESS;
}

bool isRivals(IsraeliQueue queue, Node_t* node1, Node_t* node2)
{
    double avg =0;
    int i = 0;
    while(queue->friendsFunctions[i]!=NULL)
    {
        avg+=queue->friendsFunctions[i](node1->data,node2->data);
        i++;
    }
    avg = avg/i;
    return (!isFriends(queue,node1,node2)) && (avg<queue->rivalry_th);
}
bool isFriends(IsraeliQueue queue, Node_t* node1, Node_t* node2)
{
    int i = 0;
    while (queue->friendsFunctions[i]!=NULL)
    {
        if(queue->friendsFunctions[i](node1->data,node2->data)>queue->friendship_th)
        {
            return true;
        }
        i++;
    }
    return false;
}
/**@param IsraeliQueue: an IsraeliQueue to which the function is to be added
 * @param FriendshipFunction: a FriendshipFunction to be recognized by the IsraeliQueue
 * going forward.
 *
 * Makes the IsraeliQueue provided recognize the FriendshipFunction provided.*/
//IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue, FriendshipFunction);

/**@param IsraeliQueue: an IsraeliQueue whose friendship threshold is to be modified
 * @param friendship_threshold: a new friendship threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int friendship_th)
{
    if (q == NULL) return ISRAELIQUEUE_BAD_PARAM;
    q->friendship_th = friendship_th;
    return ISRAELIQUEUE_SUCCESS;
}

/**@param IsraeliQueue: an IsraeliQueue whose rivalry threshold is to be modified
 * @param friendship_threshold: a new rivalry threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int rivalry_th)
{
    if (q == NULL) return ISRAELIQUEUE_BAD_PARAM;
    q->rivalry_th = rivalry_th;
    return ISRAELIQUEUE_SUCCESS;
}

/**Returns the number of elements of the given queue. If the parameter is NULL, 0
 * is returned.*/
int IsraeliQueueSize(IsraeliQueue queue)
{
    if(queue == NULL)
    {
        return 0;
    }
    return queue->size;
}

/**Removes and returns the foremost element of the provided queue. If the parameter
 * is NULL or a pointer to an empty queue, NULL is returned.*/
void* IsraeliQueueDequeue(IsraeliQueue queue)
{
    if(queue == NULL || queue->size == 0)//
    {
        return NULL;
    }
    Node_t* ptrNode = queue->head;
    void* data = queue->head->data;

    queue->head = queue->head->next;
    free(ptrNode);
    if(queue->head!=NULL)//if there is more than one node in the queue
    {
        queue->head->prev = NULL;
    }
    else//if there is only one node in the queue
    {
        queue->tail = NULL;
    }

    queue->size--;

    return data;
}
//    if(IsraeliQueueSize(queue) == 0 || queue==NULL)
//    {
//        return NULL;
//    }
//    Node_t* ptrNode = queue->head;
//    void* data = queue->head->data;
//
//    queue->head = queue->head->next;
//    free(ptrNode);
//    if(queue->head!=NULL)
//    {
//        queue->head->prev = NULL;
//    }
//    else
//    {
//        queue->tail = NULL;
//    }
//
//    queue->size--;
//
//    return data;



/**@param item: an object comparable to the objects in the IsraeliQueue
 *
 * Returns whether the queue contains an element equal to item. If either
 * parameter is NULL, false is returned.*/
bool IsraeliQueueContains(IsraeliQueue q, void *data)
{
    Node_t* current = q -> head;
    while(current){
        if (q -> compareFunction(data, current -> data))
        {
            return true;
        }
        current = current -> next;
    }
    return false;
}

/**Advances each item in the queue to the foremost position accessible to it,
 * from the back of the queue frontwards.*/
IsraeliQueueError IsraeliQueueImprovePositions(IsraeliQueue queue)
{
    if(queue == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }

    Node_t* curr = NULL;
    IsraeliQueue q2 = IsraeliQueueCreate(queue->friendsFunctions,queue->compareFunction,queue->friendship_th,queue->rivalry_th);
    if(q2 ==NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }

    while(queue->size>0)
    {
        curr = dequeueFromTail(queue);
        addNode(q2,curr);
    }
    queue->head = q2->head;
    queue->tail = q2->tail;
    queue->size = q2->size;
    free(q2);
    return ISRAELIQUEUE_SUCCESS;
}

Node_t* dequeueFromTail(IsraeliQueue queue)
{
    Node_t* tail = queue->tail;
    queue->tail = tail->prev;
    queue->tail->next = NULL;
    queue->size--;
    return tail;
}
void addNode(IsraeliQueue queue, Node_t* newNode)
{

    Node_t* nodeFriend = NULL;
    Node_t* nodeRival = NULL;

    if(queue->size == 0)//first enqueue
    {
        queue->head = newNode;
        queue->tail = newNode;
        newNode->next = NULL;
        newNode->prev = NULL;
        queue->size++;
        return;
    }
    nodeFriend = queue->head;

    while (nodeFriend != NULL)
    {
        if(nodeFriend->friends < FRIEND_QUOTA && isFriends(queue, nodeFriend, newNode))
        {
            nodeRival = nodeFriend->next;
            while(nodeRival != NULL)
            {
                if(nodeFriend->rivals < RIVAL_QUOTA && isRivals(queue, nodeFriend, newNode))
                {
                    nodeRival->rivals++;
                    nodeFriend = nodeRival->next;
                    break;
                }
                nodeRival=nodeRival->next;
            }

            if(nodeRival==NULL)// = can enqueue after friend
            {
                break;
            }
        }
        nodeFriend = nodeFriend->next;
    }


    IsraeliQueueInsertAfterNode(queue,nodeFriend,newNode);
}

/**@param q_arr: a NULL-terminated array of IsraeliQueues
 * @param ComparisonFunction: a comparison function for the merged queue
 *
 * Merges all queues in q_arr into a single new queue, with parameters the parameters described
 * in the exercise. Each queue in q_arr enqueues its head in the merged queue, then lets the next
 * one enqueue an item, in the order defined by q_arr. In the event of any error during execution, return NULL.*/
//IsraeliQueue IsraeliQueueMerge(IsraeliQueue*,ComparisonFunction);


/**@param IsraeliQueue: an IsraeliQueue in which to insert the item.
 * @param node: an node ptr to enqueue
 *
 * Places the item in the tail.*/
IsraeliQueueError IsraeliQueueClassicEnqueue (IsraeliQueue queue, Node_t* node)
{
    Node_t* newNode = (Node_t*)malloc(sizeof (Node_t));
    if(newNode == NULL)
    {
        return ISRAELIQUEUE_ALLOC_FAILED;
    }
    //assert(queue==NULL || node == NULL);
    newNode->data = node -> data;
    newNode->friends = node -> friends;
    newNode->rivals = node -> rivals;
    newNode->next = NULL;
    newNode->prev = queue ->tail;
    if(queue->tail==NULL)
    {
        queue->tail=newNode;
        queue->head = newNode;
    }
    else
    {
        queue->tail->next = newNode;
    }

    queue->tail = newNode;
    queue->size++;
    return ISRAELIQUEUE_SUCCESS;
}
/**
 *
 * @param queue israeliQueue
 * @param friend friend in queue
 * @param nodeNew new node that we want to enqueue
 *
 */
void IsraeliQueueInsertAfterNode(IsraeliQueue queue, Node_t* friend, Node_t* nodeNew)
{

    if(friend == queue->tail || friend ==NULL)// have no friends or friend is tail
    {
        nodeNew->prev = queue->tail;
        queue->tail->next = nodeNew;
        queue->tail = nodeNew;
        nodeNew->next = NULL;
        queue->size++;
        return;
    }

    nodeNew->next = friend->next;
    nodeNew->prev = friend;
    friend->next = nodeNew;
    queue->size++;

}

