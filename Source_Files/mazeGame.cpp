#include "mazemain.h"

int main()
{
	// 갈 수 있는 방향 중 랜덤으로 방향을 결정
	srand(time(NULL));

	createMap();

	//기록 리턴
	int isFinish = run();

	return isFinish;
}

// 맵을 초기화, recursive backtracking 알고리즘 사용하여 맵 생성
void createMap()
{
	initMap();
	createPath();

	//문자 변경
	changeChar();

	//도착지 지정
	getDestination();

	drawMap();
}

// 실행코드
int run()
{
	system("mode con: cols=140 lines=65");

	wchar_t *screen = new wchar_t[screenWidth * screenHeight];
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	/* 	1. 코드 설명
	- 프로그램의 속도, 즉 단위 시간 당 cpu 할당량 상관없이 사용자가 2개의 입력 사이의 시간차이와 속도를 가지고 현재 플레이어가 움직인 거리를 계산한다.*/

	while (1)
	{
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		// main()로 점수 반환
		int isFinish = inputUser(fElapsedTime);
		if (isFinish > 0)
		{
			return isFinish;
		}

		for (int x = 0; x < screenWidth; x++)
		{
			// rayangle은 fov값을 이용한 바라보는 시야각도들
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)screenWidth) * fFOV;

			float fStepSize = 0.1f;
			float fDistanceToWall = 0.0f; // distanceToWall은 빛이 플레이어 위치에서부터 벽에 부딪힌 지점까지의 거리

			bool bHitWall = false;
			bool bBoundary = false;

			// fEye는 방향을 찾아내는(갖는) 벡터값)
			float fEyeX = sinf(fRayAngle);
			float fEyeY = cosf(fRayAngle);

			/* 1. 코드 설명
			- fDistanceToWall을 통해 중간에 벽을 만났을 때 저장된 distance 값을 통해 벽을 나타냄

			   2. fDepth 설명
			- 광선이 닿는 최대 거리로, 어느 지점 이상일 때 사용자한테 보이지 않기 위해 사용됨.*/
			while (!bHitWall && fDistanceToWall < fDepth)
			{
				/* 2. DistanceToWall
				- 빛이 플레이어 위치에서부터 벽에 부딪힌 지점까지의 거리를 표현 */
				fDistanceToWall += fStepSize;

				int nTestX = (int)(fPlayerY + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerX + fEyeY * fDistanceToWall);

				if (nTestX < 0 || nTestX >= mapWidth || nTestY < 0 || nTestY >= mapHeight)
				{
					bHitWall = true;
					fDistanceToWall = fDepth;
				}
				else
				{

					/*  1. 코드 설명
					- 벽 사이가 가까우면(중첩) 중첩한 부분을 표시 안하고 boundary 처리. 그래서 벽이 여러 개 존재하는 것처럼 보임. */
					if (map.c_str()[nTestX * mapWidth + nTestY] == WALL)
					{
						bHitWall = true;
						vector<pair<float, float>> p;

						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								
								 /* 3. 경계선 처리
								 - ty, tx를 통해 겹치는 부분을 바운더리 처리를 하여 안나타게 함. 
								 - 그리고 경계선이 있는 곳(바운더리)은 유니코드 문자 0x2502를 넣어서 처리 */
								float vy = (float)nTestY + ty - fPlayerX;
								float vx = (float)nTestX + tx - fPlayerY;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(make_pair(d, dot));
							}

						// Sort Pairs from closest to farthest
						sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right)
							 { return left.first < right.first; });

						
						/* 2. boundary
						- 벽과 벽 사이에 있는 경계선을 나타냄*/  // tx랑 ty가 1,1 일때는 못봄
						float fBound = 0.0021;
						if (acos(p.at(0).second) < fBound)
							bBoundary = true;
						if (acos(p.at(1).second) < fBound)
							bBoundary = true;
						if (acos(p.at(2).second) < fBound)
							bBoundary = true;
					}
				}
			}

			int nCeiling = (float)(screenHeight / 2.0) - screenHeight / ((float)fDistanceToWall);
			int nFloor = screenHeight - nCeiling;

			short nShade = ' ';
			if (fDistanceToWall <= fDepth / 20.0f)
				nShade = 0x2588; // Very close
			else if (fDistanceToWall < fDepth / 19.0f)
				nShade = 0x2588;
			else if (fDistanceToWall < fDepth / 10.0f)
				nShade = 0x2593;
			else if (fDistanceToWall < fDepth / 9.0f)
				nShade = 0x2592;
			else if (fDistanceToWall < fDepth / 4.0f)
				nShade = 0x2591;
			else
				nShade = ' '; // Too far away

			if (bBoundary)
				nShade = 0x2502; // Black it out

			for (int y = 0; y < screenHeight; y++)
			{
				// Each Row
				// Ceilling
				if (y < nCeiling)
					screen[y * screenWidth + x] = ' ';

				// Wall
				else if (y == nCeiling || y == nFloor)
					screen[y * screenWidth + x] = 0x2550;

				else if (y > nCeiling && y <= nFloor && bBoundary)
					screen[y * screenWidth + x] = nShade;

				else if (y > nCeiling && y < nFloor)
					screen[y * screenWidth + x] = ' ';

				else // Floor
				{
					// Shade floor based on distance
					float b = 1.0f - (((float)y - screenHeight / 2.0f) / ((float)screenHeight / 2.0f));
					if (b < 0.15)
						nShade = '#';
					else if (b < 0.25)
						nShade = 'x';
					else if (b < 0.55)
						nShade = '-';
					else if (b < 0.9)
						nShade = 0x2219;
					else
						nShade = nShade;
					screen[y * screenWidth + x] = nShade;
				}
			}
		}

		// Display Stats
		swprintf_s(screen, 50, L"%d층 - X=%3.2f - Y=%3.2f - A=%3.2f - FPS=%3.2f", mapFloor, fPlayerX, fPlayerY, fPlayerA, 1.0f / fElapsedTime);

		// Display miniMap
		if (showMinimap)
		{
			// Display Map
			for (int y = 0; y < mapHeight; y++)
				for (int x = 0; x < mapWidth; x++)
				{
					screen[x + (y + 3) * screenWidth] = map[x + y * mapWidth];
				}
			screen[((int)fPlayerY + 3) * screenWidth + (int)fPlayerX] = 'P';
		}

		// Display Frame
		screen[screenWidth * screenHeight - 1] = '\0';
		WriteConsoleOutputCharacterW(hConsole, screen, screenWidth * screenHeight, {0, 0}, &dwBytesWritten);
	}
	return 0;
}

// 1차원 배열을 2차원 배열처럼 사용할 때 쓰는 인덱스 값
int mapXY(int x, int y)
{
	return x + y * mapWidth;
}

//맵 0으로 초기화, 테두리 1로 초기화
void initMap()
{

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			if (y == 0 || y == mapHeight - 1) // 위아래 테두리를 1(#)로
				map[mapXY(x, y)] = BOUNDARY;
			else if (x == 0 || x == mapWidth - 1) // 좌우 테두리를 1(#)로
				map[mapXY(x, y)] = BOUNDARY;
			else
				map[mapXY(x, y)] = WALL; // 나머지 2(@)로(벽)
		}
	}
}

// recursive backtracking 알고리즘 사용하여 맵 생성
void createPath()
{	
	// 플레이어 초기 위치 값 설정
	setPlayerXY(1.00f, 1.00f);

	while (!xyStack.empty())
	{
		// 3. 상하좌우 갈 수 있는 벽 확인
		// 4. 갈 수 있는 벽 근처에 통로가 1개인지 확인
		// 5. 갈 수 있는 벽이 있으면 랜덤으로 결정

		int dir = getDirection(); //플레이어 위치에서 갈 수 있는 방향을 랜덤으로 리턴

		//리턴 받은 방향이 none인지 알아내서 스택에서 pop
		if (dir == NONE)
		{
			xyStack.pop();
			continue;
		}

		//뚫은 곳의 x,y좌표를 스택에 저장
		//리턴 받은 방향(UP) -> (x,y 값 + 현재 위치 x,y값)을 스택에 push
		int nextX = getDirX(fPlayerX, dir);
		int nextY = getDirY(fPlayerY, dir);
		_SPlayerLoc loc{(float)nextX, (float)nextY};
		xyStack.push(loc);

		//도착지 설정
		setDestination(nextX, nextY, xyStack.size());

		//플레이어가 지나간 곳은 빈칸으로 저장
		map[mapXY(nextX, nextY)] = EMPTY;
	}
}

// 도착지 설정
// 스택의 size로, 플레이어로부터 멀리 떨어진 x,y값을 도착지로 설정
void setDestination(int x, int y, int size)
{
	if (size > _sDestination.size)
	{
		_sDestination.size = size;
		_sDestination.x = x;
		_sDestination.y = y;
	}
}

// 도착지 얻어오기
void getDestination()
{
	map[mapXY(_sDestination.x, _sDestination.y)] = FINISH;
}

// test로 맵 출력
void drawMap()
{
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			printf("%c", map[mapXY(x, y)]);
		}
		printf("\n");
	}
}

// 현 플레이어 위치에서 상하좌우 방향을 알아냄
// 이후 알아낸 방향에 벽이 있는지 확인 && 갈 수 있는 벽 근처에 통로가 1개인지 확인
int getDirection()
{
	int availableWalls = 0;
	int bWall[4];

	fPlayerX = xyStack.top().x;
	fPlayerY = xyStack.top().y;

	for (int i = 0; i < 4; i++)
	{
		//  ###
		// #@@@#
		// #@@@#
		// #@@P#
		//  ###
		int nx = getDirX((int)fPlayerX, i);
		int ny = getDirY((int)fPlayerY, i);

		// 뒤에 부분 설명(통로가 1개인지 확인하는 부분)
		// getDir()로 x,y 좌표를 구한다.
		// mapXY()로 해당 위치의 map 배열 인덱스 값을 구한다. [x][y] -> [x+y*width]
		// 구한 인덱스의 배열 값이 0인지(통로) 확인
		if (map[mapXY(nx, ny)] == WALL)
		{
			int count = 0;

			// 만나는 통로 개수가 1개인지 확인한다.
			for (int j = 0; j < 4; j++)
			{
				boolean isEmpty = map[mapXY(getDirX(nx, j), getDirY(ny, j))] == EMPTY;

				if (isEmpty)
				{
					count++;
				}
			}
			// 갈 수 있는 벽 근처의 통로가 없거나 1개 이상이면 리턴할 수 있도록
			if (count != 1)
			{
				continue;
			}

			//배열이나 리스트에 [i] 저장
			bWall[availableWalls++] = i;
		}

		// bwall[0] = 상, bwall[1] = 우
	}

	// 0, 1, 2, 3 = up, down, left, right
	// availableWalls가 0이면 갈 수 있는 곳이 없음
	if (availableWalls == 0)
	{
		return NONE;
	}
	else
	{ // 랜덤으로 갈 수 있는 방향을 결정
		int r = rand() % availableWalls;
		return bWall[r]; //리턴 방향(0,1,2,3)
	}
}

//상하좌우 x축 좌표 리턴
int getDirX(int x, int i)
{
	int x_dirArr[4] = {0, 0, -1, 1}; // 상화좌우 x축 방향
	if (i < 0 && 3 < i)
	{
		printf("getDirX: ERROR(%d)\n", i);
	}
	return x + x_dirArr[i];
}

//상하좌우 y축 좌표 리턴
int getDirY(int y, int i)
{
	int y_dirArr[4] = {-1, 1, 0, 0}; // 상화좌우 y축 방향
	if (i < 0 && 3 < i)
	{
		printf("getDirY: ERROR(%d)\n", i);
	}
	return y + y_dirArr[i];
}

// 맵 생성 시 초기값 플레이어 위치 설정
void setPlayerXY(float x, float y)
{
	fPlayerX = x;
	fPlayerY = y;
	map[mapXY((int)x, (int)y)] = EMPTY; // 사용자가 지나간 길을 0으로 표시
	_SPlayerLoc loc{x, y};
	xyStack.push(loc); // 플레이어 위치를 구조체에 저장 후 구조체를 스택에 push
}

//구조체의 x,y 값 저장
_SPlayerLoc setStructXY(float x, float y)
{
	_sPlayerLoc.x = x;
	_sPlayerLoc.y = y;

	return _sPlayerLoc;
}

// 테두리(@)를 #으로 변경
void changeChar()
{

	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			if (map[mapXY(x, y)] == '@')
			{
				map[mapXY(x, y)] = '#';
			}
		}
		printf("\n");
	}
}

int inputUser(float fElapsedTime)
{

	// Handle CCW Rotation
	if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
		fPlayerA -= (fSpeed * 0.75f) * fElapsedTime;

	// Handle CW Rotation
	if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
		fPlayerA += (fSpeed * 0.75f) * fElapsedTime;

	// Handle Forwards movement & collision
	if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
	{
		/* 2. Sin, cosf 설명
		- sin, cosf을 통해 어느 각도든 좌표 이동을 간단하게 계산 가능
		- 앞에서 구한 시간 * 속도 * 각도를 통해 거리를 구한다 */

		fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime;
		fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime;

		if (map.c_str()[(int)fPlayerY * mapWidth + (int)fPlayerX] == WALL)
		{
			fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime;
			fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime;
		}
		else if (map.c_str()[(int)fPlayerY * mapWidth + (int)fPlayerX] == FINISH)
		{

			return 1;
		}
	}

	// Handle backwards movement & collision
	if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
	{
		fPlayerY -= sinf(fPlayerA) * fSpeed * fElapsedTime;
		fPlayerX -= cosf(fPlayerA) * fSpeed * fElapsedTime;

		if (map.c_str()[(int)fPlayerY * mapWidth + (int)fPlayerX] == WALL)
		{
			fPlayerY += sinf(fPlayerA) * fSpeed * fElapsedTime;
			fPlayerX += cosf(fPlayerA) * fSpeed * fElapsedTime;
		}
		else if (map.c_str()[(int)fPlayerY * mapWidth + (int)fPlayerX] == FINISH)
		{

			return 1;
		}
	}

	if (GetAsyncKeyState((unsigned short)'M') & 0x8000)
	{
		showMinimap = true;
	}
	else
	{
		showMinimap = false;
	}

	if (GetAsyncKeyState((unsigned short)'H') & 0x8000)
	{

		return 1;
		// printf("다음 층으로 이동하시겠습니까?, 현재 기록: %f\n\n", dRecord);
	}
	return 0;
}
