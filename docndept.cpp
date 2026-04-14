#define _CRT_SECURE_NO_WARNINGS
#include"docndept.h"
#include<stdlib.h>

// ==================== 主函数 ====================
int main() {
    doctorList = NULL;
    int choice;

    while (1) {
        printf("\n===== 主菜单 =====\n");
        printf("1. 医生信息管理\n");
        printf("2. 医生接诊全流程\n");
        printf("3. 医生报表与患者查询\n");
        printf("0. 退出\n");
        printf("请选择：");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("输入错误\n");
            continue;
        }
        getchar();

        switch (choice) {
            case 1:
                while (1) {
                    showMenu();
                    if (scanf("%d", &choice) != 1) {
                        while (getchar() != '\n');
                        printf("输入错误\n");
                        continue;
                    }
                    getchar();
                    if (choice == 0) break;

                    switch (choice) {
                        case 1: addDoctor(); break;
                        case 2: delDoctor(); break;
                        case 3: modifyDoctor(); break;
                        case 4: showAllDoctor(); break;
                        case 5: doctorListBuild(); break;
                        case 6: doctorSave(doctorList); break;
                        case 7: doctorFree(doctorList); break;
                        default: printf("无效选项\n");
                    }
                }
                break;

            case 2: {
                int recId, patId, docId, type;
                char diag[200], item[30];
                printf("记录ID："); scanf("%d", &recId);
                printf("患者ID："); scanf("%d", &patId);
                printf("医生ID："); scanf("%d", &docId);
                getchar();
                printf("诊断内容："); fgets(diag, 200, stdin);
                diag[strcspn(diag, "\n")] = 0;
                printf("检查类型(1=常规,2=生化,3=影像)："); scanf("%d", &type);
                getchar();
                printf("检查项目："); fgets(item, 30, stdin);
                item[strcspn(item, "\n")] = 0;

                doctorWholeFlow(recId, patId, docId, diag,(ExamType) type, item);
                break;
            }

            case 3:
                doctorReportMenu();
                break;

            case 0:
                doctorFree(doctorList);
                printf("程序退出\n");
                return 0;

            default:
                printf("无效选项\n");
        }
    }
}

// ==================== 医生管理 ====================
void showMenu() {
    printf("\n===== 医生信息管理 =====\n");
    printf("1. 添加医生\n");
    printf("2. 删除医生\n");
    printf("3. 修改医生信息\n");
    printf("4. 显示所有医生\n");
    printf("5. 从文件读取医生\n");
    printf("6. 保存医生到文件\n");
    printf("7. 释放医生链表\n");
    printf("0. 返回上一级\n");
    printf("请输入操作：");
}

int isDocIdValid(int docId) {
    return docId >= 2000 && docId <= 2999;
}

int isDeptIdValid(int deptId) {
    return deptId >= 3000 && deptId <= 3999;
}

int isDocIdExist(int docId) {
    Doctor* p = doctorList;
    while (p) {
        if (p->docId == docId) return 1;
        p = p->next;
    }
    return 0;
}

void addDoctor() {
    int docId;
    printf("医生编号(2000~2999)：");
    if (scanf("%d", &docId) != 1 || !isDocIdValid(docId)) {
        printf("格式或范围错误\n");
        return;
    }
    if (isDocIdExist(docId)) {
        printf("已存在\n");
        return;
    }

    int deptId;
    printf("科室ID(3000~3999)：");
    if (scanf("%d", &deptId) != 1 || !isDeptIdValid(deptId)) {
        printf("科室ID错误\n");
        return;
    }

    Doctor* newNode =(Doctor*) malloc(sizeof(Doctor));
    if (!newNode) return;
    newNode->docId = docId;
    newNode->deptId = deptId;
    newNode->next = NULL;

    printf("姓名："); scanf("%s", newNode->docName);
    printf("性别："); scanf("%s", newNode->docGender);

    if (!doctorList) doctorList = newNode;
    else {
        Doctor* p = doctorList;
        while (p->next) p = p->next;
        p->next = newNode;
    }
    printf("添加成功\n");
}

void delDoctor() {
    int docId;
    printf("输入医生编号：");
    if (scanf("%d", &docId) != 1) return;

    Doctor *p = doctorList, *pre = NULL;
    while (p) {
        if (p->docId == docId) {
            if (!pre) doctorList = p->next;
            else pre->next = p->next;
            free(p);
            printf("删除成功\n");
            return;
        }
        pre = p;
        p = p->next;
    }
    printf("未找到\n");
}

void modifyDoctor() {
    int docId;
    printf("医生编号：");
    if (scanf("%d", &docId) != 1) return;

    Doctor* p = doctorList;
    while (p) {
        if (p->docId == docId) {
            printf("新姓名："); scanf("%s", p->docName);
            printf("新性别："); scanf("%s", p->docGender);
            int newDept;
            printf("新科室ID：");
            scanf("%d", &newDept);
            if (isDeptIdValid(newDept)) p->deptId = newDept;
            printf("修改成功\n");
            return;
        }
        p = p->next;
    }
    printf("未找到\n");
}

void showAllDoctor() {
    if (!doctorList) { printf("无数据\n"); return; }
    printf("%-8s %-10s %-6s %-6s\n", "工号", "姓名", "性别", "科室ID");
    Doctor* p = doctorList;
    while (p) {
        printf("%-8d %-10s %-6s %-6d\n", p->docId, p->docName, p->docGender, p->deptId);
        p = p->next;
    }
}

void doctorListBuild() {
    char path[256];
    printf("文件路径：");
    scanf("%s", path);
    FILE* fp = fopen(path, "r");
    if (!fp) { perror("失败"); return; }

    Doctor *head = NULL, *tail = NULL;
    int id, dept;
    char name[20], gen[10];

    while (fscanf(fp, "%d %s %s %d", &id, name, gen, &dept) == 4) {
        if (!isDocIdValid(id) || !isDeptIdValid(dept) || isDocIdExist(id)) continue;
        Doctor* t = (Doctor*)malloc(sizeof(Doctor));
        t->docId = id; t->deptId = dept;
        strcpy(t->docName, name);
        strcpy(t->docGender, gen);
        t->next = NULL;
        if (!head) head = tail = t;
        else { tail->next = t; tail = t; }
    }
    fclose(fp);
    doctorFree(doctorList);
    doctorList = head;
    printf("读取完成\n");
}

void doctorSave(Doctor* head) {
    if (!head) { printf("空\n"); return; }
    char path[256];
    printf("保存路径："); scanf("%s", path);
    FILE* fp = fopen(path, "w");
    if (!fp) return;
    Doctor* p = head;
    while (p) {
        fprintf(fp, "%d %s %s %d\n", p->docId, p->docName, p->docGender, p->deptId);
        p = p->next;
    }
    fclose(fp);
    printf("保存成功\n");
}

void doctorFree(Doctor* head) {
    Doctor *p = head, *r;
    while (p) {
        r = p;
        p = p->next;
        free(r);
    }
    doctorList = NULL;
}

// ==================== 时间与接诊流程 ====================
void getCurrentTime(char *str) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    if (!tm || !str) return;
    sprintf(str, TIME_FORMAT, tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday, tm->tm_hour, tm->tm_min);
}

Doctor* findDoctorById(int docId) {
    Doctor* p = doctorList;
    while (p) { if (p->docId == docId) return p; p = p->next; }
    return NULL;
}

Patient* findPatientById(int patId) {
    Patient* p = patientList;
    while (p) { if (p->patId == patId) return p; p = p->next; }
    return NULL;
}

Medicine* findMedicineById(int medId) {
    Medicine* p = medicineList;
    while (p) { if (p->medId == medId) return p; p = p->next; }
    return NULL;
}

Bed* findAvailableBed() {
    Bed* p = bedList;
    while (p) { if (!strcmp(p->bedStatus, "空闲")) return p; p = p->next; }
    return NULL;
}

int countDoctorTodayReceive(int docId) {
    int cnt = 0;
    Record* p = recordList;
    while (p) { if (p->docId == docId) cnt++; p = p->next; }
    return cnt;
}

int doctorCanReceive(int docId) {
    Doctor* d = findDoctorById(docId);
    if (!d) return 0;
    return countDoctorTodayReceive(docId) < MAX_DOC_DAILY_RECEIVE;
}

int patientCanRegister(int patId) {
    Patient* p = findPatientById(patId);
    if (!p) return 0;
    return p->regCountToday < MAX_PAT_DAILY_REGISTER;
}

Record* doctorRegister(int recId, int patId, int docId) {
    if (!doctorCanReceive(docId) || !patientCanRegister(patId)) return NULL;
    Patient* pat = findPatientById(patId);
    if (!pat) return NULL;

    Record* r = (Record*)malloc(sizeof(Record));
    memset(r, 0, sizeof(Record));
    r->recId = recId;
    r->patId = patId;
    r->docId = docId;
    getCurrentTime(r->regTime);
    strcpy(r->recStatus, "挂号");
    pat->regCountToday++;
    r->next = recordList;
    recordList = r;
    return r;
}

void doctorDiagnose(Record* r, const char* content) {
    if (!r || !content) return;
    strncpy(r->diagContent, content, 199);
    strcpy(r->recStatus, "看诊中");
}

Exam* doctorCreateExam(int examId, int patId, ExamType type, const char* item) {
    Exam* e =(Exam*) malloc(sizeof(Exam));
    memset(e, 0, sizeof(Exam));
    e->examId = examId;
    e->patId = patId;
    e->type = type;
    strncpy(e->itemName, item, 29);
    getCurrentTime(e->examTime);
    e->fee = EXAM_FEE;
    e->next = examList;
    examList = e;
    return e;
}

int doctorPrescribe(Record* r, int medId, int num) {
    if (!r || num < 1 || num > 10) return 0;
    Medicine* m = findMedicineById(medId);
    if (!m || m->medStock < num) return 0;
    m->medStock -= num;
    r->needMedicine = 1;
    r->medId = medId;
    r->medNum = num;
    return 1;
}

int doctorArrangeHospital(Record* r) {
    if (!r) return 0;
    Bed* bed = findAvailableBed();
    if (!bed) return 0;
    bed->patId = r->patId;
    strcpy(bed->bedStatus, "占用");
    r->needHospital = 1;
    r->bedId = bed->bedId;
    getCurrentTime(r->startTime);
    return 1;
}

void doctorFinishRecord(Record* r) {
    if (r) strcpy(r->recStatus, "诊疗结束");
}

void doctorWholeFlow(int recId, int patId, int docId, const char* diag, ExamType examType, const char* examItem) {
    int op, mid, mnum;
    static int eid = 1000;
    Record* r = doctorRegister(recId, patId, docId);
    if (!r) return;
    doctorDiagnose(r, diag);
    Exam* e = doctorCreateExam(eid++, patId, examType, examItem);
    if (e) r->examId = e->examId;

    printf("开药(1/0)：");
    scanf("%d", &op);
    if (op == 1) {
        printf("药品ID："); scanf("%d", &mid);
        printf("数量："); scanf("%d", &mnum);
        doctorPrescribe(r, mid, mnum);
    }

    printf("住院(1/0)：");
    scanf("%d", &op);
    if (op == 1) doctorArrangeHospital(r);
    doctorFinishRecord(r);
}

// ==================== 报表 ====================
void showDoctorReport() {
    printf("\n===== 医生工作报表 =====\n");
    printf("ID  姓名   科室ID  总接诊 待诊  开药  住院\n");
    Doctor* d = doctorList;
    while (d) {
        int total = 0, wait = 0, med = 0, hosp = 0;
        Record* r = recordList;
        while (r) {
            if (r->docId == d->docId) {
                total++;
                if (!strcmp(r->recStatus, "挂号")) wait++;
                if (r->needMedicine) med++;
                if (r->needHospital) hosp++;
            }
            r = r->next;
        }
        printf("%-4d %-6s %-6d %-6d %-4d %-4d %-4d\n",
               d->docId, d->docName, d->deptId, total, wait, med, hosp);
        d = d->next;
    }
}

void showExpandPatientList() {
    Patient* p = patientList;
    while (p) {
        printf("患者ID:%d 姓名:%s 电话:%s\n", p->patId, p->patName, p->patPhone);
        int op;
        printf("1查看详情 0跳过：");
        scanf("%d", &op);
        if (op == 1) {
            Record* r = recordList;
            while (r) {
                if (r->patId == p->patId) {
                    printf("记录:%d 医生:%d 状态:%s 开药:%d 住院:%d\n",
                           r->recId, r->docId, r->recStatus, r->needMedicine, r->needHospital);
                }
                r = r->next;
            }
        }
        p = p->next;
    }
}

void doctorReportMenu() {
    int op;
    while (1) {
        printf("\n1 医生报表 | 2 患者展开 | 0 返回\n");
        scanf("%d", &op);
        if (op == 0) break;
        if (op == 1) showDoctorReport();
        if (op == 2) showExpandPatientList();
    }
}
