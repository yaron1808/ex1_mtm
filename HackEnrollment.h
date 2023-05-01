#ifndef HACKENROLLMENT_H
#define HACKENROLLMENT_H
#include <stdio.h>

#define FRIENDSHIP_THRESHOLD 20
#define RIVALRY_THRESHOLD 0

typedef struct EnrollmentSystem_t* EnrollmentSystem;
/**
 *
 * @param students student file
 * @param courses course file
 * @param hackers hackers file
 * @return NULL if any of the files is NULL or if allocation failed, otherwise a new EnrollmentSystem
 */
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

#endif //HACKENROLLMENT_H
