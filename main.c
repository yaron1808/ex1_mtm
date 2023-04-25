#include "IsraeliQueue.h"
#include <stdio.h>

int main()
{
   int a = 100;
   int b = 200;
   int c = 300;


   IsraeliQueue iq = IsraeliQueueCreate(NULL,NULL,8,9);
   printf("queue size = %d\n", IsraeliQueueSize(iq));

    IsraeliQueueEnqueue(iq,&a);
    IsraeliQueueEnqueue(iq,&b);
    IsraeliQueueEnqueue(iq,&c);
    printf("queue size = %d\n", IsraeliQueueSize(iq));
    int* p;
    p = IsraeliQueueDequeue(iq);
    printf("first element = %d\n",*p);
    p = IsraeliQueueDequeue(iq);
    printf("second element = %d\n",*p);
    p = IsraeliQueueDequeue(iq);
    printf("third element = %d\n",*p);
    printf("queue size = %d\n", IsraeliQueueSize(iq));


}