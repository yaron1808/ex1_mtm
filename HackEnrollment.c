#include "HackEnrollment.h"
#include <stdlib.h>
#include "IsraeliQueue.h"

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
    int size;
};
typedef struct Courses_t* Course;
typedef struct Courses_t Courses_t;
struct EnrollmentSystem_t{
    Student* students;
    Course* courses;
    IsraeliQueue* queuesArr;
};

typedef struct EnrollmentSystem_t EnrollmentSystem_t;
Student* createStudentsArray(FILE* students);
Course* createCoursesArray(FILE* courses);
IsraeliQueue* createQueuesArray(FILE* courses);
int findHowManyEnters(FILE* file);
Course createCourse(char* courseNumber, int size);

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
    system->students = createStudentsArray(students);
    system->courses = createCoursesArray(courses);
    system->queuesArr = createQueuesArray();

    return system;
}
Student* createStudentsArray(FILE* students)
{
    int len = findHowManyEnters(students);
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

