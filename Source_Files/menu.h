#pragma once
#include <windows.h>
#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <chrono>

typedef struct UserInformation {
    char name[16];
    char id[16];
    char password[16];
    int record;
    int rank;
} UI;

#define MAX_USER_DATA  100

UI g_user_data[MAX_USER_DATA];
int g_user_count = 0;
int success_login;
int current_user = 0;

using namespace std;
using namespace chrono;


// 키보드 값
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define SUBMIT 4  // 선택 (스페이스바)

//커서 이동
void cursormove(int x, int y);
int keyControl();

//로그인 메뉴 함수
void LoadUserData();
void SaveUserData();
void GetPassword(char parm_password[]);
void UserRegister();
void loginProcess();
void loginTitle();
void loginDraw(int x, int y);
int loginMenu(int x, int y);


//메인 메뉴 함수
int mainMenu(int x, int y);
void mainTitle();
void getInfo();
void getEndCredit();
void getRank();
void menuDraw(int x, int y);
int menuControl(int x, int y);
void endDraw();


//기록 측정
double measureRecord();
steady_clock::time_point getFinishTime();
steady_clock::time_point startSec, endSec;



