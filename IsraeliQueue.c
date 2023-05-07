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
    bool improved;
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
Node_t* removeFromTail(IsraeliQueue queue);
int FindTheFriendship_thForTheMergeFnction (int friendship_th, int numOfQueue);
int FindTheRivalry_thForTheMergeFnction (int rivalry_th, int numOfQueue);



/**Creates a new IsraeliQueue_t object with the provided friendship functions, a NULL-terminated array,
 * comparison function, friendship threshold and rivalry threshold. Returns a pointer
 * to the new object. In case of failure, return NULL.*/
IsraeliQueue IsraeliQueueCreate
(FriendshipFunction* friendsArray, ComparisonFunction compare, int friendship_th, int rivalry_th)
{
    if(friendsArray==NULL || compare==NULL)
    {
        return NULL;
    }
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
/**
 *
 * @param friendsArr array of friendship functions
 * @return size of the array
 */
int friendsArraySize(FriendshipFunction* friendsArr)
{
    int i = 0;
    while (friendsArr[i]!=NULL)
    {
        i++;
    }
    return i;
}
/**
 *
 * @param source source array
 * @param dest destination array
 */
void copyFunctionToArray(FriendshipFunction* source, FriendshipFunction* dest)
{
    assert(friendsArraySize(source) <= friendsArraySize(dest));

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

    IsraeliQueue clone=
    IsraeliQueueCreate(q->friendsFunctions,q->compareFunction,q->friendship_th,q->rivalry_th);
    if(clone!=NULL)
    {
        Node_t* current = q -> head;
        while (current)
        {
            if (IsraeliQueueClassicEnqueue(clone, current) == ISRAELIQUEUE_ALLOC_FAILED)
            {
                free(clone);
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
    free(q->friendsFunctions);
    free(q);

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
/**
 *
 * @param queue an IsraeliQueue
 * @param node1 node in the queue
 * @param node2 another node in the queue
 * @return true if node1 and node2 are rivals, false otherwise
 */
bool isRivals(IsraeliQueue queue, Node_t* node1, Node_t* node2)
{
    double avg = 0;
    int i = 0;
    while(queue->friendsFunctions[i]!=NULL)
    {
        int a = queue->friendsFunctions[i](node1->data,node2->data);
        if(a > queue->friendship_th)
        {
            return false;
        }
        avg+=a;
        i++;
    }
    avg = avg/i;
    return  (avg < queue->rivalry_th);
}
/**
 *
 * @param queue an IsraeliQueue
 * @param node1 node in the queue
 * @param node2 another node in the queuen
 * @return true if node1 and node2 are friends, false otherwise
 */
bool isFriends(IsraeliQueue queue, Node_t* node1, Node_t* node2)
{
    int i = 0;
    while (queue->friendsFunctions[i]!=NULL)
    {
        if(queue->friendsFunctions[i](node1->data,node2->data) > queue->friendship_th)
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
IsraeliQueueError IsraeliQueueAddFriendshipMeasure(IsraeliQueue queue, FriendshipFunction newFriendFunction)
{
    if (queue == NULL || newFriendFunction == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    int i = 0;
    while (queue->friendsFunctions[i]!=NULL)
    {
        i++;
    }
    FriendshipFunction* newArray = (FriendshipFunction*) malloc((i+2) * sizeof(FriendshipFunction)); //allocate i+2 for NULL and the newFriendFunction
    int j = 0;
    while (queue->friendsFunctions[j]!=NULL)
    {
        newArray[j] = queue->friendsFunctions[j];
        j++;
    }
    newArray[i] = newFriendFunction;
    newArray[i+1] = NULL;
    free(queue->friendsFunctions);
    queue->friendsFunctions = newArray;
    return ISRAELIQUEUE_SUCCESS;
}

/**@param IsraeliQueue: an IsraeliQueue whose friendship threshold is to be modified
 * @param friendship_threshold: a new friendship threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateFriendshipThreshold(IsraeliQueue q, int friendship_th)
{
    if (q == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
    q->friendship_th = friendship_th;
    return ISRAELIQUEUE_SUCCESS;
}

/**@param IsraeliQueue: an IsraeliQueue whose rivalry threshold is to be modified
 * @param friendship_threshold: a new rivalry threshold for the IsraeliQueue*/
IsraeliQueueError IsraeliQueueUpdateRivalryThreshold(IsraeliQueue q, int rivalry_th) {
    if (q == NULL)
    {
        return ISRAELIQUEUE_BAD_PARAM;
    }
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
    if(queue == NULL || queue->size == 0)
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
    Node_t* curr = queue->head;
    while (curr!=NULL)
    {
        curr->improved = false;
        curr = curr->next;
    }
    curr = queue->tail;
    while (curr!=NULL)
    {
        if(curr->improved == false)
        {
            Node_t* ptrNode = removeFromTail(queue);
            addNode(queue,ptrNode);
            curr->improved = true;
        }
        curr = curr->prev;
    }
    return ISRAELIQUEUE_SUCCESS;
}

Node_t* removeFromTail(IsraeliQueue queue)
{
    if(queue->head == NULL)//if the queue is empty
    {
        return NULL;
    }

    else if(queue->head == queue->tail)//if there is only one node in the queue
    {
        Node_t* ptrNode = queue->head;
        queue->head = NULL;
        queue->tail = NULL;
        queue->size--;
        return ptrNode;
    }

    else//if there is more than one node in the queue
    {
        Node_t* curr = queue->tail->prev;
        //while(curr->next != queue->tail)//find the node before the tail
        //{
        //    curr = curr->next;
        //}
        Node_t* ptrNode = queue->tail;
        queue->tail = curr;
        queue->tail->next = NULL;
        queue->size--;
        return ptrNode;
    }





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
                if(nodeRival->rivals < RIVAL_QUOTA && isRivals(queue, nodeRival, newNode))
                {
                    nodeRival->rivals++;
                    nodeFriend = nodeRival;
                    break;
                }
                nodeRival=nodeRival->next;
            }

            if(nodeRival==NULL)// = can enqueue after friend
            {
                nodeFriend->friends++;
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
IsraeliQueue IsraeliQueueMerge(IsraeliQueue* israeliQueueArray ,ComparisonFunction newCompareFunction)
{
    if (israeliQueueArray == NULL || newCompareFunction == NULL)
    {
        return NULL;
    }
    int numOfQueue = 0;
    int friendship_th = 0;
    int rivalry_th = 1;
    int sizeOfFriendshipArray = 0;
    while(israeliQueueArray[numOfQueue] != NULL)
    {
        friendship_th = friendship_th + israeliQueueArray[numOfQueue] -> friendship_th;
        rivalry_th = rivalry_th * israeliQueueArray[numOfQueue] -> rivalry_th;
        numOfQueue++;
        sizeOfFriendshipArray = sizeOfFriendshipArray + friendsArraySize (israeliQueueArray[numOfQueue] -> friendsFunctions);

    }
    friendship_th = FindTheFriendship_thForTheMergeFnction (friendship_th, numOfQueue);
    rivalry_th = FindTheRivalry_thForTheMergeFnction (rivalry_th,numOfQueue);
    FriendshipFunction* mergeArray = malloc(sizeof (FriendshipFunction) * (sizeOfFriendshipArray + 1));
    if (mergeArray == NULL)
    {
        return NULL;
    }
    FriendshipFunction* currentArray = mergeArray;
    for (int i = 0; i < numOfQueue; i++)
    {
        copyFunctionToArray(israeliQueueArray[i] -> friendsFunctions,currentArray);
        currentArray = & (currentArray[friendsArraySize(israeliQueueArray[i] -> friendsFunctions)]);
    }
    mergeArray[sizeOfFriendshipArray] = NULL;
    IsraeliQueue IsraeliQueueMerge = IsraeliQueueCreate(mergeArray,newCompareFunction,friendship_th, rivalry_th);
    bool enter = true;
    while(enter)
    {
        enter = false;
        for (int i = 0; i < numOfQueue; i++)
        {
            if (israeliQueueArray[i]->head == NULL)
            {
                continue;
            }
            if (IsraeliQueueClassicEnqueue(IsraeliQueueMerge, israeliQueueArray[i]->head) != ISRAELIQUEUE_SUCCESS)
            {
                return NULL;
            }
            IsraeliQueueDequeue(israeliQueueArray[i]);
            enter = true;
        }
    }
    for (int i = 0; i < numOfQueue; i++)
    {
        IsraeliQueueDestroy(israeliQueueArray[i]);
    }
    return IsraeliQueueMerge;
}
/**
 *
 * @param friendship_th: the sum of the friendship_th of all the israeliqueue in the array
 * @param numOfQueue: the size of the israeliqueue array
 * @return
 */
int FindTheFriendship_thForTheMergeFnction (int friendship_th, int numOfQueue)
{
    if (friendship_th % numOfQueue == 0)
    {
        friendship_th = friendship_th/numOfQueue;
    }
    else
    {
        friendship_th = friendship_th/numOfQueue + 1;
    }
    return friendship_th;
}
/**
 *
 * @param rivalry_th: the multiplication of the rivalry_th of all the israeliqueue in the array
 * @param numOfQueue: the size of the israeliqueue array
 * @return
 */
int FindTheRivalry_thForTheMergeFnction (int rivalry_th, int numOfQueue)
{
    if (rivalry_th < 0)
    {
        rivalry_th = rivalry_th * -1;
    }

    int x = 0;
    int exponent = 0;
    while(exponent < rivalry_th)
    {
        x++;
        exponent = 1;
        for(int i = 0; i < numOfQueue; i++){
            exponent = exponent * x;
        }
    }
    return x;
}

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
        Node_t *ptrNode = queue->tail;
        nodeNew->prev = queue->tail;
        queue->tail = nodeNew;
        ptrNode->next = nodeNew;
        nodeNew->next = NULL;
        queue->size++;
        return;
    }

    friend->next->prev = nodeNew;
    nodeNew->next = friend->next;
    nodeNew->prev = friend;
    friend->next = nodeNew;
    queue->size++;

}

