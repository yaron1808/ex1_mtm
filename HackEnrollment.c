#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "HackEnrollment.h"
#include "IsraeliQueue.h"

#define ABS(N) (((N)<0)?(-(N)):((N)))
#define BUFFER_SIZE 256

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

/**
 * this function check how many enters in the file
 * @param file file to check
 * @return number of enters
 */
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

/**
 * this function read line from file
 * @param file file to read from
 * @return line from file
 */
char* readLineFromFile(FILE* file)
{
    if (file == NULL)
    {
        return NULL;
    }

    int bufferSize = BUFFER_SIZE;
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
            bufferSize += BUFFER_SIZE;
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
 * this function read line from file until space
 * @param file file to read from
 * @return word from file
 */

char* readUntilSpaceFromFile(FILE* file)
{
    if (file == NULL)
    {
        return NULL;
    }

    int bufferSize = BUFFER_SIZE;
    int position = 0;
    char* buffer = malloc(bufferSize * sizeof(char));

    if (buffer == NULL)
    {
        return NULL;
    }
    int c = fgetc(file);
    if (c != ' ')
    {
        ungetc(c, file);
    }
    while ((c = fgetc(file)) != EOF && c != '\n' && c != ' ')
    {
        buffer[position++] = (char)c;

        if (position >= bufferSize)
        {
            bufferSize += BUFFER_SIZE;
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
 * this function allocate array of ints from string
 * @param str string to allocate from
 * @param len update the length of the array
 * @return pointer to the array
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

    int counter = 0;
    int returnValue = 0;
    int value = 0;
    int bytesRead;
    const char* str0 = str;
    returnValue = sscanf(str, "%d%n", &value, &bytesRead);
    str+=bytesRead;
    while(returnValue != EOF && returnValue != 0)
    {
        counter++;
        returnValue = sscanf(str, "%d%n", &value, &bytesRead);
        str += bytesRead;
    }
    *len = counter;
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


/**
 * this function create student
 * @param id id of the student
 * @param totalCredits total credits of the student
 * @param GPA GPA of the student
 * @param firstName first name of the student
 * @param lastName last name of the student
 * @param city city of the student
 * @param department department of the student
 * @return pointer to the student
 */

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
/**
 * this function destroy student and deallocate all the memory
 * @param student student to destroy
 */
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
/**
 * this function creates array of students from file
 * @param students file to read from
 * @param len len of the array
 * @return pointer to the array
 */
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

    for(int i = 0; i<(*len); i++)
    {
        fscanf(students,"%d %d %d",&id,&totalCredits,&GPA);
        firstName = readUntilSpaceFromFile(students);
        lastName = readUntilSpaceFromFile(students);
        city = readUntilSpaceFromFile(students);
        department = readUntilSpaceFromFile(students);
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

/**
 * this function create course
 * @param courseNumber the number of the course
 * @param size course max size
 * @return
 */
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

/**
 * this function destroy course and deallocate all the memory
 * @param course course to destroy
 */
void destroyCourse(Course course)
{
    if(course == NULL)
    {
        return;
    }
    IsraeliQueueDestroy(course->queue);
    free(course);
}
/**
 * this function creates array of courses from file
 * @param courses file to read from
 * @param len len of the array
 * @return pointer to the array
 */
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
        fscanf(courses, "%d %d", &courseNumber, &size);
        coursesArray[i] = createCourse(courseNumber, size);
        if(coursesArray[i] == NULL)
        {
            return NULL;
        }
    }
    return coursesArray;
}

/**
 * this function destroy courses array and deallocate all the memory
 * @param courses courses array to destroy
 * @param len len of the array
 */
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

/**
 * this function create hacker
 * @param id id of the hacker
 * @param courses array of courses the hacker is enrolled to
 * @param coursesLen len of the courses array
 * @param friendsIds array of friends ids
 * @param friendsLen len of the friends array
 * @param rivalsIds array of rivals ids
 * @param rivalsLen len of the rivals array
 * @return pointer to the hacker
 */
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

/**
 * this function destroy hacker and deallocate all the memory
 * @param hacker hacker to destroy
 */
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
/**
 * this function creates array of hackers from file
 * @param hackers file to read from
 * @param len len of the array
 * @return pointer to the array
 */

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

    for (int i = 0; i < (*len); ++i)
    {
        fscanf(hackers,"%d\n", &id);
        char* line = readLineFromFile(hackers);
        coursesArr = allocateIntsArrayFromLine(line,&coursesLen);
        free(line);
        line = readLineFromFile(hackers);
        friendsArr = allocateIntsArrayFromLine(line,&friendsLen);
        free(line);
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

/**
 * this function destroy hackers array and deallocate all the memory
 * @param hackers hackers array to destroy
 * @param len len of the array
 */

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

/**
 * this function link each hacker to his student and vice versa
 * @param hackersArr hackers array
 * @param hackerLen hackers array len
 * @param studentsArr students array
 * @param studentsLen students array len
 */
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

/**
 * this function create enrollment system
 * @param students students file
 * @param courses courses file
 * @param hackers hackers file
 * @return pointer to the new enrollment system
 */
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

/**
 * this function find course by course number
 * @param sys enrollment system to search in
 * @param courseNum course number to search
 * @return pointer to the course
 */
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

/**
 * this function find student by id
 * @param sys enrollment system to search in
 * @param id id to search
 * @return pointer to the student
 */
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

/**
 * this function read enrollment from file and update the enrollment system
 * @param sys enrollment system to read to
 * @param queues file to read from
 * @return pointer to the updated enrollment system
 */
EnrollmentSystem readEnrollment(EnrollmentSystem sys, FILE* queues)
{
    if(sys ==NULL || queues == NULL)
    {
        return NULL;
    }
    FriendshipFunction friendFunctions[] = {NULL};
    for (int i = 0; i < sys->coursesLen; ++i)
    {
        sys->courses[i]->queue = IsraeliQueueCreate(friendFunctions,compareFunction,
                                                    FRIENDSHIP_THRESHOLD,RIVALRY_THRESHOLD);
        if(sys->courses[i]->queue == NULL)
        {
            return NULL;
        }
    }

    char* line = NULL;
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
        line = readLineFromFile(queues);
    }
    return sys;
}



/**
 *
 * @param id1 id of student 1
 * @param id2 id of student 2
 * @return absolute value of the difference between the ids
 */
int IdsDiff(int id1, int id2)
{
    return ABS(id1-id2);
}

/**
 * this function calculate the friendship between two students by their ids
 * when at least one of them is a hacker
 * @param item1 student 1
 * @param item2 student 2
 * @return id difference
 */
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

/**
 * this function calculate the friendship between two students by their names
 * @param firstName1 first name of student 1
 * @param firstName2 first name of student 2
 * @param lastName1 last name of student 1
 * @param lastName2 last name of student 2
 * @return absolute value of the difference between the names
 */
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

/**
 * this function calculate the friendship between two students by their names
 * when at least one of them is a hacker
 * @param item1 student 1
 * @param item2 student 2
 * @return name difference
 */
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

/**
 * this function indicate the friendship between student and hacker
 * @param student student to check
 * @param hacker hacker to check
 * @return trinary value of the friendship
 */
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

/**
 * this function indicate the friendship between two students
 * @param item1 student 1
 * @param item2 student 2
 * @return trinary value of the friendship
 */
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

/**
 * this function update the number of courses enrolled for each hacker
 * @param sys enrollment system to update
 * @param id array of ids of students enrolled to the course
 * @param len length of the array
 */
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

/**
 * this function copy the content of one file to another
 * @param input input file
 * @param output output file
 */
void copy(FILE* input, FILE* output)
{
    rewind(input); // rewind the file pointer to the beginning (just in case)
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, input) != NULL)
    {
        fputs(buffer, output);
    }
}

/**
 * this function hack the enrollment system
 * @param sys enrollment system to hack
 * @param out output file
 */
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
        if((sys->hackers[i]->coursesEnrolled < MIN_COURSES && sys->hackers[i]->coursesLen > 1)
        || (sys->hackers[i]->coursesEnrolled == 0 && sys->hackers[i]->coursesLen == 1))
        {
            fprintf(out,"Cannot satisfy constraints for %d\n", sys->hackers[i]->id);
            fclose(tempEnrollment);
            remove("temp.txt");
            return;
        }
    }

    copy(tempEnrollment,out);
    fclose(tempEnrollment);
    remove("temp.txt");
}

/**
 * this function convert a string to upper case
 * @param str string to convert
 */
void stringToUpper(char* str)
{
    int i = 0;
    while(str[i])
    {
        str[i] = (str[i] >= 'a' && str[i] <= 'z') ? str[i] - ('a'-'A') : str[i];
        i++;
    }
}

/**
 * this function set the case sensitivity of the system
 * @param sys enrollment system to update
 * @param caseSensitive true if case sensitive, false otherwise
 */
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

/**
 * this function destroy the enrollment system and deallocate all the memory
 * @param sys enrollment system to destroy
 */
void destroyEnrollment(EnrollmentSystem sys)
{
    if(sys == NULL)
    {
        return;
    }

    destroyStudentsArray(sys->students,sys->studentsLen);
    destroyCoursesArray(sys->courses,sys->coursesLen);
    destroyHackersArray(sys->hackers,sys->hackersLen);
    free(sys);

}