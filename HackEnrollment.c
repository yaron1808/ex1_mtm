#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HackEnrollment.h"
#include "IsraeliQueue.h"

#define ABS(N) (((N)<0)?(-(N)):((N)))
#define CHUNK_SIZE 256

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
    int coursesEnrolled;
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

void destroyHackersArray(Hacker* hackers, int len);

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


char* readLineFromFile(FILE* file)
{
    if (file == NULL)
    {
        return NULL;
    }

    int bufferSize = 256;
    int position = 0;
    char* buffer = malloc(bufferSize * sizeof(char));

    if (buffer == NULL)
    {
        return NULL;
    }

    int c;
    while ((c = fgetc(file)) != EOF && c != '\n')
    {
        buffer[position++] = (char)c;

        if (position >= bufferSize)
        {
            bufferSize += 256;
            char* temp = realloc(buffer, bufferSize * sizeof(char));
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }
    }

    if (position == 0 && c == EOF)
    {
        free(buffer);
        return NULL;
    }

    buffer[position] = '\0';
    return buffer;
}


/**
 *
 * @param str
 * @param len
 * @return
 */
int* allocateIntsArrayFromLine(const char* str,int* len)
{
    if(strlen(str) == 0)
    {
        int* arr = malloc(sizeof(int));
        if(arr == NULL)
        {
            return NULL;
        }
        *len = 0;
        arr[0] = 0;
        return arr;
    }
    {
        return NULL;
    }
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
    if(courses == NULL || friendsIds == NULL || rivalsIds == NULL || coursesLen == 0)
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
    hacker->coursesEnrolled = 0;
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

    //char* line = NULL;
    for (int i = 0; i < (*len); ++i)
    {
        fscanf(hackers,"%d\n", &id);
        //fscanf(hackers,"%m[^\n]\n", &line);
        char* line = readLineFromFile(hackers);
        coursesArr = allocateIntsArrayFromLine(line,&coursesLen);
        free(line);
        //fscanf(hackers,"%m[^\n]\n", &line);
        line = readLineFromFile(hackers);
        friendsArr = allocateIntsArrayFromLine(line,&friendsLen);
        free(line);
//        fscanf(hackers,"%m[^\n]\n", &line);
        line = readLineFromFile(hackers);
        rivalsArr = allocateIntsArrayFromLine(line,&rivalsLen);
        free(line);
        hackersArr[i] = createHacker(id,coursesArr,coursesLen,friendsArr,friendsLen,rivalsArr,rivalsLen);
        if(hackersArr[i] == NULL)
        {
            destroyHackersArray(hackersArr,i);
            return NULL;
        }
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
    return (int)(p1==p2);
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
    //int ret = fscanf(queues,"%m[^\n]\n",&line);
    line = readLineFromFile(queues);
    while(line!=NULL)
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
        //fscanf(queues,"%m[^\n]\n",&line);
        line = readLineFromFile(queues);
    }
    return sys;
}




int IdsDiff(int id1, int id2)
{
    return ABS(id1-id2);
}

int friendshipFunc3(void* item1, void* item2)
{
    Student student1 = (Student)item1;
    Student student2 = (Student)item2;
    if(student1->hacker == NULL && student2->hacker == NULL)
    {
        return NO_RELATION;
    }
    return IdsDiff(student1->id,student2->id);

}

int nameDiff(const char* firstName1, const char* firstName2, const char* lastName1, const char* lastName2)
{
    int firstNameDiff = 0;
    int lastNameDiff = 0;
    int i = 0;
    while(firstName1[i] && firstName2[i])
    {
        firstNameDiff+=ABS(firstName1[i]-firstName2[i]);
        i++;
    }
    int j = i;
    while (firstName1[i])
    {
        firstNameDiff+=firstName1[i];
        i++;
    }
    i=j;
    while (firstName2[i])
    {
        firstNameDiff+=firstName2[i];
        i++;
    }

    i = 0;
    while(lastName1[i] && lastName2[i])
    {
        lastNameDiff+=ABS(lastName1[i] - lastName2[i]);
        i++;
    }
    j=i;
    while (lastName1[i])
    {
        lastNameDiff+=lastName1[i];
        i++;
    }
    i=j;
    while (lastName2[i])
    {
        lastNameDiff+=lastName2[i];
        i++;
    }
    return firstNameDiff+lastNameDiff;
}

int friendshipFunc2(void* item1, void* item2)
{
    Student student1 = (Student)item1;
    Student student2 = (Student)item2;
    if(student1->hacker == NULL && student2->hacker == NULL)
    {
        return NO_RELATION;
    }
    return nameDiff(student1->firstName,student2->firstName,student1->lastName,student2->lastName);
}

int friendShipHackerStudent(Student student, Hacker hacker)
{
    for (int i = 0; i < hacker->friendsLen; ++i)
    {
        if(hacker->friendsIds[i] == student->id)
        {
            return FRIENDS;
        }
    }

    for (int i = 0; i < hacker->rivalsLen; ++i)
    {
        if(hacker->rivalsIds[i] == student->id)
        {
            return RIVALS;
        }
    }
    return NO_RELATION;
}

int friendshipFunc1(void* item1, void* item2)
{
    Student student1 = (Student)item1;
    Student student2 = (Student)item2;

    if(student1->hacker == NULL && student2->hacker == NULL)
    {
        return NO_RELATION;
    }
    int result = NO_RELATION;
    if(student1->hacker != NULL)
    {
        result = friendShipHackerStudent(student2, student1->hacker);
    }

    if(result == 0 && student2->hacker != NULL)
    {
        result = friendShipHackerStudent(student1, student2->hacker);
    }
    return result;


}
bool IsInArray(int* arr, int len, int num)
{
    for (int i = 0; i < len; ++i)
    {
        if(arr[i] == num)
        {
            return true;
        }
    }
    return false;
}

int findHackerPositionInQueue(IsraeliQueue queue, Hacker hacker)
{

}

void updateHackerEnrollment(EnrollmentSystem sys, int* id, int len)
{
    Course currentCourse = findCourseByCourseNum(sys, id[0]);
    for (int i = 1; i < len && i <= currentCourse->courseSize; ++i)
    {
        if(findStudentById(sys,id[i])->hacker!=NULL)
        {
            findStudentById(sys,id[i])->hacker->coursesEnrolled++;
        }
    }
}

void copy(FILE* input, FILE* output)
{
    rewind(input); // rewind the file pointer to the beginning (just in case)
    char buffer[CHUNK_SIZE];
    while (fgets(buffer, CHUNK_SIZE, input) != NULL)
    {
        fputs(buffer, output);
    }
}

void hackEnrollment(EnrollmentSystem sys, FILE* out)
{
    if(sys == NULL || out == NULL)
    {
        return;
    }

    for (int i = 0; i < sys->coursesLen; ++i)
    {
        IsraeliQueueAddFriendshipMeasure(sys->courses[i]->queue, friendshipFunc1);
        IsraeliQueueAddFriendshipMeasure(sys->courses[i]->queue, friendshipFunc2);
        IsraeliQueueAddFriendshipMeasure(sys->courses[i]->queue, friendshipFunc3);
        for (int j = 0; j < sys->hackersLen; ++j)
        {
            if(IsInArray(sys->hackers[j]->courses,sys->hackers[j]->coursesLen,sys->courses[i]->courseNumber))
            {
                IsraeliQueueEnqueue(sys->courses[i]->queue,sys->hackers[j]->student);
            }
        }

    }

    FILE* tempEnrollment = fopen("temp.txt","w+");
    if(tempEnrollment == NULL)
    {
        return;
    }

    for (int i = 0; i < sys->coursesLen; ++i)
    {
        int queueSize = IsraeliQueueSize(sys->courses[i]->queue);
        if(queueSize == 0)
        {
            continue;
        }
        fprintf(tempEnrollment, "%d ",sys->courses[i]->courseNumber);
        for (int j = 0; j < queueSize - 1; ++j)
        {
            Student student = (Student) IsraeliQueueDequeue(sys->courses[i]->queue);
            fprintf(tempEnrollment, "%d ", student->id);
        }
        Student student = (Student) IsraeliQueueDequeue(sys->courses[i]->queue);
        if(student != NULL)
        {
            fprintf(tempEnrollment, "%d\n", student->id);
        }
        else
        {
            fprintf(tempEnrollment, "\n");
        }
    }

    rewind(tempEnrollment);
    char* line = readLineFromFile(tempEnrollment);
    int len;
    while(line!=NULL)
    {
        int* id = allocateIntsArrayFromLine(line,&len);
        updateHackerEnrollment(sys,id,len);
        free(id);
        free(line);
        line = readLineFromFile(tempEnrollment);
    }


    for (int i = 0; i < sys->hackersLen; ++i)
    {
        if(sys->hackers[i]->coursesEnrolled < MIN_COURSES && sys->hackers[i]->coursesLen > 1)
        {
            fprintf(out,"Cannot satisfy constraints for %d", sys->hackers[i]->id);
            return;
        }
        if(sys->hackers[i]->coursesEnrolled == 0 && sys->hackers[i]->coursesLen == 1)
        {
            fprintf(out,"Cannot satisfy constraints for %d", sys->hackers[i]->id);
            return;
        }
    }

    copy(tempEnrollment,out);
    fclose(tempEnrollment);
    remove("temp.txt");
}

void stringToUpper(char* str)
{
    int i = 0;
    while(str[i])
    {
        str[i] = (str[i] >= 'a' && str[i] <= 'z') ? str[i] - ('a'-'A') : str[i];
        i++;
    }
}

void setCaseSensitivity(EnrollmentSystem sys, bool caseSensitive)
{
    if(sys == NULL)
    {
        return;
    }
    if(caseSensitive)
    {
        for (int i = 0; i < sys->studentsLen; ++i)
        {
            stringToUpper(sys->students[i]->firstName);
            stringToUpper(sys->students[i]->lastName);
        }
    }
}