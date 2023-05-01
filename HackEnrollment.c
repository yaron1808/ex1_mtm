#include "HackEnrollment.h"
#include <stdlib.h>
#include "IsraeliQueue.h"

struct EnrollmentSystem_t{
    FILE* studentsFile;
    FILE* coursesFile;
    FILE* hackersFile;
};

typedef struct EnrollmentSystem_t EnrollmentSystem_t;
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){
    if(students == NULL || courses == NULL || hackers == NULL){
        return NULL;
    }
    EnrollmentSystem system = malloc(sizeof(EnrollmentSystem_t));
    if(system == NULL){
        return NULL;
    }
    system->studentsFile = students;
    system->coursesFile = courses;
    system->hackersFile = hackers;
    return system;
}

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    if(sys ==NULL || queues == NULL)
    {
        return NULL;
    }

    return NULL;

}