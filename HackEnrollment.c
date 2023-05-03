#include <stdlib.h>
#include <string.h>
#include "HackEnrollment.h"
#include "IsraeliQueue.h"

#define ABS(N) (((N)<0)?(-(N)):((N)))

 struct Students_t
{
    long long id;
    int totalCredits;
    int GPA;
    char* firstName;
    char* lastName;
    char* city;
    char* department;
    bool isHacker;
    char** desiredCourses;
    long long* friendsIds;
    long long* rivalsIds;

};

typedef struct Students_t* Student;
struct Courses_t
{
    char* courseNumber;
    IsraeliQueue queue;
    int size;
};
typedef struct Courses_t* Course;
typedef struct Courses_t Courses_t;
struct EnrollmentSystem_t{
    Student* students;
    Course* courses;
    Student* hackers;
};
//functions declerations
typedef struct EnrollmentSystem_t EnrollmentSystem_t;
Student* createStudentsArray(FILE* students, FILE* hackers);
Course* createCoursesArray(FILE* courses);
IsraeliQueue* createQueuesArray(FILE* courses);
int findHowManyEnters(FILE* file);
Student createStudent(long long id, int totalCredits, int GPA, char* firstName, char* lastName, char* city, char* department, char** desiredCourses, long long* friendsIds, long long* rivalsIds);
Student* createHackersArray(FILE* hackers);
int howManySpaces(char* str);
Course createCourse(char* courseNumber, int size);

//functions
EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers){
    if(students == NULL || courses == NULL || hackers == NULL)
    {
        fclose(students);
        fclose(courses);
        fclose(hackers);
        return NULL;
    }
    EnrollmentSystem system = malloc(sizeof(*system));
    if(system == NULL)
    {
        return NULL;
    }
    system->students = createStudentsArray(students,hackers);
    system->courses = createCoursesArray(courses);
    system->hackers = createHackersArray(hackers);

    return system;
}
Student* createHackersArray(FILE* hackers)
{
    int len = findHowManyEnters(hackers);
    if(len == -1)
    {
        return NULL;
    }
    len = len/4;
    Student* hackersArr = malloc(sizeof(struct Students_t)*len);
    if(hackersArr == NULL)
    {
        return NULL;
    }
    char buffer[1024] = "";
    for (int i = 0; i < len; ++i)
    {
        hackersArr[i] = createStudent(0,0,0,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
        fgets(buffer,1024,hackers);
        sscanf(buffer,"%lld",&hackersArr[i]->id);
        for(int j = 0; j < 3; j++)
        {

        }
    }
    return hackersArr;
}

int howManySpaces(char* str)
{
    int counter = 0;
    for(int i = 0; str[i] != '\0'; i++)
    {
        if(str[i] == ' ')
        {
            counter++;
        }
    }
    return counter;
}
Student* createStudentsArray(FILE* students, FILE* hackers)
{
    int len = findHowManyEnters(students);
    if(len == -1)
    {
        return NULL;
    }
    Student* studentsArr = malloc(sizeof(struct Students_t)*len);
    if(studentsArr == NULL)
    {
        return NULL;
    }

    long long id;
    int totalCredits;
    int GPA;
    char* firstName;
    char* lastName;
    char* city;
    char* department;

    char buffer[1024] = "";
    for(int i = 0; i<len; i++)
    {

        fgets(buffer,1024,students);
        sscanf(buffer,"%lld %d %d %s %s %s %s",&id,&totalCredits,&GPA,firstName,lastName,city,department);
        studentsArr[i] = createStudent(id,totalCredits,GPA,firstName,lastName,city,department,NULL,NULL,NULL);
        if(studentsArr[i] == NULL)
        {
            return NULL;
        }
    }
    return studentsArr;
}


Course* createCoursesArray(FILE* courses)
{
    int len = findHowManyEnters(courses);
    if (len == -1)
    {
        return NULL;
    }
    Course* coursesArray = malloc(len * sizeof (Courses_t));
    for (int i = 0; i < len; i++)
    {
        char* courseNumber;
        int size;
        fscanf(courses, "%s %d", courseNumber, &size);
        coursesArray[i] = createCourse(courseNumber, size);
    }
    return coursesArray;
}

IsraeliQueue* createQueuesArray(FILE* courses)
{

}
int findHowManyEnters(FILE* file)
{
    if(file == NULL)
    {
        return -1;
    }
    int counter = 0;
    int c = fgetc(file);
    while (c!=EOF)
    {
        if(c == '\n')
        {
            counter++;
        }
        c = fgetc(file);
    }
    return counter;
}
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    if(sys ==NULL || queues == NULL)
    {
        return NULL;
    }

    return NULL;

}

Student createStudent(long long id, int totalCredits, int GPA, char* firstName, char* lastName, char* city, char* department, char** desiredCourses, long long* friendsIds, long long* rivalsIds)
{
    if(firstName == NULL || lastName == NULL || city == NULL || department == NULL || desiredCourses == NULL || friendsIds == NULL || rivalsIds == NULL)
    {
        return NULL;
    }
    Student student = malloc(sizeof(*student));
    if(student == NULL)
    {
        return NULL;
    }
    student->id = id;
    student->totalCredits = totalCredits;
    student->GPA = GPA;
    student->firstName = firstName;
    student->lastName = lastName;
    student->city = city;
    student->department = department;
    student->desiredCourses = desiredCourses;
    student->friendsIds = friendsIds;
    student->rivalsIds = rivalsIds;
    return student;
}

Course createCourse(char* courseNumber, int size)
{
    if(courseNumber == NULL )
    {
        return NULL;
    }
    Course course = malloc(sizeof(*course));
    if(course == NULL)
    {
        return NULL;
    }
    course->courseNumber = courseNumber;
    course->size = size;
    return course;
}

int IdsDiff(long long id1, long long id2)
{
    return ABS(id1-id2);
}

int nameDiff(char* firstName1, char* firstName2, char* lastName1, char* lastName)
{
    int firstNameDiff = 0;
    int lastNameDiff = 0;
    int i = 0;
    while(firstName1[i] && firstName2[i])
    {
        firstNameDiff+=ABS(firstName1[i]-firstName2[i]);
        i++;
    }
    while (firstName1[i])
    {
        firstNameDiff+=firstName1[i];
        i++;
    }
    while (firstName2[i])
    {
        firstNameDiff+=firstName2[i];
        i++;
    }

    i = 0;
    while(lastName1[i] && lastName[i])
    {
        lastNameDiff+=ABS(lastName1[i]-lastName[i]);
        i++;
    }
    while (lastName1[i])
    {
        lastNameDiff+=lastName1[i];
        i++;
    }
    while (lastName[i])
    {
        lastNameDiff+=lastName[i];
        i++;
    }
    return firstNameDiff+lastNameDiff;
}

