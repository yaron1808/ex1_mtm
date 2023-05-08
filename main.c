#include <stdio.h>
#include <string.h>
#include "IsraeliQueue.h"
#include "HackEnrollment.h"

int main(int argc, char** argv)
{
    if(argc!=6 && argc!=7)
    {
        printf("Usage: %s [-i] <students file> <courses file> <hackers file> <queues file> <output file> \n", argv[0]);
        return 1;
    }
    char* flag = argv[1];

    char* students_file = (argc==6) ? argv[1] : argv[2];
    char* courses_file = (argc==6) ? argv[2] : argv[3];
    char* hackers_file = (argc==6) ? argv[3] : argv[4];
    char* queues_file = (argc==6) ? argv[4] : argv[5];
    char* output_file = (argc==6) ? argv[5] : argv[6];

    FILE* students = fopen(students_file, "r");
    FILE* courses = fopen(courses_file, "r");
    FILE* hackers = fopen(hackers_file, "r");
    FILE* queues = fopen(queues_file, "r");
    FILE* out = fopen(output_file, "w");

    if(students == NULL || courses == NULL || hackers == NULL || queues == NULL || out == NULL)
    {
        printf("Error opening files\n");
        return 0;
    }

    EnrollmentSystem sys = createEnrollment(students, courses, hackers);
    readEnrollment(sys, queues);
    if(strcmp(flag, "-i") == 0)
    {
        setCaseSensitivity(sys, true);
    }
    hackEnrollment(sys, out);
    fclose(students);
    fclose(courses);
    fclose(hackers);
    fclose(queues);
    fclose(out);
    destroyEnrollment(sys);
    return 0;
}
