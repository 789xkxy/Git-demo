#define _CRT_SECURE_NO_WARNINGS
#include"common.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "record.h"
//第一部分：文件部分
//1.文件读取与创建链表
int isValidRecord(Record* r) {//判断一条记录是否合法
    //Id不为负数，开药数量不为负数，开关只能是0或1
    if (r->recId < 0 || r->patId < 0 || r->docId < 0)return 0;
    if (r->examId < 0 || r->medId < 0 || r->bedId < 0)return 0;
    if (r->needMedicine < 0 || r->needMedicine>1)return 0;
    if (r->needHospital < 0 || r->needHospital>1)return 0;
    if (r->medNum < 0)return 0;
    return 1;

}
int recordListBuild() {
	char filePath[256];
    FILE* fp = NULL;
    while (1) {
        printf("请输入就诊记录文件路径：\n");
        scanf("%s", filePath);
        fp = fopen(filePath, "r");
        if (fp != NULL) break;
        perror("文件读取失败,请重试！\n");
            
        system("pause");
    }
	Record* head, * p;
	head= p = NULL;
    int line = 0;
    while(1){
        line++;
        int recId, patId, docId, examId;
        int needMedicine, medId, medNum, needHospital, bedId;
        char regTime[30], diagContent[200], startTime[30], recStatus[30];
        int res=fscanf(fp, "%d %d %d %s %s %d %d %d %d %d %d %s %s", &recId, &patId, &docId, regTime, diagContent, &examId,& needMedicine, &medId, &medNum,& needHospital, &bedId, startTime, recStatus);
        if (res == EOF)break;//文件读完了
        if (res != 13) {
            printf("第%d行数据格式错误！退出读取！\n",line);
            fclose(fp);
            return 0 ;
        }
        Record*tmp = (Record*)malloc(sizeof(Record));
        tmp->recId = recId;
        tmp->patId = patId;
        tmp->docId = docId;
        strcpy(tmp->regTime, regTime);
        strcpy(tmp->diagContent,diagContent);
        tmp->examId = examId;
        tmp->needMedicine = needMedicine;
        tmp->medId = medId;
        tmp->medNum = medNum;
        tmp->needHospital = needHospital;
        tmp->bedId = bedId;
        strcpy(tmp->startTime, startTime);
        strcpy(tmp->recStatus, recStatus);
        tmp->next = NULL;
        if (!isValidRecord(tmp)) {
            printf("第%d行数据非法！跳过该记录。\n",line);
            free(tmp);
            continue;
        }
        if (head == NULL) {
            head = tmp; p = head;
        }
        else {
            p->next = tmp; p = tmp;
        }
    }
    fclose(fp);
    printf("文件读取成功！\n");
    recordList = head;
    return 1;
}
// 2.文件写入与保存
void recordSave(Record* head) {//存储
    if (head == NULL) {
        printf("链表无数据，无需保存\n");
        return;
    }
    printf("请输入你要保存文件的路径：\n");
    char filePath[256];
    scanf("%s", filePath);
    FILE* fp = fopen(filePath, "w");
    if (fp == NULL) {
        printf("文件保存失败\n");
        perror(filePath);
        return;
    }//文档打开检验
    Record* p = head;
    int w_ok = 1;
    while (p != NULL) {
        int res = fprintf(fp, "%d %d %d %s %s %d %d %d %d %d %d %s %s", p->recId, p->patId, p->docId, p->regTime, p->diagContent, p->examId, p->needMedicine, p->medId, p->medNum, p->needHospital, p->bedId, p->startTime, p->recStatus);
        if (res < 0||!isValidRecord(p)) {
            printf("文件写入失败！\n");
            w_ok = 0;
            break;
        }//文档写入成功检验
        p = p->next;

    }
    fclose(fp);//关闭
    if (w_ok)
        printf("链表数据已成功保存到：%s\n", filePath);
    else printf("文件保存失败！\n");
    return;

}
// 3.释放链表
void recordFree(Record* head) {
    Record* p = head;
    while (p != NULL) {
        Record* r = p;
        p = p->next;
        free(r);
    }
    printf("链表已释放\n");//释放
}
//第二部分：就诊记录的增删改查与打印
//1.增加一个空链表：此部分用于挂号流程中，从挂号处读取patId&docId建立一个空的就诊记录

int getMaxId() {//得到目前最大的就诊记录编号，避免重复
    int maxId = 70000;
    Record* p = recordList;
    while (p != NULL) {
        if (p->recId > maxId)maxId = p->recId;
        p = p->next;
    }
    return maxId;
}
int createEmptyRecord(int patId, int docId) {
    Record* newNode = (Record*)malloc(sizeof(Record));
    newNode->patId = patId;
    newNode->docId = docId;
    newNode->examId = 0;
    newNode->medId = 0;
    newNode->bedId = 0;
    newNode->needMedicine = 0;
    newNode->needHospital = 0;
    newNode->medNum = 0;
    strcpy(newNode->recStatus, "挂号");
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    strftime(newNode->regTime, 30, "%T-%m-%d %H:%M", t);
    int maxId = getMaxId();
    newNode->recId = maxId + 1;//新建的就诊记录编号大于之前任何一个
    newNode->next=recordList;
    recordList = newNode;
    return newNode->recId;
}
//2.给医生做一个showPatient函数，支持他查看与选择要接待的患者，并把这个节点传给医生要做的modify函数
Patient* findPatientById(int patId) {
    Patient* p = patientList;
    while (p != NULL) {
        if (p->patId == patId) {
            return p;
        }
        p = p->next;
    }
    
    return NULL;
}
Record* showAndChoosePatientOfaDoc() {
    printf("请输入医生Id：\n");
    int docId;
    scanf("%d", &docId);
    Record* patList[10];
    int count = 0;
    Record* p = recordList;
    while (p != NULL) {
        if (p->docId == docId&&strcmp(p->recStatus,"挂号")==0 ){
            patList[count++] = p;
        }
        p = p->next;
    }
    if (count == 0) {
        printf("当前没有待接诊的患者！\n");
        return NULL;
    }
    printf("======今日您的挂号列表======\n");
    for (int i = 0; i < count; i++) {
        printf("%d.就诊记录编号：%d\t状态：%s\n", i + 1, patList[i]->recId,  patList[i]->recStatus);
        int patId = patList[i]->patId;
        Patient* pat= findPatientById(patId);
        if (pat != NULL) {
            printf("患者ID：%d\n", pat->patId);
            printf("患者姓名：%s\n", pat->patName);
            printf("患者ID：%s\n", pat->patGender);
            printf("患者ID：%s\n", pat->patBirth);
            printf("患者ID：%s\n", pat->patPhone);
        }
        else printf("未找到该患者\n");
    }
    int choice;
    while (1) {
        choice = 0;
        printf("请选择要接诊的患者序号：\n");
        scanf("%d", &choice);
        if (choice >= 1 && choice <= count) {
            break;
        }
        printf("选择无效！请重新选择。\n");
    }
    return patList[choice - 1];
}
//3.删除一个就诊记录：按照患者视角和管理员视角分为两个功能
//患者视角：自助退号系统——可以按照就诊id/患者信息（患者id及姓名）退掉状态为“挂号”的号
//管理员视角：登录需密码，可以按照就诊记录id单独删除或者患者信息，医生信息批量删除就诊记录（无论状态)
//所有删除均需二次确认

//(1)患者视角：自助退号系统——只能退自己的号+挂号状态的号
//1.1患者基本工具：按就诊ID查&删除一个节点
Record* findRecordByrecId(int recId) {
    Record* p = recordList;
    while (p != NULL) {
        if (p->recId == recId)return p;
        p = p->next;
    }
    printf("该就诊记录不存在！\n");
    return NULL;
}
void deleteRecordNode(Record* target) {
    if (target == NULL)return;
    Record* p = recordList, * pre = NULL;
    while (p != NULL && p != target) {
        pre = p; p = p->next;
    }
    if (pre == NULL&&p!=NULL)recordList = p->next;
    else pre->next = p->next;
    free(p);
    return;
}
//1.2 核心函数
void patienCancel(int loginPatId,char* loginPatName) {
    printf("\n======患者自助退号======\n");
    int opt;
    while (1) {
        printf("1.按就诊记录编号退号\n");
        printf("2.按照患者ID及姓名退号\n");
        printf("3.放弃本次删除\n");
        printf("请输入退号方式序号：\n");
        if (scanf("%d", &opt) == 1 && opt >= 1 && opt <= 3) {
            break;
        }
        printf("输入序号错误！请重试。\n");
    }
    Record* target = NULL;
    int patId;
    char patName[20];
    if (opt == 1) {
        int recId;
        printf("请输入就诊记录编号：\n");
        scanf("%d", &recId);
        target = findRecordByrecId(recId);
        if (target == NULL) {
            printf("未找到记录！");
            return;
        }
        if (target->patId != loginPatId) {
            printf("仅允许退自己的号！\n");
            return;
        }
        system("pause");
    }
    
    else if (opt == 2) {
        
        printf("请输入患者ID：\n");
        scanf("%d", &patId);
        printf("请输入患者姓名：\n");
        scanf("%s", patName);
        if (patId!= loginPatId || strcmp(patName, loginPatName) != 0) {
            printf("身份验证失败！\n");
            return;
        }
        Record* p = recordList;
        while (p != NULL) {
            if (p->patId == patId) {
                target = p; break;
            }
            p = p->next;
        }
        if (target == NULL) {
            printf("无记录可退！\n");
            return;
        }
    }
    else if (opt == 3) {
        printf("本次删除已放弃!\n"); return;
    }
    if (strcmp(target->recStatus, "挂号") != 0) {
        printf("只有“挂号”状态的号可以退！\n");
        return;
    }
    printf("\n——退号确认——\n");
    char patname[20];
    getPatName(patId, patname);
    char docname[20];
    getDocName(target->docId, docname);
    printf("就诊ID：%d\n患者：%s\nj接诊医生：%s\n状态：%s\n挂号时间：%s\n",
        target->recId, patname, docname, target->recStatus, target->regTime);
    char c;
    printf("确认退号？(Y/N)\n");
    scanf("%c", &c);
    if (c == 'Y' || c == 'y') {
        deleteRecordNode(target);
        printf("退号成功！\n");
    }
    else {
        printf("已取消！\n");
    }
    return;

}
//(2)管理员视角：删除任意状态下的就诊记录+批量删除某位医生/患者的所有诊疗记录
//2.1 管理员基本函数:管理员需要密码登录
int adminLogin() {
    char pwd[20];
    printf("请输入管理员密码：\n");
    scanf("%s", pwd);
    if (strcmp(pwd, "123456") == 0)return 1;
    return 0;
}
//2.2管理员删除功能函数——1.按recId删一个节点 2.删一个患者的全部就诊记录 3.删一个医生的全部就诊记录
int printAndCountRecordByPatId(int patId) {
    Record* p = recordList;
    int count = 0;
    printf("======待删除患者就诊记录预览======\n");
    while (p != NULL) {
        if (p->patId == patId) {
            char patname[20];
            getPatName(patId, patname);
            char docname[20];
            getDocName(p->docId, docname);
            printf("就诊ID：%d\n患者：%s\n接诊医生：%s\n状态：%s\n记录生成时间：%s\n",
                p->recId, patname, docname, p->recStatus, p->regTime);
            count++;
        }
        p = p->next;
    }
    if (count == 0) {
        printf("未找到该患者的任何就诊记录。\n");
}
    else {
        printf("====================\n");
        printf("共找到%d条记录。\n",count);
    }
    return count;
}
int printAndCountRecordByDocId(int docId) {
    Record* p = recordList;
    int count = 0;
    printf("======待删除患者就诊记录预览======\n");
    while (p != NULL) {
        if (p->docId == docId) {
            char patname[20];
            getPatName(p->patId, patname);
            char docname[20];
            getDocName(docId, docname);
            printf("就诊ID：%d\n患者：%s\n接诊医生：%s\n状态：%s\n记录生成时间：%s\n",
                p->recId, patname, docname, p->recStatus, p->regTime);
            count++;
        }
        p = p->next;
    }
    if (count == 0) {
        printf("未找到该医生的任何接诊记录。\n");
    }
    else {
        printf("====================\n");
        printf("共找到%d条记录。\n", count);
    }
    return count;
}
//核心批量删除函数：根据条件删除，confirm为1时执行删除，为0时仅预览
void batchDeleteRecord(int type, int id, int confirm) {
    //type:1.患者2.医生
    Record* p = recordList;
    Record* pre = NULL;
    int deletedCount = 0;
    while (p != NULL){
        int flag = 0;//删除标记
        if (type == 1 && p->patId == id)flag = 1;
        if (type == 2 && p->docId == id)flag = 1;
        if (flag&&confirm) {
            deleteRecordNode(p);
                p = pre->next;
                deletedCount++;
        }
        else {
            pre = p;
            p = p->next;
        }
    }
    if (confirm) {
        printf("已成功删除%d条记录。\n",deletedCount);
    }
}
void adminDeleteByRecId() {
    printf("请输入待删除就诊记录编号：\n");
    int recId;
    scanf("%d",& recId);
    Record* p = findRecordByrecId(recId);
    deleteRecordNode(p);
    printf("已删除该条就诊记录。\n");
    return;

}
void adminDeleteByPatient() {
    int patId;
    while (1) {
        printf("请输入待删除患者ID：\n");
        if (scanf("%d", &patId) == 1) {
            while (getchar() != '\n');
            break;
        }
        while (getchar() != '\n');//清除输入缓冲区防止死循环
        printf("输入错误！请重试。\n");
    }
    //1.预览
    int count = printAndCountRecordByPatId(patId);
    if (count == 0) {
        printf("本次删除已放弃！\n");
        return;
    }
    //2.确认
    char confirm;
    printf("确认删除该患者所有就诊记录？(Y/N)\n");
    scanf("%c", &confirm);
    if (confirm == 'Y' || confirm == 'y') {
        batchDeleteRecord(1, patId, 1);
    }
    else printf("已取消删除操作。\n");
    return;
    
}
void adminDeleteByDoctor() {
    int docId;
    while (1) {
        printf("请输入待删除医生ID：\n");
        if (scanf("%d", &docId) == 1) {
            while (getchar() != '\n');
            break;
        }
        while (getchar() != '\n');//清除输入缓冲区防止死循环
        printf("输入错误！请重试。\n");
    }
    //1.预览
    int count = printAndCountRecordByPatId(docId);
    if (count == 0) {
        printf("本次删除已放弃！\n");
        return;
    }
    //2.确认
    char confirm;
    printf("确认删除该医生所有接诊记录？(Y/N)\n");
    scanf("%c", &confirm);
    if (confirm == 'Y' || confirm == 'y') {
        batchDeleteRecord(2, docId, 1);
    }
    else printf("已取消删除操作。\n");
    return;

}

//2.3 管理员删除核心函数
void adminDelete() {
    
    int op;
    printf("\n====== 管理员删除系统 ======\n");
    while (1) {
        printf("1. 按就诊记录编号单条删除\n");
        printf("2. 按患者ID批量删除\n");
        printf("3. 按医生ID批量删除\n");
        printf("4.放弃本次删除\n");
        printf("请输入删除方式序号：\n");
        if (scanf("%d", &op) == 1 && op <= 4 && op >= 1) {
            break;
        }
        while (getchar ()!= '\n');//清除输入缓冲区防止死循环
        printf("输入序号错误！请重试。\n");
    }
    switch (op) {
    case 1:adminDeleteByRecId(); system("pause"); break;
    case 2:adminDeleteByPatient();  system("pause"); break;
    case 3:adminDeleteByDoctor();  system("pause"); break;
    default:printf("本次删除已放弃!\n");  system("pause"); break;
    }
}
//4.修改诊疗记录与医生接诊流程相似，到时候引用就行，不必重写
//5.查询功能：本处查询功能从医院视角出发，
//            进行1.就诊id查询（基本操作）2.状态查询3.时间查询
//5.1详细打印一个就诊记录
// 辅助函数：将 ExamType 枚举转换为字符串
const char* examTypeName(ExamType t) {
    if (t == EXAM_NORMAL) return "常规检查";
    if (t == EXAM_BIOCHEM) return "生化检查";
    return "影像检查";
}
void printARecord(Record*rec) {
    if (rec == NULL) {
        printf("错误：记录指针为空！\n");
        return;
    }
    char patName[20] = { 0 };
    char docName[20] = { 0 };
    char deptName[30] = { 0 };
    char medName[30] = { 0 };
    char bedInfo[30] = { 0 };
    char examInfo[50] = { 0 };
    //1.基础信息
    getPatName(rec->patId, patName);
    getDocName(rec->docId, docName);
    //2.科室
    Doctor* doc = doctorList;
    int deptId = -1;
    while (doc) {
        if (doc->docId == rec->docId) {
            deptId = doc->deptId;
            break;
        }
        doc = doc->next;
    }

    if (deptId > 0) {
        getDeptName(deptId, deptName);
    }
    else {
        strcpy(deptName, "未分配科室");
    }
    //3.年龄性别
    Patient* p = patientList;
    char gender[10] = "未知";
    char birth[30] = "未知";
    while (p) {
        if (p->patId == rec->patId) {
            strcpy(gender, p->patGender);
            strcpy(birth, p->patBirth);
            break;
        }
        p = p->next;
    }
    int age = calculateAge(birth);
    //4.药品，床位，检查
    if (rec->needMedicine == 1 && rec->medId > 0) {
        getMedName(rec->medId, medName);
    }
    else {
        strcpy(medName, "无");
    }

    if (rec->needHospital == 1 && rec->bedId > 0) {
        // 这里 getBedInfo 返回的是状态，如果需要床位号，可能需要修改 getBedInfo 或单独查找
        // 假设我们想显示床位ID和状态
        Bed* b = bedList;
        while (b) {
            if (b->bedId == rec->bedId) {
                sprintf(bedInfo, "%d号(%s)", b->bedId, b->bedStatus);
                break;
            }
            b = b->next;
        }
        if (strlen(bedInfo) == 0) sprintf(bedInfo, "%d号(未知)", rec->bedId);
    }
    else {
        strcpy(bedInfo, "未住院");
    }

    if (rec->examId > 0) {
        sprintf(examInfo, "已开检查 (ID:%d)", rec->examId);
        // 如果有更详细的检查列表遍历，可以在这里扩展
    }
    else {
        strcpy(examInfo, "无");
    }
    //5.打印
    printf("\n");
    printf("============================================================\n");
    printf("                  就诊记录详情 (ID: %d)\n", rec->recId);
    printf("============================================================\n");

    // --- 第一部分：患者与医生 ---
     printf("[基本信息]\n");
    printf("  就诊科室: %-12s  接诊医生: %-8s (ID:%d)\n", deptName, docName, rec->docId);
    printf("  患者姓名: %-10s  性别: %-6s  年龄: %d岁\n", patName, gender, age);
    printf("  患者ID:   %-10d  挂号时间: %s\n", rec->patId, rec->regTime);
    printf("  当前状态: 【%s】\n", rec->recStatus);
    printf("----------------------------------------------------------------------\n");

    // --- 第二部分：诊疗内容 ---
    printf("[诊断信息]\n");
    Exam* e = examList;
    int cnt = 0;
    while (e) {
        if (e->patId == rec->patId) {
            cnt++;
            printf("ID:%d  类型:%s  时间:%s  结果:%s  费用:%d元\n",
                e->examId, examTypeName(e->type),
                e->examTime, e->result, e->fee);
        }
        e = e->next;
    }
    if (cnt == 0) printf("无检查记录\n");
    else printf("共 %d 项检查\n", cnt);
    printf("------------------------------------------------------------\n");

    // --- 第三部分：治疗与住院 ---
    printf("[治疗方案]\n");
    if (rec->needMedicine) {
        printf("  开药情况: 是\n");
        printf("  药品名称: %-20s  数量: %d 盒\n", medName, rec->medNum);
    }
    else {
        printf("  开药情况: 否\n");
    }

    if (rec->needHospital) {
        printf("  住院情况: 是\n");
        printf("  床位信息: %s\n", bedInfo);
        printf("  入院时间: %s\n", rec->startTime);
    }
    else {
        printf("  住院情况: 否\n");
    }

    printf("============================================================\n");
    printf("\n");


}

void recordResearchByStatus(){
    int opt;
    char status[30];
    while (1) {

        printf("请输入查询状态: ");
        printf("1.挂号 2.住院 3.诊疗结束\n");
        
        scanf("%d",& opt);
        if (opt==1) {
            strcpy(status, "挂号");
        }
        else if (opt==2) {
            strcpy(status, "住院");
        }
        else if (opt==3) {
            strcpy(status, "诊疗结束");
        }
        else {
            printf("输入错误！请重新输入！\n");
            continue;
        }
    }
    Record* rec = recordList;
    while (rec) {
        if (strcmp(rec->recStatus, status) == 0) {
            printARecord(rec);
        }
        rec = rec->next;
    }
   
   

}
// 辅助函数：获取用户输入的时间字符串 (格式: YYYY-MM-DD HH:MM)
void getInputTime(char* timeBuf, int bufSize) {
   
    // 使用 %[^\n] 读取整行，防止空格截断
    scanf(" %[^\n]", timeBuf);
    // 简单校验长度，理想情况下应解析验证
    if (strlen(timeBuf) < 16) {
        printf("警告：时间格式可能不正确，请确保包含日期和时间。\n");
    }
}
void recordResearchByTime(){
    char startTime[30] = { 0 };
    char endTime[30] = { 0 };

    printf("\n====== 按时间范围查询 ======\n");
    printf("请输入开始时间 (格式: YYYY-MM-DD HH:MM): ");
    getInputTime(startTime, sizeof(startTime));
    printf("请输入结束时间 (格式: YYYY-MM-DD HH:MM): ");
    getInputTime(endTime, sizeof(endTime));

    // 校验时间逻辑：开始时间不能晚于结束时间
    if (strcmp(startTime, endTime) > 0) {
        printf("错误：开始时间不能晚于结束时间！\n");
        return;
    }
    Record* p = recordList;
    int count = 0;
    while (p != NULL) { 
        if (strcmp(p->regTime, startTime) >= 0 && strcmp(p->regTime, endTime) <= 0) {

            printARecord(p);

            count++;
        }
        p = p->next;
    }
    if (count == 0) {
        printf("没有找到符合条件的记录！\n");
    }
    printf("共找到%d条记录！\n", count);
    return;
    }



void adminSearchByRecId() {
    printf("请输入待查询就诊记录编号：\n");
    int recId;
    scanf("%d",& recId);
    Record* p = findRecordByrecId(recId);
    printARecord(p);
    return;
    
}
void adminSearchByPatient() { 
    printf("请输入待查询患者编号：\n");
    int patId;
    scanf("%d",& patId);
    Record* p = recordList;
    int count = 0;
    while (p != NULL) {
        if (p->patId == patId) {
            printARecord(p);
            return;
            count++;
        }
        p=p->next;
    }
    if (count == 0)
    printf("未找到该患者！\n");
    printf("已找到%d条结果！\n", count);
    return;
}
void adminSearchByDoctor() {
    printf("请输入待查询医生编号：\n");
    int docId;
    scanf("%d", &docId);
    Record* p = recordList;
    int count = 0;
    while (p != NULL) {
        if (p->docId == docId) {
            printARecord(p);
            return;
            count++;
        }
        p=p->next;
    }
    if (count == 0)
    printf("未找到该医生！\n");
    printf("已找到%d条结果！\n", count);
    return;
}
void recordResearch() {
   
    int op;
    printf("\n====== 管理员查询系统 ======\n");
    while (1) {
        printf("1. 按就诊记录编号单条查询\n");
        printf("2. 按患者ID批量查询\n");
        printf("3. 按医生ID批量查询\n");
        printf("4. 按状态批量查询\n");
        printf("5. 按时间批量查询\n");
        printf("6. 放弃本次查询\n");
        printf("请输入删除方式序号：\n");
        if (scanf("%d", &op) == 1 && op <= 6 && op >= 1) {
            break;
        }
        while (getchar() != '\n');//清除输入缓冲区防止死循环
        printf("输入序号错误！请重试。\n");
    }
    switch (op) {
    case 1:adminSearchByRecId(); system("pause"); break;
    case 2:adminSearchByPatient(); system("pause"); break;
    case 3:adminSearchByDoctor(); system("pause"); break;
    case 4:recordResearchByStatus(); system("pause"); break;
    case 5:recordResearchByTime(); system("pause"); break;
    default:printf("本次查询已放弃!\n"); break;
    }
   
}
 //6.医院方面的报表:分为财务和就诊情况两方面，就诊记录统计报表又有详细和统计两种
// 6.1医院财务统计报表
void hospitalFeeReport()
{
    Record* p = recordList;
    int totalRecord = 0;
    float totalReg = 0, totalExam = 0, totalMed = 0, totalIncome = 0;
    if (recordList == NULL) {
        printf("暂无就诊记录，无法统计报表！\n");
        return;
    }
    while (p != NULL) {
        if (p == NULL) break;
        totalRecord++;
        totalReg += p->feeReg;
        totalExam += p->feeExam;
        totalMed += p->feeMedicine;
        totalIncome += p->feeTotal;
        Record* next = p->next;
        p = p->next;
    }
    float totalCost = getTotalPurchaseValue();
    printf("\n============= 医院财务报表 =============\n");
    printf("  总就诊记录：%d 条\n", totalRecord);
    printf("  挂号总收入：%.2f 元\n", totalReg);
    printf("  检查总收入：%.2f 元\n", totalExam);
    printf("  药品总收入：%.2f 元\n", totalMed);
    printf("  全院总收入：%.2f 元\n", totalIncome);
    printf("  总采购金额：%.2f 元\n", totalCost);
    printf("  总利润：%.2f 元\n", totalIncome - totalCost);
    printf("=========================================\n");
}
// 6.2打印单条就诊记录（明细报表用）
void printRecordItem(Record* r) {
    char patName[20] = { 0 };
    char docName[20] = { 0 };

    getPatName(r->patId, patName);   // 你已有的工具函数
    getDocName(r->docId, docName);

    printf("| %-6d | %-10s | %-10s | %-12s | %-20s |\n",
        r->recId,
        patName,
        docName,
        r->recStatus,
        r->regTime);
}

// 打印所有就诊记录（明细报表）
void recordReportDetail(Record* head) {
    if (head == NULL) {
        printf("暂无就诊记录。\n");
        return;
    }

    printf("==================== 就诊记录明细报表 ====================\n");
    printf("| %-6s | %-10s | %-10s | %-12s | %-20s |\n",
        "就诊ID", "患者", "医生", "状态", "挂号时间");
    printf("-----------------------------------------------------------\n");

    Record* p = head;
    while (p != NULL) {
        printRecordItem(p);
        p = p->next;
    }
    printf("===========================================================\n");
}
// 6.3医院统计报表：显示全院就诊概况
void recordReportStatistics(Record* head) {
    int total = 0;        // 总记录数
    int reg = 0;          // 仅挂号
    int finished = 0;     // 诊疗结束
    int hospital = 0;     // 住院

    Record* p = head;
    while (p != NULL) {
        total++;
        if (strcmp(p->recStatus, "挂号") == 0) reg++;
        else if (strcmp(p->recStatus, "诊疗结束") == 0) finished++;
        else if (strcmp(p->recStatus, "住院") == 0) hospital++;

        p = p->next;
    }

    printf("================== 医院就诊统计报表 ==================\n");
    printf("  总就诊记录数：%d\n", total);
    printf("  待看诊（挂号）：%d\n", reg);
    printf("  已完成诊疗：%d\n", finished);
    printf("  住院中：%d\n", hospital);
    printf("=======================================================\n");
}
void recordReportMenu() {
    Record* p = recordList;
    int opt;
    printf("\n====== 就诊记录报表 ======\n");
    printf("1. 查看全部就诊明细\n");
    printf("2. 查看医院统计概况\n");
    printf("3. 查看医院财务报表\n");
    printf("0. 返回上一级菜单\n");
    printf("请选择：");
    scanf("%d", &opt);

    if (opt == 1) {
        recordReportDetail(p);
        system("pause");
    }
    else if (opt == 2) {
        recordReportStatistics(p);
        system("pause");
    }
    else if (opt == 3) {
        hospitalFeeReport();
        system("pause");
    }
    else {
        printf("输入错误！\n");
    }
}
//7.患者缴费系统
void patientPayprint(Record*p) {
    p->payed = 0;
    char patname[20];
   getPatName(p->patId, patname);
   char docname[20];
   getDocName(p->docId, docname);
   printf("就诊ID：%d\n患者：%s\n接诊医生：%s\n状态：%s\n记录生成时间：%s\n",
       p->recId, patname, docname, p->recStatus, p->regTime); 
   printf("挂号费：%f\t检查费：%f\t药品费：%f\t总费用：%f\n",p->feeReg,p->feeExam,p->feeMedicine,p->feeTotal   );
   p->payed = 1;
}
void recordPay(int loginPatId, char* loginPatName)
{
    printf("\n======缴费系统======\n");
    Record* p = recordList;
    int Total = 0;
    while (p != NULL&&p->payed==0) {
        if (p->patId ==loginPatId) {
            printf("患者缴费信息如下：\n");
            patientPayprint(p);
            Total += p->feeTotal;
        }
    }
    printf("总费用为：%d\n", Total);
    printf("请扫描二维码线上支付或到人工柜台进行缴费。\n");

}
//8.管理员修改功能
void adminModifyMenu() {
    while (1) {
        printf("\n====== 管理员修改功能 ======\n");
        printf("1. 修改患者信息\n");
        printf("2. 修改医生信息\n");
        printf("3. 修改就诊记录\n");
        printf("0. 返回管理员主菜单\n");
        printf("请选择：");

        int opt;
        if (scanf("%d", &opt) != 1) {
            while (getchar() != '\n');
            printf("输入错误！\n");
            continue;
        }

        if (opt == 1) adminModifyPatient();
        else if (opt == 2) adminModifyDoctor();
        else if (opt == 3) adminModifyRecord();
        else if (opt == 0) break;
        else printf("输入错误！\n");
    }
}
void adminModifyPatient() {
   /* int patId;
    printf("请输入患者ID：");
    if (scanf("%d", &patId) != 1) {
        while (getchar() != '\n');
        printf("输入错误！\n");
        return;
    }
    Patient* p = findPatientById(patId);
    if (p == NULL) {
        printf("未找到该患者！\n");
        return;
    }
    printf("\n======信息修改列表======\n");
    printf("1. 修改患者姓名\n");
    printf("2. 修改患者性别\n");
    printf("3. 修改患者生日\n");
   */
    //等待患者负责人写
    printf("待完善\n");
    return;
}
void adminModifyDoctor() {
    //确认医生负责人写了没
    printf("待完善\n");
    return;
}
/**
 * 修改指定的就诊记录
 */
 // 辅助函数：清除缓冲区并读取一行字符串
void safeGetString(char* buffer, int size) {
    while (getchar() != '\n'); // 清除缓冲区
    if (fgets(buffer, size, stdin) != NULL) {
        // 去除 fgets 可能读入的换行符
        buffer[strcspn(buffer, "\n")] = 0;
    }
}
void adminModifyRecord() {
    printf("\n====== 修改就诊记录 ======\n");
    printf("请输入要修改的就诊记录ID: ");
    int recId;
    if (scanf("%d", &recId) != 1) return;

    // 1. 查找记录
    Record* target = findRecordByrecId(recId); 
    if (target == NULL) {
        printf("未找到 ID 为 %d 的记录。\n", recId);
        return;
    }

    // 2. 显示当前简要信息
    char patName[20], docName[20];
    getPatName(target->patId, patName);
    getDocName(target->docId, docName);
    printf("当前记录: ID[%d] 患者[%s] 医生[%s] 状态[%s] 时间：%s\n",
        target->recId, patName, docName, target->recStatus,target->regTime);

    // 3. 修改菜单循环
    int choice;
    while (1) {
        printf("\n--- 请选择要修改的内容 ---\n");
        printf("1. 诊断内容 \n");
        printf("2. 检查项目 \n");
        printf("3. 开药情况 \n");
        printf("4. 住院情况 \n");
        printf("5. 就诊状态 (Status: 挂号/看诊/住院/结束)\n");
        printf("0. 保存并退出\n");
        printf("请输入选项: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); // 错误处理
            continue;
        }

        if (choice == 0) break;

        switch (choice) {
        case 1: { // 修改诊断
            printf("当前诊断: %s\n", target->diagContent);
            printf("请输入新诊断内容: ");
            safeGetString(target->diagContent, sizeof(target->diagContent));
            printf("诊断已更新。\n");
            break;
        }
        case 2: { // 修改检查
            printf("当前检查ID: %d (0表示无)\n", target->examId);
            printf("请输入新的检查ID (整数): ");
            scanf("%d", &target->examId);
            // 这里可以添加逻辑：如果ID改变，去 examList 里验证是否存在
            printf("检查ID已更新。\n");
            break;
        }
        case 3: { // 修改开药
            printf("当前是否开药: %s\n", target->needMedicine ? "是" : "否");
            printf("是否开药? (1=是, 0=否): ");
            scanf("%d", &target->needMedicine);

            if (target->needMedicine) {
                printf("请输入药品ID: ");
                scanf("%d", &target->medId);
                printf("请输入数量: ");
                scanf("%d", &target->medNum);
                // 简单校验
                if (target->medNum < 0) target->medNum = 0;
                if (target->medNum > 10) target->medNum = 10;
            }
            else {
                target->medId = 0;
                target->medNum = 0;
            }
            printf("开药信息已更新。\n");
            break;
        }
        case 4: { // 修改住院
            printf("当前是否住院: %s\n", target->needHospital ? "是" : "否");
            printf("是否住院? (1=是, 0=否): ");
            scanf("%d", &target->needHospital);

            if (target->needHospital) {
                printf("请输入床位ID: ");
                scanf("%d", &target->bedId);
                printf("请输入入院时间 (YYYY-MM-DD HH:MM): ");
                safeGetString(target->startTime, sizeof(target->startTime));

                // 可选：联动更新 Bed 结构体的 patId 和 status
                // updateBedStatus(target->bedId, target->patId, "占用"); 
            }
            else {
                target->bedId = 0;
                strcpy(target->startTime, "");
                // 可选：释放床位
                // updateBedStatus(oldBedId, 0, "空闲");
            }
            printf("住院信息已更新。\n");
            break;
        }
        case 5: { // 修改状态
            printf("当前状态: %s\n", target->recStatus);
            printf("请输入新状态 (挂号/看诊/住院/诊疗结束): ");
            safeGetString(target->recStatus, sizeof(target->recStatus));
            printf("状态已更新。\n");
            break;
        }
        default:
            printf("无效选项，请重试。\n");
        }
    }

    printf("\n 记录 ID:%d 修改完成并已保存至内存。\n", target->recId);
}