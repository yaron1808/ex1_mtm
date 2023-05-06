#include "IsraeliQueue.h"
#include "HackEnrollment.h"
#include <stdio.h>

int friend1(void* a, void* b){
    int aa = *(int*)a;
    int bb = *(int*)b;
    return aa;

}

int friend2(void* a, void* b){
    int aa = *(int*)a;
    int bb = *(int*)b;
    return bb;

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
   FILE* courses = fopen("C:\\Users\\lasko\\CLionProjects\\ex1_mtm\\ExampleTest\\courses.txt", "r");
   FILE* students = fopen("C:\\Users\\lasko\\CLionProjects\\ex1_mtm\\ExampleTest\\students.txt", "r");
   FILE* hackers = fopen("C:\\Users\\lasko\\CLionProjects\\ex1_mtm\\ExampleTest\\hackers.txt", "r");
   FILE* queues = fopen("C:\\Users\\lasko\\CLionProjects\\ex1_mtm\\ExampleTest\\queues.txt", "r");

    if(students == NULL || courses == NULL || hackers == NULL){
        return 0;
    }
    EnrollmentSystem sys = createEnrollment(students, courses, hackers);
    readEnrollment(sys, queues);
    fclose(students);
    fclose(courses);
    fclose(hackers);
    fclose(queues);
    return 0;
}
