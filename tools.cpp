
#include"common.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
//1.查名字
//1.1根据患者id-》取患者姓名
void getPatName(int patId, char* name) {
    Patient* p = patientList;
    while (p) {
        if (p->patId == patId) {
            strcpy(name, p->patName);
            return;
        }
        p = p->next;
    }
    strcpy(name, "未知");
}

// 1.2 根据医生ID → 取医生姓名
void getDocName(int docId, char* name) {
    Doctor* d = doctorList;
    while (d) {
        if (d->docId == docId) {
            strcpy(name, d->docName);
            return;
        }
        d = d->next;
    }
    strcpy(name, "未知");
}

// 1.3 根据药品ID → 取药品名
void getMedName(int medId, char* name) {
    Medicine* m = medicineList;
    while (m) {
        if (m->medId == medId) {
            strcpy(name, m->medTrade);
            return;
        }
        m = m->next;
    }
    strcpy(name, "无");
}

// 1.4 根据床位ID → 取床位号/描述
void getBedInfo(int bedId, char* info) {
    Bed* b = bedList;
    while (b) {
        if (b->bedId == bedId) {
            strcpy(info, b->bedStatus);
            return;
        }
        b = b->next;
    }
    strcpy(info, "无");
}
// 1.5 根据检查ID → 取检查项名称
void getExamName(int examId, char* name) { 

}
// 1.6 根据医生ID → 取科室名称
void getDeptName(int deptId, char* name) {
    Dept* d = deptList;
    while (d) {
        if (d->deptId == deptId) {
            strcpy(name, d->deptName);
            return;
        }
        d = d->next;
    }
    strcpy(name, "未知科室");
}
//2.计算年龄
// 辅助函数：根据出生日期字符串计算年龄
// 假设 patBirth 格式为 "YYYY-MM-DD" 或类似包含年份的格式
// 辅助函数：根据出生日期字符串计算年龄
// 假设 patBirth 格式为 "YYYY-MM-DD" 或类似包含年份的格式
int calculateAge(const char* birthStr) {
    if (birthStr == NULL || strlen(birthStr) < 4) return 0;

    int birthYear = atoi(birthStr); // 简单提取前4位作为年份
    if (birthYear <= 0) return 0;

    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    int currentYear = t->tm_year + 1900;

    return currentYear - birthYear;
}// 3.自动计算当前就诊记录的费用（通用工具函数）
void calcRecordFee(Record* r)
{
    // 1. 挂号费（固定）
    r->feeReg = 20.0f;

    // 2. 检查费（有检查就收50）
    r->feeExam = (r->examId > 0) ? 50.0f : 0.0f;

    // 3. 药品费（有开药就计算）
    r->feeMedicine = 0.0f;
    if (r->needMedicine && r->medId > 0 && r->medNum > 0) {
        Medicine* m = medicineList;
        while (m) {
            if (m->medId == r->medId) {
                r->feeMedicine = m->sellPrice * r->medNum;
                break;
            }
            m = m->next;
        }
    }

    // 4. 总费用
    r->feeTotal = r->feeReg + r->feeExam + r->feeMedicine;
}
float getTotalPurchaseValue() {
    float total = 0;
    Medicine* p = medicineList;
    while (p) {
        total += p->purchasePrice * p->medStock;
        p = p->next;
    }
    return total;
}
Dept* deptList = NULL;
Doctor* doctorList = NULL;
Patient* patientList = NULL;
Medicine* medicineList = NULL;
Bed* bedList = NULL;
Exam* examList = NULL;
Record* recordList = NULL;