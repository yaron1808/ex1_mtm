#include "IsraeliQueue.h"
#include <stdio.h>
int friend1(void* a, void* b)
{
    int aa = *(int*)a;
    int bb = *(int*)b;
    return aa+bb;
}
int friend2(void* a, void* b)
{
    int aa = *(int*)a;
    int bb = *(int*)b;
    return aa-bb;
}

int main()
{

    FriendshipFunction arr[3];
    arr[0] = &friend1;
    arr[1] = &friend2;
    arr[2] = NULL;
    int nums[10] = {1,2,3,4,5,6,7,8,9,10};

    IsraeliQueue q = IsraeliQueueCreate(arr, NULL, 5, 3);
    for (int i = 0; i < 10; ++i) {
        IsraeliQueueEnqueue(q, &nums[i]);
    }
    printf("size: %d\n", IsraeliQueueSize(q));
    //dequeue
    for (int i = 0; i < 10; ++i) {
        int* num = (int*)IsraeliQueueDequeue(q);
        printf("%d, ", *num);
    }
    printf("\n");

    for (int i = 0; i < 10; ++i) {
        IsraeliQueueEnqueue(q, &nums[i]);
    }
    printf("size: %d\n", IsraeliQueueSize(q));
    IsraeliQueueImprovePositions(q);
    //dequeue
    for (int i = 0; i < 10; ++i) {
        int* num = (int*)IsraeliQueueDequeue(q);
        printf("%d, ", *num);
    }
        printf("\n");

}