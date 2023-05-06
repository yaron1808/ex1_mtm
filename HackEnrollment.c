#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HackEnrollment.h"
#include "IsraeliQueue.h"

#define ABS(N) (((N)<0)?(-(N)):((N)))


typedef struct Hacker_t* Hacker;
typedef struct Courses_t* Course;
typedef struct Student_t* Student;

 struct Student_t
{
    int id;
    int totalCredits;
    int GPA;
    char* firstName;
    char* lastName;
    char* city;
    char* department;
    Hacker hacker;
};

struct Hacker_t
{
    Student student;
    int id;
    int* courses;
    int coursesLen;
    int* friendsIds;
    int friendsLen;
    int* rivalsIds;
    int rivalsLen;
};

struct Courses_t
{
    int courseNumber;
    int courseSize;
    IsraeliQueue queue;
};
typedef struct Courses_t Courses_t;

struct EnrollmentSystem_t{
    Student* students;
    int studentsLen;
    Course* courses;
    int coursesLen;
    Hacker* hackers;
    int hackersLen;
};
//functions declerations
typedef struct EnrollmentSystem_t EnrollmentSystem_t;

//Student* createStudentsArray(FILE* students);
//Course* createCoursesArray(FILE* courses);
//IsraeliQueue* createQueuesArray(FILE* courses);
//int findHowManyEnters(FILE* file);
//Student createStudent(char* id, int totalCredits, int GPA, char* firstName, char* lastName, char* city, char* department, char** desiredCourses, char** friendsIds, char** rivalsIds);
//Hacker * createHackersArray(FILE* hackers);
//Course createCourse(int courseNumber, int size);

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
    rewind(file);
    return counter;
}

int howManySpaces(const char* str)
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

/**
 *
 * @param str
 * @param len
 * @return
 */
int* allocateIntsArrayFromLine(const char* str,int* len)
{
    int cnt = 0;
    int ret = 0;
    int val = 0;
    int bytesRead;
    const char* str0 = str;
    ret = sscanf(str,"%d%n",&val, &bytesRead);
    str+=bytesRead;
    while(ret != EOF && ret != 0)
    {
        cnt++;
        ret = sscanf(str,"%d%n",&val, &bytesRead);
        str += bytesRead;
    }
    *len = cnt;
    str = str0;

    int* arr = malloc(sizeof(int) * (*len));
    if(arr == NULL)
    {
        *len = 0;
        return NULL;
    }
    for (int i = 0; i < (*len); ++i)
    {
        sscanf(str,"%d%n",&arr[i], &bytesRead);
        str += bytesRead;
    }

    return arr;
}

//functions


//TODO: implement destory functions

Student createStudent(int id, int totalCredits, int GPA, char* firstName, char* lastName, char* city, char* department)
{
    if(firstName == NULL || lastName == NULL || city == NULL || department == NULL)
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
    student->hacker = NULL;

    return student;
}

void destroyStudent(Student student)
{
    if(student == NULL)
    {
        return;
    }
    free(student->firstName);
    free(student->lastName);
    free(student->city);
    free(student->department);
    if(student->hacker != NULL)
    {
        student->hacker->student = NULL;
    }
    free(student);
}

Student* createStudentsArray(FILE* students, int* len)
{
    *len = findHowManyEnters(students);
    if(*len == -1)
    {
        return NULL;
    }
    Student* studentsArr = malloc(sizeof(struct Student_t) * (*len));
    if(studentsArr == NULL)
    {
        return NULL;
    }

    int id;
    int totalCredits;
    int GPA;
    char* firstName;
    char* lastName;
    char* city;
    char* department;
    int num;

    for(int i = 0; i<(*len); i++)
    {
        num = fscanf(students,"%d %d %d %ms %ms %ms %ms",&id,&totalCredits,&GPA,&firstName,&lastName,&city,&department); // TODO check if it works
        assert(num == 7);
        studentsArr[i] = createStudent(id,totalCredits,GPA,firstName,lastName,city,department);
        if(studentsArr[i] == NULL)
        {
            return NULL;
        }
    }
    return studentsArr;
}

void destroyStudentsArray(Student* students, int len)
{
    if(students == NULL)
    {
        return;
    }
    for(int i = 0; i<len; i++)
    {
        destroyStudent(students[i]);
    }
    free(students);
}

Course createCourse(int courseNumber, int size)
{
    Course course = malloc(sizeof(*course));
    if(course == NULL)
    {
        return NULL;
    }
    course->courseNumber = courseNumber;
    course->courseSize = size;
    course->queue = NULL;
    return course;
}

void destroyCourse(Course course)
{
    if(course == NULL)
    {
        return;
    }
    IsraeliQueueDestroy(course->queue);
    free(course);
}

Course* createCoursesArray(FILE* courses, int* len)
{
    *len = findHowManyEnters(courses);
    if (*len == -1)
    {
        return NULL;
    }
    Course* coursesArray = malloc((*len) * sizeof (Courses_t));
    for (int i = 0; i < (*len); i++)
    {
        int courseNumber;
        int size;
        int num;
        num = fscanf(courses, "%d %d", &courseNumber, &size);
        assert(num==2);
        coursesArray[i] = createCourse(courseNumber, size);
    }
    return coursesArray;
}

void destroyCoursesArray(Course* courses, int len)
{
    if(courses == NULL)
    {
        return;
    }
    for(int i = 0; i<len; i++)
    {
        destroyCourse(courses[i]);
    }
    free(courses);
}

Hacker createHacker(int id, int* courses, int coursesLen, int* friendsIds, int friendsLen, int* rivalsIds, int rivalsLen)
{
    if(courses == NULL || friendsIds == NULL || rivalsIds == NULL || coursesLen == 0 || friendsLen == 0 || rivalsLen == 0)
    {
        return NULL;
    }

    Hacker hacker = malloc(sizeof(*hacker));
    if(hacker == NULL)
    {
        return NULL;
    }
    hacker->id = id;

    hacker->courses = courses;
    hacker->coursesLen = coursesLen;
    hacker->friendsIds = friendsIds;
    hacker->friendsLen = friendsLen;
    hacker->rivalsIds = rivalsIds;
    hacker->rivalsLen = rivalsLen;
    hacker->student = NULL;
    return hacker;
}

void destroyHacker(Hacker hacker)
{
    if(hacker == NULL)
    {
        return;
    }
    free(hacker->courses);
    free(hacker->friendsIds);
    free(hacker->rivalsIds);
    if(hacker->student != NULL)
    {
        hacker->student->hacker = NULL;
    }
    free(hacker);
}

Hacker* createHackersArray(FILE* hackers, int *len)
{
    *len = findHowManyEnters(hackers);
    if(*len == -1)
    {
        return NULL;
    }
    (*len) = (*len)/4;

    Hacker* hackersArr = malloc(sizeof(struct Hacker_t) * (*len));

    if(hackersArr == NULL)
    {
        return NULL;
    }

    int id;
    int* coursesArr;
    int coursesLen;
    int* friendsArr;
    int friendsLen;
    int* rivalsArr;
    int rivalsLen;

    char* line = NULL;
    for (int i = 0; i < (*len); ++i)
    {
        fscanf(hackers,"%d\n", &id);
        fscanf(hackers,"%m[^\n]\n", &line);
        coursesArr = allocateIntsArrayFromLine(line,&coursesLen);
        free(line);
        fscanf(hackers,"%m[^\n]\n", &line);
        friendsArr = allocateIntsArrayFromLine(line,&friendsLen);
        free(line);
        fscanf(hackers,"%m[^\n]\n", &line);
        rivalsArr = allocateIntsArrayFromLine(line,&rivalsLen);
        free(line);
        hackersArr[i] = createHacker(id,coursesArr,coursesLen,friendsArr,friendsLen,rivalsArr,rivalsLen);

    }
    return hackersArr;
}

void destroyHackersArray(Hacker* hackers, int len)
{
    if(hackers == NULL)
    {
        return;
    }
    for(int i = 0; i<len; i++)
    {
        destroyHacker(hackers[i]);
    }
    free(hackers);
}

void linkHackerToStudent(Hacker* hackersArr,int hackerLen, Student* studentsArr, int studentsLen)
{
    int studentIndex = 0;
    for(int i = 0; i<hackerLen; i++)
    {
        while(studentIndex<studentsLen)
        {
            if(hackersArr[i]->id == studentsArr[studentIndex]->id)
            {
                hackersArr[i]->student = studentsArr[studentIndex];
                studentsArr[studentIndex]->hacker = hackersArr[i];
                break;
            }
            studentIndex++;
        }
    }
}

EnrollmentSystem createEnrollment(FILE* students, FILE* courses, FILE* hackers)
{
    if(students == NULL || courses == NULL || hackers == NULL)
    {
        return NULL;
    }
    EnrollmentSystem sys = malloc(sizeof(*sys));
    if(sys == NULL)
    {
        return NULL;
    }
    int studentsLen;
    int coursesLen;
    int hackersLen;
    sys->students = createStudentsArray(students,&studentsLen);
    sys->courses = createCoursesArray(courses,&coursesLen);
    sys->hackers = createHackersArray(hackers,&hackersLen);
    if(sys->students == NULL || sys->courses == NULL || sys->hackers == NULL)
    {
        return NULL;
    }
    sys->studentsLen = studentsLen;
    sys->coursesLen = coursesLen;
    sys->hackersLen = hackersLen;
    linkHackerToStudent(sys->hackers,hackersLen,sys->students,studentsLen);
    return sys;
}

int compareFunction(void* p1, void* p2)
{
//    Student s1 = (Student)p1;
//    Student s2 = (Student)p2;
//    if(s1->id > s2->id)
//    {
//        return 1;
//    }
//    if(s1->id < s2->id)
//    {
//        return -1;
//    }
    return 0;
}

Course findCourseByCourseNum(EnrollmentSystem sys, int courseNum)
{
    for (int i = 0; i < sys->coursesLen; ++i)
    {
        if(sys->courses[i]->courseNumber == courseNum)
        {
            return sys->courses[i];
        }
    }
    return NULL;
}

Student findStudentById(EnrollmentSystem sys, int id)
{
    for (int i = 0; i < sys->studentsLen; ++i)
    {
        if(sys->students[i]->id == id)
        {
            return sys->students[i];
        }
    }
    return NULL;
}

EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    if(sys ==NULL || queues == NULL)
    {
        return NULL;
    }
    FriendshipFunction friendFunctions[] = {NULL};
    for (int i = 0; i < sys->coursesLen; ++i)
    {
        sys->courses[i]->queue = IsraeliQueueCreate(friendFunctions,compareFunction,FRIENDSHIP_THRESHOLD,RIVALRY_THRESHOLD);
        if(sys->courses[i]->queue == NULL)
        {
            return NULL;
        }
    }

    char* line = NULL;
    int ret = fscanf(queues,"%m[^\n]\n",&line);
    while(line!=NULL && ret!= 0 && ret !=EOF)
    {
        int len;
        int* id = allocateIntsArrayFromLine(line,&len);
        if(id == NULL)
        {
            return NULL;
        }
        free(line);

        int courseNum = id[0];
        IsraeliQueue q = findCourseByCourseNum(sys,courseNum)->queue;
        for (int i = 1; i < len; ++i)
        {
            IsraeliQueueEnqueue(q, findStudentById(sys,id[i]));
        }
        free(id);
        fscanf(queues,"%m[^\n]\n",&line);
    }

    return sys;
}




int IdsDiff(int id1, int id2)
{
    return ABS(id1-id2);
}

int nameDiff(const char* firstName1, const char* firstName2, const char* lastName1, const char* lastName)
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

