#include "menu.h"

int main()
{
    system("mode con cols=140 lines=50");
    SetConsoleOutputCP(CP_UTF8);

    char id[16], password[16] = {0};
    success_login = 0;
    int x = 56;
    int y = 16;

    loginTitle();

    //로그인 메뉴 호출
    loginMenu(x, y);

    printf("  로그인 되었습니다!\n");
    system("pause");
    system("cls");

    //메인 메뉴 호출
    mainMenu(x, y);
}

// 실행 시 가장 먼저 보여지는 화면
// 로그인, 회원가입 기능 포함된 함수
int loginMenu(int x, int y)
{
    while (success_login == 0)
    { // 로그인
        LoadUserData();
        loginDraw(x, y);
        int loginmenuCode = menuControl(x, y); // 로그인 메뉴를 불러온다

        if (loginmenuCode == 0)
        {
            loginProcess(); // 누르면 로그인 과정으로 넘어간다
        }
        else if (loginmenuCode == 2)
        { // 회원가입
            system("cls");
            UserRegister();
        }
        else if (loginmenuCode == 4)
        { // github 링크
            printf("https://github.com/Daniel-923?tab=repositories");
            system("pause");
        }
        else if (loginmenuCode == 6)
        { // 종료
            return 0;
        }
        system("cls");
    }
    return 0;
}

// 로그인이 성공했을 때 보여지는 화면
// gotoXY를 통해 게임시작, 게임 순위, 게임 정보 기능 및 선택 구현
int mainMenu(int x, int y)
{
    while (1)
    {
        mainTitle();
        menuDraw(x, y);
        int menuCode = menuControl(x, y);

        if (menuCode == 0)
        {
            while (1)
            {
                //시간 측정 시작
                startSec = steady_clock::now();
                // 게임 시작
                int isFinish = system("H:\\Study\\C_Language\\Maze_Project_0.1\\Source_Files\\mazeGame.exe");

                if (!isFinish)
                    break;

                //시간 측정 끝
                endSec = getFinishTime();

                int record = (int)measureRecord();
                // printf("목적지에 도착하셨습니다.\n\n");
                endDraw();
                printf("현재 기록: %d초\n\n\n\n", record);
                printf("게임메뉴로 돌아갑니다...\n\n");
                Sleep(3000);

                //개인 기록 갱신
                if (record > g_user_data[current_user].record)
                    g_user_data[current_user].record = record;

                system("mode con cols=140 lines=50");

                break;
            }
        }
        else if (menuCode == 2)
        { //게임 정보
            getRank();
        }
        else if (menuCode == 4)
        { //게임 순위
            getInfo();
        }
        else if (menuCode == 6)
        { //종료
            getEndCredit();
            return 0;
        }
        system("cls");
    }
}

// - 파일 포인터로 파일을 열어 유저의 정보를 읽음
void LoadUserData()
{
    int index = 0;
    FILE *p_file = fopen("user.dat", "rb"); // p_file의 주소값을 참조해서 user.dat의 파일을 열고 읽음
    if (p_file != NULL)
    {                                                 // p_file의 주소값이 있다면
        fread(&g_user_count, sizeof(int), 1, p_file); // int크기만큼의 메모리만큼 유저의 데이터를 1개씩 읽는다
        for (index = 0; index < g_user_count; index++)
        {
            fread(g_user_data + index, sizeof(UI), 1, p_file); // count된 유저정보까지 UI를 1개씩 읽는다
        }
        fclose(p_file);
    }
}

// - 파일 포인터로 파일에 구조체 배열로 저장된 유저의 정보를 저장한다
void SaveUserData() // UI g_user_data[MAX_USER_DATA];
{
    int index = 0;
    // p_file의 주소값을 참조해서 user.dat을 열고 입력된 유저의 정보를 쓴다
    FILE *p_file = fopen("user.dat", "wb");
    if (p_file != NULL)
    {
        fwrite(&g_user_count, sizeof(int), 1, p_file);
        for (index = 0; index < g_user_count; index++)
        {
            fwrite(g_user_data + index, sizeof(UI), 1, p_file);
        }
        fclose(p_file);
    }
}

// - 입력된 유저의 비밀번호를 하나씩 입력할 때마다 *로 바꾼다
void GetPassword(char parm_password[])
{
    int input = 0, index = 0;
    while (input != 13)
    {                     // 비밀번호가 13자리를 넘지 않을 경우
        input = _getch(); // 사용자에게 하나의 키를 입력받는다
        if (input == 13)
            parm_password[index] = 0;
        // 입력한 비밀번호가 딱 13자리일 경우 13번째 문자열부터는 0으로 처리한다
        else
        {
            printf("*");
            parm_password[index++] = input; // 문자를 저장한 뒤에 인덱스를 증가한다
        }
    }
    printf("\n");
}

void UserRegister() // 회원가입
{
    printf("저희 시스템에 회원 등록하시려면 다음 정보를 입력하셔야 합니다.\n\n");

    printf("1. Input ID: ");
    gets(g_user_data[g_user_count].id);
    printf("2. Input Password: ");
    gets(g_user_data[g_user_count].password);
    printf("3. Input Name: ");
    gets(g_user_data[g_user_count].name);

    g_user_count++; //앞의 절차가 완료되었을때 저장할 유저수를 1 증가시킨다.

    printf("회원이 되신걸 축하드립니다.. \n\n");

    SaveUserData(); //유저 정보를 저장하는 함수로 이동시킨다
}

//커서 위치 이동함수
void gotoXY(int x, int y)
{
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // HANDLE 자료형의 consoleHandle라는 변수를 선언하고 핸들을 가져온 후 변수에 대입함.
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(consoleHandle, pos);
}

void loginDraw(int x, int y)
{

    gotoXY(x - 2, y); //-2 한 이유는 >를 출력해야하기 때문에
    printf(">   로그인  ");
    gotoXY(x, y + 2);
    printf(" 회원가입");
    gotoXY(x, y + 4);
    printf("  GitHub");
    gotoXY(x, y + 6);
    printf("   종료  ");
}

int keyControl()
{
    char temp = _getch();

    if (temp == 'w' || temp == 'W')
    {
        return UP;
    }
    else if (temp == 'a' || temp == 'A')
    {
        return LEFT;
    }
    else if (temp == 's' || temp == 'S')
    {
        return DOWN;
    }
    else if (temp == 'd' || temp == 'D')
    {
        return RIGHT;
    }
    else if (temp == ' ')
    {
        return SUBMIT;
    }
    return 0;
}

void loginProcess()
{ //로그인 과정이다
    system("cls");
    printf("  로그인\n");

    char id[16], password[16] = {0};
    int index = 0, input = 0;

    while (!success_login)
    { // 로그인 실패시
        printf("Input ID: ");
        gets(id); // id를 받음
        printf("\n");
        printf("Input Password: ");
        GetPassword(password); // password를 받음

        for (index = 0; index < g_user_count; index++) // 현재 유저 수를 1씩 증가하면서 검사
        {
            if (!strcmp(g_user_data[index].id, id))
            { // db의 id(password)와 입력한 id(password)끼리 비교해서 같으면 로그인
                if (!strcmp(g_user_data[index].password, password))
                {
                    success_login = 1;
                    current_user = index;
                    printf("\n\n성공적으로 로그인 했습니다. ^^; \n\n\n\n\n\n");
                }
                else
                    printf("암호가 틀렸습니다. 다시 입력해주세요. \n\n");
                break;
            }
        }

        if (index == g_user_count) // index = 0 따라서 현재 유저 수가 0일 경우
        {
            printf("사용자님이 입력하신 ID는 등록이 되어있지 않습니다. 등록하시겠습니까? ( y / n ): ");
            input = _getche();
            system("cls");

            if (input == 'y' || input == 'Y')
            {
                UserRegister();
                break;
            }
            else
            {
                printf("다시 입력하세요...\n\n");
                continue;
            }
        }
    }
}

void loginTitle()
{ // 로그인 제목

    printf("\n\n\n\n");

    printf(" 		 		   #######  #       #  ######  ##    #  #######  ##   ## \n");
    printf("	 			      #     #   #   #  #       # #   #     #       # #   \n");
    printf("	 			      #     #  # #  #  #####   #  #  #     #        #    \n");
    printf("				      #     # #   # #  #       #   # #     #        #    \n");
    printf("				      #     ##     ##  ######  #    ##     #        #    \n");
}

//ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
//메인 메뉴

void mainTitle()
{
    printf("\n\n\n\n");
    printf("                 ##   ##     #    #####  #####      #####  #####         \n");
    printf("                 # # # #    # #      #   #              #  #   #    ###  \n");
    printf("                 #  #  #   #####    #    #####      #####  #   #   #     \n");
    printf("                 #     #   #   #   #     #          #      #   #   ####  \n");
    printf("                 #     #   #   #  #####  #####      #####  #####       # \n");
    printf("                                                                   ####  \n");
}

void menuDraw(int x, int y)
{
    gotoXY(x - 2, y);
    printf("> 게 임 시 작");
    gotoXY(x, y + 2);
    printf("게 임 순 위");
    gotoXY(x, y + 4);
    printf("게 임 정 보");
    gotoXY(x, y + 6);
    printf("   종 료  \n");
    gotoXY(x - 56, y);
    printf("  W(위로 이동)\n");
    gotoXY(x - 56, y + 2);
    printf("  S (아래로 이동)\n");
    gotoXY(x - 56, y + 4);
    printf("  SPACE BAR(들어가기,나가기)\n");
}

int menuControl(int x, int y)
{

    while (1)
    {
        int n = keyControl();
        switch (n)
        {
        case UP:
        {
            if (y > 16)
            {
                gotoXY(x - 2, y);
                printf(" ");
                gotoXY(x - 2, y -= 2);
                printf(">");
            }
            break;
        }
        case DOWN:
        {
            if (y < 21)
            {
                gotoXY(x - 2, y);
                printf(" ");
                gotoXY(x - 2, y += 2);
                printf(">");
            }
            break;
        }
        case SUBMIT:
        {
            return y - 16;
        }
        }
    }
}

// 랭크 표시
// 파일에 있는 데이터로 현재 순위와 역대 순위를 계산
void getRank()
{
    int x = 47;
    int y = 8;

    system("cls");

    //순위 계산
    int record[g_user_count] = {
        0,
    };
    int rank[g_user_count];
    int rank_count = 1;
    int high_index = 0;

    //유저들의 기록들을 배열에 저장
    for (int i = 0; i < g_user_count; i++)
    {
        record[i] = g_user_data[i].record;
    }

    for (int i = 0; i < g_user_count; i++)
    {
        if (i + 1 == 100)
            break;

        if (record[i] < record[i + 1])
        {
            rank_count++;
            high_index = i + 1;
        }
        g_user_data[i].rank = rank_count;
    }

    //자신의 개인 순위 및 기록
    char *personal_id = g_user_data[current_user].id;
    int personal_record = g_user_data[current_user].record;
    int personal_rank = g_user_data[current_user].rank;

/*     //역대 순위 및 기록
    char *high_id = g_user_data[high_index].id;
    int high_record = g_user_data[high_index].record;
    int high_rank = g_user_data[high_index].rank; */

/*     printf("\n\n");
    gotoXY(x - 38, y - 2);
    printf("*****역대순위************************\n\n");
    gotoXY(x - 38, y);
    printf("ID: %c\n\n", high_id);
    gotoXY(x - 38, y + 2);
    printf("시간: %d초\n\n", high_record);
    gotoXY(x - 38, y + 4);
    printf("순위: %d위\n\n", high_rank);
    gotoXY(x - 38, y + 6);
    printf("************************************\n\n"); */

    gotoXY(x, y - 2);
    printf("*****개인순위************************\n\n");
    gotoXY(x, y);
    printf("ID: %c\n\n", personal_id);
    gotoXY(x, y + 2);
    printf("시간: %d초\n\n", personal_record);
    gotoXY(x, y + 4);
    printf("순위: %d위\n\n", personal_rank);
    gotoXY(x, y + 6);
    printf("*************************************\n\n");

    printf("\n\n\n\n\n\n");
    system("pause");
}

// 게임 정보
void getInfo()
{
    system("cls");
    printf("\n\n");
    printf("조작키 설명\n\n");
    printf("W (위로 이동)\n\n");
    printf("S (아래로 이동)\n\n");
    printf("A (좌측 회전)\n\n");
    printf("D (우측 회전)\n\n");
    printf("M (미니맵이 보여집니다)\n\n\n");
    printf("게임설명:\n\n");
    printf("해당 게임은 3D 1인칭 미로 게임입니다.\n\n");
    printf("회원가입과 로그인이 가능합니다\n\n");
    printf("미로 들어갔을 때부터 끝날 때까지의 시간으로 순위를 결정합니다.\n\n");
    printf("친구들과 순위를 겨뤄보세요\n\n");
    printf("재밌게 즐겨주세요!!\n\n");

    system("pause");
}

//종료 되기 전에 나오는 엔딩 크레딧
void getEndCredit()
{
    int x = 47;
    int y = 8;
    {
        system("cls");

        printf("\n\n");
        gotoXY(x - 1, y - 1);
        printf(" *******************************\n\n ");
        gotoXY(x, y);
        printf(" 만든이: twenty \n\n");
        gotoXY(x, y + 2);
        printf(" 리더: 김윤영 \n");
        gotoXY(x, y + 4);
        printf(" 팀원: 윤재승, 권오선 \n");
        gotoXY(x, y + 6);
        printf(" 재밌게 즐겨주셔서 감사합니다.\n");
        gotoXY(x, y + 8);
        printf(" 지금까지 twenty였습니다.\n");
        gotoXY(x, y + 9);
        printf("********************************\n");

        Sleep(3000);
    }
}

void endDraw()
{
    printf("\n\n\n\n");
    printf("                                                      **                  **     *  *            * \n ");
    printf("                   ******    *             ********            ********  *  *           * *           *         * \n");
    printf("                   *         *               *  *                 **     *  *          *   *          *         *                 *\n");
    printf("                   ******    ***            *    *               *  *    ****         *     *         *         *       *******   *\n");
    printf("                   *         *           ************             **     *  *      *************      *         *       *         *\n");
    printf("                   ******    *                 *                         *  *         *     *         *         *       *         *****\n");
    printf("                      ********             *********                                  *     *         ********* *       *******   *\n");
    printf("                             *                     *                *    *            *******                   *                 *\n");
    printf("                      ********             *********               * *  * *           *     *                   *                 *\n");
    printf("                      *                    *                      *   *    *          *******                   *                 *\n");
    printf("                      ********             *********             *          * \n");
}

// 도착 기록 측정
steady_clock::time_point getFinishTime()
{
    /* finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("%f초", duration); */

    steady_clock::time_point end = steady_clock::now();
    return end;
}

double measureRecord()
{

    std::chrono::duration<double> record = endSec - startSec;

    double dRecord = record.count();
    // cout << "현재 기록: " << record.count() << endl;
    return dRecord;
}
