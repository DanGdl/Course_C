/*
compile: gcc -Wall -Wextra -Wpedantic ex11_custom_types.c -o ex11   OR: gcc ex11_custom_types.c -o ex11
run: ./ex11
*/

// enums, structs, unions, macroses
#include <stdio.h>
#include <inttypes.h>

#define UNUSED(x) ((void) x)


typedef enum WeekDay {
    WD_NONE = -1, WD_SUN, WD_MON, WD_TUE, WD_WEN, WD_THU, WD_FRI, WD_SAT = 10
} WeekDay_t;


typedef struct Birthday {
    uint32_t year;
    uint8_t month;
    uint8_t day;
} Birthday_t;

typedef struct Person {
    // struct Birthday birthday;
    Birthday_t birthday;
    char* name;
} Person_t;

typedef struct Course {
    char* name;
    uint8_t grade;
} Course_t;

typedef struct Student {
    Person_t person;
    Course_t course;
    /* // anonimous struct
    struct {
        char* name;
        uint8_t grade;
    } course;
    */
} Student_t;


#define CTR_PERSON(name_, y_, m_, d_) { .birthday = { .year = y_, .month = m_, .day = d_, }, .name ="<" #name_ ">", }
#define CTR_COURSE(name_, grade_) { .name = name_, .grade = grade_, }


#define ARRAY(name, dtype) \
typedef struct name##Array { \
   unsigned long int cap; \
   unsigned long int len; \
   dtype* values; \
} name##Array_t;


ARRAY(Double, double)
ARRAY(Student, Student_t)

typedef union F2I {
    float val_f;
    uint32_t val_u;
    int32_t val_i;
} F2I_t;

void student_print(const Student_t* const s) {
    if (s == NULL) return;
    printf("Student: Name %s, birthday %u.%u.%u, Course %s, grade %u\n",
        s->person.name, s->person.birthday.year, s->person.birthday.month,
        s->person.birthday.day, s->course.name, s->course.grade
    );
}

int main(void) {
    printf("Value of WD_NONE %d, WD_SUN %d, WD_MON %d, WD_TUE %d, WD_WEN %d, WD_THU %d, WD_FRI %d, WD_SAT %d\n",
        WD_NONE, WD_SUN, WD_MON, WD_TUE, WD_WEN, WD_THU, WD_FRI, WD_SAT
    );
    Student_t s1 = {
        .person = CTR_PERSON("Dude", 1990, 10, 5),
        .course = {
            .name = "Math",
            .grade = 90,
        },
    };
    student_print(&s1);
    Student_t* s1_ptr = &s1;

    printf("\nSizeof Student_t %lu bytes\n", sizeof(s1));
    printf("Sizeof Person_t %lu bytes\n", sizeof(s1.person));
    printf("Sizeof Course_t %lu bytes\n", sizeof(s1.course));

    printf("Address of s1 %p\n",                        (void*) s1_ptr);
    printf("Address of s1.person %p\n",                 (void*) &s1_ptr->person);
    printf("Address of s1.person.birthday %p\n",        (void*) &s1_ptr->person.birthday);
    printf("Address of s1.person.birthday.year %p\n",   (void*) &s1_ptr->person.birthday.year);
    printf("Address of s1.person.birthday.month %p\n",  (void*) &s1_ptr->person.birthday.month);
    printf("Address of s1.person.birthday.day %p\n",    (void*) &s1_ptr->person.birthday.day);
    printf("Address of s1.person.name %p\n",            (void*) &s1_ptr->person.name);

    printf("Address of s1.course %p\n",                 (void*) &s1_ptr->course);
    printf("Address of s1.course.name %p\n",            (void*) &s1_ptr->course.name);
    printf("Address of s1.course.grade %p\n",           (void*) &s1_ptr->course.grade);
    
    DoubleArray_t arr = { 0 };
    StudentArray_t arr1 = { 0 };
    UNUSED(arr);
    UNUSED(arr1);

    F2I_t f2i = { 0 };
    f2i.val_f = 2.5;
    printf("\nF2I float val %f, integer val %d, unsigned integer %u (0x%X)\n", f2i.val_f, f2i.val_i, f2i.val_u, f2i.val_u);


    // packing
    typedef struct BitMask1 {
        uint8_t flag1 :1;
        uint8_t flag2 :1;
        uint8_t flag3 :1;
        uint8_t flag4 :1;
    } BitMask1_t;

    printf("Sizeof BitMask1_t %lu bytes\n", sizeof(BitMask1_t));

    // alingment
    typedef struct Example1 {
        uint64_t flag1;
        uint8_t flag2;
        void* flag6;
        uint32_t flag3;
        uint16_t flag4;
        uint64_t flag5;
    } Example1_t;

    typedef struct Example2 {
        uint64_t flag1;
        uint64_t flag5;
        void* flag6;
        uint32_t flag3;
        uint16_t flag4;
        uint8_t flag2;
    } Example2_t;

    printf("Sizeof Example1_t %lu bytes\n", sizeof(Example1_t));
    printf("Sizeof Example2_t %lu bytes\n", sizeof(Example2_t));


#pragma pack(push, 1)
    typedef struct Birthday2 {
        uint32_t year;
        uint8_t month;
        uint8_t day;
    } Birthday2_t;

    typedef struct Person2 {
        Birthday2_t birthday;
        char* name;
    } Person2_t;

    typedef struct Course {
        char* name;
        uint8_t grade;
    } Course2_t;

    typedef struct Student2 {
        Person2_t person;
        Course2_t course;
    } Student2_t;

    // init with zeroes
    Student2_t s = { 0 };
    printf("\nSizeof Student2_t %lu bytes\n", sizeof(s));
    printf("Sizeof Person2_t %lu bytes\n", sizeof(s.person));
    printf("Sizeof Course2_t %lu bytes\n\n", sizeof(s.course));

    printf("Address of s %p\n",                        (void*) &s);
    printf("Address of s.person %p\n",                 (void*) &s.person);
    printf("Address of s.person.birthday %p\n",        (void*) &s.person.birthday);
    printf("Address of s.person.birthday.year %p\n",   (void*) &s.person.birthday.year);
    printf("Address of s.person.birthday.month %p\n",  (void*) &s.person.birthday.month);
    printf("Address of s.person.birthday.day %p\n",    (void*) &s.person.birthday.day);
    printf("Address of s.person.name %p\n",            (void*) &s.person.name);

    printf("Address of s.course %p\n",                 (void*) &s.course);
    printf("Address of s.course.name %p\n",            (void*) &s.course.name);
    printf("Address of s.course.grade %p\n",           (void*) &s.course.grade);
#pragma pack(pop)

    return 0;
}
