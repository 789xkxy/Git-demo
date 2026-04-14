#define _CRT_SECURE_NO_WARNINGS
#ifndef DOCNDEPT_H
#define DOCNDEPT_H
#include "common.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef TIME_FORMAT
#define TIME_FORMAT "%04d-%02d-%02d %02d:%02d"
#endif

#define MAX_DOC_DAILY_RECEIVE  40
#define MAX_PAT_DAILY_REGISTER 4
#define MAX_MEDICINE_PER_TIME  10
#define EXAM_FEE               50

// 全局链表
Dept* deptList = NULL;
Doctor* doctorList = NULL;
Patient* patientList = NULL;
Medicine* medicineList = NULL;
Bed* bedList = NULL;
Exam* examList = NULL;
Record* recordList = NULL;

// 函数声明
void showMenu();
void addDoctor();
void delDoctor();
void modifyDoctor();
void showAllDoctor();
int isDocIdValid(int docId);
int isDocIdExist(int docId);
int isDeptIdValid(int deptId);
void doctorListBuild();
void doctorSave(Doctor* head);
void doctorFree(Doctor* head);

void getCurrentTime(char* str);
Doctor* findDoctorById(int docId);
Patient* findPatientById(int patId);
Medicine* findMedicineById(int medId);
Bed* findAvailableBed(void);
int countDoctorTodayReceive(int docId);
int doctorCanReceive(int docId);
int patientCanRegister(int patId);
Record* doctorRegister(int recId, int patId, int docId);
void doctorDiagnose(Record* r, const char* content);
Exam* doctorCreateExam(int examId, int patId, ExamType type, const char* item);
int doctorPrescribe(Record* r, int medId, int num);
int doctorArrangeHospital(Record* r);
void doctorFinishRecord(Record* r);
void doctorWholeFlow(int recId, int patId, int docId, const char* diag, ExamType examType, const char* examItem);

void showDoctorReport();
void showExpandPatientList();
void doctorReportMenu();
#endif