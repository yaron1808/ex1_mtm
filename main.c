#include "IsraeliQueue.h"
#include <stdio.h>
int comparison_function_mock(void *obj1, void *obj2) {
    int id1 = *(int *)obj1;
    int id2 = *(int *)obj2;

    return id1 - id2;
}
int mockfriendshipfunction(void* firstObject, void* secondObject){
    int temp = (*(int*)firstObject)+(*(int*)firstObject)+50;
    return temp;
}


int main(){
    // test 1

    /* iterations:
    1 2 3 4 5 6 7 8 9 10 11 12 <-original
    1 12 2 3 4 5 6 7 8 9 10 11
    1 11 12 2 3 4 5 6 7 8 9 10
    1 10 11 12 2 3 4 5 6 7 8 9
    1 9 10 11 12 2 3 4 5 6 7 8
    1 8 9 10 11 12 2 3 4 5 6 7
    1 8 7 9 10 11 12 2 3 4 5 6
    1 8 6 7 9 10 11 12 2 3 4 5
    1 8 5 6 7 9 10 11 12 2 3 4
    1 8 4 5 6 7 9 10 11 12 2 3
    1 8 3 4 5 6 7 9 10 11 12 2
    1 8 3 2 4 5 6 7 9 10 11 12
    8 3 1 2 4 5 6 7 9 10 11 12

     */
    int arr[]={1,2,3,4,5,6,7,8,9,10,11,12};
    FriendshipFunction functions[]={mockfriendshipfunction,NULL};
    IsraeliQueue queue=IsraeliQueueCreate(functions, comparison_function_mock, 0, 0);

    for (int i=0; i<12; i++){
        IsraeliQueueEnqueue(queue, &arr[i]);

    }
    IsraeliQueueImprovePositions(queue);
    //dequeue
    for (int i = 0; i < 12; ++i) {
        int* num = (int*)IsraeliQueueDequeue(queue);
        printf("%d, ", *num);
    }
    printf("\n");
    printf("size: %d\n", IsraeliQueueSize(queue));

}
