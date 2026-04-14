#define _CRT_SECURE_NO_WARNINGS

#ifndef RECORD_H   
#define RECORD_H
#include"common.h"

// ====================== 就诊记录 ======================
int recordListBuild();
int createEmptyRecord(int patId, int docId);
Record* findRecordByrecId(int recId);
void recordSave(Record* head);
void recordFree(Record* head);
void deleteRecordNode(Record* target);

// ====================== 查询 ======================
void recordResearch();
void recordResearchByStatus();
void recordResearchByTime();
void adminSearchByPatient();
void adminSearchByDoctor();
void adminSearchByRecId();

// ====================== 批量删除 ======================
void adminDelete();
void adminDeleteByPatient();
void adminDeleteByDoctor();
void adminDeleteByRecId();
int printAndCountRecordByPatId(int patId);
int printAndCountRecordByDocId(int docId);
void batchDeleteRecord(int type, int id, int confirm);

// ====================== 修改======================
void adminModifyMenu();
void adminModifyPatient();
void adminModifyDoctor();
void adminModifyRecord();

// ====================== 患者功能 ======================
void patienCancel(int loginPatId, char* loginPatName);
void patientPayprint(Record* p);
void recordPay(int loginPatId, char* loginPatName);

// ====================== 报表 ======================
void recordReportMenu();
void recordReportDetail(Record* head);
void recordReportStatistics(Record* head);
void hospitalFeeReport();

// ====================== 工具函数 ======================
void getPatName(int patId, char* name);
void getDocName(int docId, char* name);
void getMedName(int medId, char* name);
void getBedInfo(int bedId, char* info);
void getDeptName(int deptId, char* name);
void printRecordItem(Record* r);
void printARecord(Record* rec);
int isValidRecord(Record* r);
int getMaxId();
void getInputTime(char* timeBuf, int bufSize);
const char* examTypeName(ExamType t);
Patient* findPatientById(int patId);
Record* showAndChoosePatientOfaDoc();
float getTotalPurchaseValue();
int calculateAge(const char* birthStr);

// ====================== 登录 ======================
int adminLogin();
void adminmain();


#endif

