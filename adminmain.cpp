#define _CRT_SECURE_NO_WARNINGS
#include "common.h"
#include "record.h" 
//管理员系统主函数menu
void adminRecordMAnagement() {
    while (1) {
        printf("\n======就诊记录管理菜单======\n");
        printf("1.删除就诊记录\n");
        printf("2.查询就诊记录\n");
        printf("3.修改就诊记录信息\n");
        printf("4.查看医院报表\n");
        printf("0.返回\n");
        int opt;
        printf("请输入操作选项：");
        if (scanf("%d", &opt) != 1) {
            while (getchar() != '\n');
            printf("输入错误！\n");
            continue;
        }
        switch (opt) {
        case 1:adminDelete();  break;
        case 2:recordResearch();break;
        case 3: adminModifyMenu();  break;
        case 4:recordReportMenu(); break;
        case 0:return;
        default:printf("输入错误！\n");
        }
    }
    return;
}
void  adminReport() { 
    while (1) {

        printf("\n======报表列表======\n");
    }
}
void adminmain(){
    
    printf("欢迎管理员登录\n");//登录密码123456
    if (!adminLogin()) {
        printf("密码错误\n");
        return;
    }
    while (1) {
    printf("\n======管理员系统菜单======\n");
    printf("1.就诊信息管理\n");
    printf("2.医药系统管理\n");
    printf("3.检查系统管理\n");//删改查（检查记录）
    printf("4.病床系统管理\n");   
    printf("5.人员系统管理\n");//医生和患者的信息增删改查，分别做成两个菜单
    printf("6.科室系统管理\n");//主要功能为科室信息查询（下属患者，医生，药品，病床等）
    printf("8.系统报表\n"); 
    printf("0.退出\n");
    printf("请输入操作选项：");
    int opt;
    if (scanf("%d", &opt) != 1) {
        while (getchar() != '\n');
        printf("输入错误！\n");
        continue;
    }
    switch (opt) { 
        case 1:adminRecordMAnagement(); break;
            case 2://adminMedicineManagement(); break;
                case 3://adminExamManagement(); break;
                    case 4://adminBedManagement(); break;
                        case 5://adminPersonManagement(); break;
                            case 6://adminDeptManagement(); break;
                                case 8:adminReport(); break;
                                    case 0:return;
                                        default:printf("输入错误！\n");
    }
    


    }
    return;
   
    }

