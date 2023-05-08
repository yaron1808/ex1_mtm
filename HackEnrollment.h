#ifndef HACKENROLLMENT_H
#define HACKENROLLMENT_H
#include <stdio.h>
#include <stdbool.h>

#define FRIENDSHIP_THRESHOLD 20
#define RIVALRY_THRESHOLD 0
#define FRIENDS 20
#define RIVALS (-20)
#define NO_RELATION 0
#define MIN_COURSES 2
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

void hackEnrollment(EnrollmentSystem sys, FILE* out);

void setCaseSensitivity(EnrollmentSystem sys, bool caseSensitive);

void destroyEnrollment(EnrollmentSystem sys);


#endif //HACKENROLLMENT_H
