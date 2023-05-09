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
 * this function creates a new EnrollmentSystemi
 * @param students student file
 * @param courses course file
 * @param hackers hackers file
 * @return NULL if any of the files is NULL or if allocation failed, otherwise a new EnrollmentSystem
 */
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers);

/**
 * this function reads the queues file and enqueues the students to the courses
 * @param sys the EnrollmentSystem
 * @param queues the queues file
 * @return the EnrollmentSystem
 */
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues);

/**
 * this function hacks the EnrollmentSystem
 * @param sys the EnrollmentSystem
 * @param out the output file
 */
void hackEnrollment(EnrollmentSystem sys, FILE* out);

/**
 * this function sets the case sensitivity of the EnrollmentSystem
 * @param sys the EnrollmentSystem
 * @param caseSensitive true if case sensitive, false otherwise
 */
void setCaseSensitivity(EnrollmentSystem sys, bool caseSensitive);

/**
 * this function destroys the EnrollmentSystem and deallocates all memory
 * @param sys the EnrollmentSystem
 */
void destroyEnrollment(EnrollmentSystem sys);


#endif //HACKENROLLMENT_H
