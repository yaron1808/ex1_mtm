#include "IsraeliQueue.h"
#include "HackEnrollment.h"
#include <stdio.h>

int friend1(void* a, void* b){
    int aa = *(int*)a;
    int bb = *(int*)b;
    return aa+bb;

}
int friend2(void* a, void* b)
{
    int aa = *(int*)a;
    int bb = *(int*)b;
    return aa;
}
int friend3(void* a, void* b)
{
    int aa = *(int*)a;
    int bb = *(int*)b;
    if(aa == bb){
        return 10;
    }

    return 4;
}

int friend4(void* a, void* b){
    int aa = *(int*)a;
    int bb = *(int*)b;
    return bb;

}
int friend5(void* a, void* b){
    int aa = *(int*)a;
    int bb = *(int*)b;
    return bb-aa;

}
int friend6(void* a, void* b){
    int aa = *(int*)a;
    int bb = *(int*)b;
    return bb-2*aa;

}

int compare(void* a, void* b){
    int aa = *(int*)a;
    int bb = *(int*)b;
    if(aa == bb){
        return 1;
    }

    return 0;
}

int main()
{
    FriendshipFunction friendsArray[7];
    friendsArray[0] = &friend1;
    friendsArray[1] = &friend2;
    friendsArray[2] = &friend3;
    friendsArray[3] = &friend4;
    friendsArray[4] = &friend5;
    friendsArray[5] = &friend6;
    friendsArray[6] = NULL;

    FriendshipFunction arrfriends1 [3];
    arrfriends1 [0] = &friend1;
    arrfriends1 [1] = &friend2;
    arrfriends1 [2] = NULL;
    FriendshipFunction arrfriends2 [2];
    arrfriends2 [0] = &friend3;
    arrfriends2 [1] = NULL;
    FriendshipFunction arrfriends3 [4];
    arrfriends3 [0] = &friend4;
    arrfriends3 [1] = &friend5;
    arrfriends3 [2] = &friend6;
    arrfriends3 [3] = NULL;
    ComparisonFunction compare = &compare;


    IsraeliQueue queue1 = IsraeliQueueCreate(arrfriends1,compare,13,7);
    int nums1[10] = {40,-10,11, 1,8,6,13,14,15,16};
    for (int i = 0; i < 10; ++i) {
        IsraeliQueueEnqueue(queue1, &nums1[i]);
    }
    printf("queue1 size = %d\n", IsraeliQueueSize(queue1));
    printIntObjectsOfQueue(queue1);


    IsraeliQueue queue2 = IsraeliQueueCreate(arrfriends2,compare,100,76);
    int nums2[4] = {-2,9,8, 10};
    for (int i = 0; i < 4; ++i) {
        IsraeliQueueEnqueue(queue2, &nums2[i]);
    }
    printf("queue2 size = %d\n", IsraeliQueueSize(queue2));
    printIntObjectsOfQueue(queue2);


    IsraeliQueue queue3 = IsraeliQueueCreate(arrfriends3,compare,8102,1);
    int nums3[7] = {3,6,9, 10, 11, 12, 0};
    for (int i = 0; i < 7; ++i) {
        IsraeliQueueEnqueue(queue3, &nums3[i]);
    }
    printf("queue3 size = %d\n", IsraeliQueueSize(queue3));
    printIntObjectsOfQueue(queue3);


    IsraeliQueue arrIsraeliQueue[4];
    arrIsraeliQueue[0] = queue1;
    arrIsraeliQueue[1] = queue2;
    arrIsraeliQueue[2] = queue3;
    arrIsraeliQueue[3] = NULL;


    IsraeliQueue queueMerge = IsraeliQueueMerge(arrIsraeliQueue, compare);
    printf("queueMerge size = %d\n", IsraeliQueueSize(queueMerge));
    printIntObjectsOfQueue(queueMerge);
//    printFriend_th(queueMerge);
    printSizeOfFriendFunction(queueMerge);
//    IsraeliQueueAddFriendshipMeasure(queueMerge, &friend4);
   if(checkingIfThisIsTheRightArray(queueMerge, friendsArray))
   {
       printf("true");
   }
   else
   {
       printf("false");

   }
//    IsraeliQueueUpdateFriendshipThreshold(queueMerge,5);
//    IsraeliQueueUpdateRivalryThreshold(queueMerge,0);
//    int newObject = 8;
//    IsraeliQueueEnqueue(queueMerge, &newObject);
//    int newObject2 = 8;
//    IsraeliQueueEnqueue(queueMerge, &newObject2);
//    printIntObjectsOfQueue(queueMerge);
    return 0;
}
//FILE* courses = fopen("C:\\Users\\lasko\\CLionProjects\\ex1_mtm\\ExampleTest\\courses.txt", "r");
//FILE* students = fopen("C:\\Users\\lasko\\CLionProjects\\ex1_mtm\\ExampleTest\\students.txt", "r");
//FILE* hackers = fopen("C:\\Users\\lasko\\CLionProjects\\ex1_mtm\\ExampleTest\\hackers.txt", "r");
//FILE* queues = fopen("C:\\Users\\lasko\\CLionProjects\\ex1_mtm\\ExampleTest\\queues.txt", "r");

//if(students == NULL || courses == NULL || hackers == NULL){
//return 0;
//}
//EnrollmentSystem sys = createEnrollment(students, courses, hackers);
//readEnrollment(sys, queues);
//fclose(students);
//fclose(courses);
//fclose(hackers);
//fclose(queues);