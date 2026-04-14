#define _CRT_SECURE_NO_WARNINGS
#ifndef COMMON_H
#define COMMON_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


// ---------------- 题目要求的容量约定 ----------------


// 时间格式统一要求
#define TIME_FORMAT     "%04d-%02d-%02d %02d:%02d"

// ---------------- 1. 科室链表 ----------------
typedef struct Department {
    int deptId;                 // 科室编号（唯一）
    char deptName[30];          // 科室名称（内科/外科/妇产/传染/急诊等）
    struct Department* next;
} Dept;

// ---------------- 2. 医生链表 ----------------
typedef struct Doctor {
    int docId;                  // 工号（唯一）
    char docName[20];           // 姓名
    char docGender[10];         // 性别
    int deptId;                 // 所属科室ID
    struct Doctor* next;
} Doctor;

// ---------------- 3. 患者链表 ----------------
typedef struct Patient {
    int patId;                  // 患者编号（唯一）
    char patName[20];           // 姓名
    char patGender[10];         // 性别
    char patBirth[30];          // 出生日期（字符串）
    int regCountToday;          //今日已挂号数
    char patPhone[20];          // 联系方式
    // 医疗记录通过ID关联，不直接存储
    struct Patient* next;
} Patient;

// ---------------- 4. 药品链表 ----------------
#define MAX_NAME_LEN 50
#define MAX_ALIAS_LEN 100
#define MEDICINE_ID_START 1001
#define MAX_PATIENT     100     // 患者最多100位
#define MAX_INPATIENT   33      // 住院患者最多33位
#define MAX_DOCTOR      20      // 医生最多20位
#define MAX_DEPT        5       // 5个科室
#define MAX_BED_TYPE    3       // 3种病房类型：普通/重症/传染
#define MAX_MEDICINE    60      // 药品最多60种
#define MAX_BED         33      // 床位总数

// 时间格式统一要求
#define TIME_FORMAT     "%04d-%02d-%02d %02d:%02d"
typedef struct Medicine {
    int medId;                                //药品编号 
    char medTrade[MAX_NAME_LEN];              //商品名 
    char medGeneric[MAX_NAME_LEN];            //通用名 
    char medAlias[MAX_ALIAS_LEN];             //别名 
    int deptId;                               //科室ID 
    int medStock;                             //库存 
    int outStock;                             //出库数量 
    float purchasePrice;                      // 进价
    float sellPrice;                          // 售价
    struct Medicine* next;
} Medicine;
// ---------------- 5. 床位链表 ----------------
typedef struct Bed {
    int bedId;                  // 床位编号
    int bedType;                // 病房类型：1=普通 2=重症 3=传染
    int deptId;                 // 所属科室
    int patId;                  // 占用患者ID（0表示空闲）
    char bedStatus[10];         // 状态：空闲/占用
    struct Bed* next;
} Bed;

// ---------------- 6. 检查链表 ----------------
typedef enum {
    EXAM_NORMAL=1,//常规
    EXAM_BIOCHEM,//生化
    EXAM_IMAGE//影像
}ExamType;
typedef struct Exam {
    int examId;       // 检查ID
    char examCode[5];     // 检查编码
    int patId;        // 患者ID
    char examTime[20];    // 检查时间
    ExamType type;        // 检查类型（常规/生化/影像）
    char itemName[30];    // 检查名称
    char result[10];      // 检查结果
    int fee;              // ✅ 检查费用（50元）
    struct Exam* next;
} Exam;
// ---------------- 7. 就诊记录链表（核心） ----------------
typedef struct Record {
    int recId;                  // 记录编号

    // 三部分信息：患者+医生+诊疗
    int patId;                  // 患者ID
    int docId;                  // 医生ID

    // 挂号部分
    char regTime[30];          // 挂号时间

    // 看诊部分
    char diagContent[200];     // 诊断内容

    // 检查部分
    int examId;                // 关联检查ID（0表示未开检查）

    // 复诊部分
    int needMedicine;          // 是否开药（0/1）
    int medId;                 // 药品ID（0表示未开药）
    int medNum;                // 开药数量（最多10盒）
    int needHospital;          // 是否住院（0/1）

    // 住院部分
    int bedId;                 // 住院床位ID
    char startTime[30];        // 住院开始时间


    // 状态标识
    char recStatus[30];        // 状态：挂号/住院/诊疗结束
    // ==================== 只加这 4 行，最小改动 ====================
    float feeReg;         // 挂号费（固定 15~50 元都行）
    float feeExam;        // 检查费（Exam 里已有 fee，直接拿）
    float feeMedicine;    // 药品费 = 药品单价 * 数量
    float feeTotal;       // 总费用（自动计算）
    int payed;
    struct Record* next;
} Record;

// ---------------- 全局链表声明（extern） ----------------
extern Dept* deptList;
extern Doctor* doctorList;
extern Patient* patientList;
extern Medicine* medicineList;
extern Bed* bedList;
extern Exam* examList;
extern Record* recordList;

#endif
