#ifndef SINGLY_H
    #define SINGLY_H
    typedef struct Student
	{
	    int rollNo;
	    struct Student * next;
	}STUDENT_T;

    STUDENT_T * Create(const int n);
    void Insert(STUDENT_T ** head , const int pos , const int rollNo);
    STUDENT_T * Delete(STUDENT_T * head , const int pos);
    void Display(STUDENT_T * head);
    void Search(STUDENT_T * head , const int value);
    STUDENT_T * FreeLL(STUDENT_T *head);
    #endif
