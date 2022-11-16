



# 게임 정보

- 조작키 설명
W(위로 이동) 
S(아래로 이동) 
A(좌측 회전)
D(우측 회전)


- 게임 설명
해당 게임은 3D 1인칭 미로 게임입니다. 

미로 들어갔을 때부터 끝날 때까지의 시간으로 순위를 결정합니다.



	boolean up = bWall[0];
	boolean down = bWall[1];
	boolean left = bWall[2];
	boolean right = bWall[3];

	if (up == true)
	{
	}
	else if (down == true)
	{
	}
	else if (left == true)
	{
	}
	else if (right == true)
	{
	}






# 배경
1. 고정: 바닥, 벽 라인
2. 가변: 횟불, 좌우측 통로, 전방 문


# 알고리즘
깊이 우선 탐색 알고리즘으로 사용자의 위치를 보여준다. 





# 규칙
함정이나 잘못 들어갔을 때 처음부터(초기화) 또는 전 갈림길에서 리스폰




# 미니맵




# 구조

1. 시작점 -> 끝점 고정

2. 시작점 <-> 끝점 변경

3. 맵 여러 개

4. 랜덤 시작점와 끝점



# 생성 함수 설계


## mapInit()
x,y -> 30, 30

x, y+1
0, 1 ~ 30, 31


0 1 2 3 4 ..... 29
30 ....

1. y=0 이거나 y=29일 때 for(x=0; x<mapWidth) x=1;
2. 아니면 if(x=0 || x=29) x=1;

- 221번 라인: 나중에 #을 1로 바꾸기


## mapCreate()


사용자가 지나간 길: 0
테두리: 1 = #
벽: 2 = @


3. 상하좌우 갈 수 있는 벽 확인
input(x,y)
현 x,y 위치에서 상하좌우 어느 방향에 벽(2)이 있는지 확인(하나도 선택 안될 수도 있고, 많으면 3개)

- 현재 플레이어 위치에서 상하좌우 방향을 알아내는 함수

- 알아낸 방향에 벽(2)이 있는지 확인
= 알아낸 방향의 좌표를 x+y*width에 대입한 값을 1차원 미로배열 인덱스에 넣어서 벽(2)이 있는지 확인한다.

  상: x, y-1
  하: x, y+1
  좌: x-1, y
  우: x+1, y





벽(2)이 있으면 구조체의 해당 방향 변수를 true로 만들고 true인 방향의 x,y값을 구조체에 저장한다.

없으면 5-1처럼 수행


4. 갈 수 있는 벽 근처에 통로가 1개인지 확인

구조체의 상하좌우 방향 변수 중 true인 방향의 x, y값을 찾아낸다.






사용자가 지나간 길: 0
테두리: 1 = #
벽: 2 = @


if (availableWalls = 0)
	{
		return NONE;
	}else {

	int r = random(availableWalls);
	return bwall[r]; //리턴 방향(0,1,2,3)
	}



# 미로 생성 알고리즘


- 스택에 넣을 때 구조체로 넣기?


1. 함수 시작

2. 초기값 플레이어 위치 설정(시작할 위치 벽을 뚫어놓음) -> 해당 위치를 스택에 push

--- 반복문 시작
3. 상하좌우 갈 수 있는 벽 확인

4. 갈 수 있는 벽 근처에 통로가 1개인지 확인

5. 갈 수 있는 벽이 있으면 랜덤으로 결정

5-1. 없으면 스택에서 pop -> 스택 위에 있는 위치로 이동(현재 위치 x,y를 버리고, 그 전 x,y값을 변수x,y에 저장) -> 3번으로 이동하기 위해 continue;
- getDirection 리턴 값이 NONE일 때 createMap()에서 처리

6. 뚫은 곳을 [x+ width * y] = 0; 배열에 0을 저장

7. 현재 위치(x,y)를 스택에 저장 (for문 밖에 있는 x,y 변수를 통해서 6번에서 저장한 x,y값을 받아서 실행)

--- end

출구: 5-1번
1. 비어있는 공간(2) 중에서 랜덤으로 도착지 설정
2. 플레이어 위치로부터 멀리 떨어진 x,y 값을 통해 검사하며 비어있는 공간에 도착지 설정
3. 5-1번이 랜덤한 값 만큼 실행되었을 때 그 위치를 도착지로 설정한다.(예를 들어 5-1이 10번 실행되면 10번째에서 플레이어의 주변은 갈 수 없는 위치고, 그 위치의 정면을 도착지로 설정)





- 스택에 넣는 자료형




# work

## 1. 배경 만들기
1-1. 고정된 배경을 2차원 배열로 만들기
1-2. [#배경-2]처럼 포인트 될 만한 것 만들기
1-3. 공룡게임 코드 참고해서, 2번을 랜덤하게 나오게 하기
 

## 2. 미로 배열 만들기
1번째
2-1. 0과 1로 미로를 2차원 배열로 만들고, 도착지도 미리 정하기
2-2. 만들어진 배열에 맞춰, 사용자에게 선택지를 제공한다. 

- 2차원 미로 맵에서 사용자가 이동할 때마다 1인칭 맵에서 이동. 
- 2차원에서 통로를 보게 되면 1인칭에서 사용자는 통로를 볼 수 있게


## 3. 시작화면 만들기



## 4. 화면 하단에 미니맵 만들기
4-1. 사용자가 이동한 경로를 보여줄 수 있게 하단에 작게 맵을 만듦.





/*
2번째
사용자의 선택에 따라 미로를 생성한다.
사용자가 B와 C 선택지 중 B를 정할 경우, B를 방문하고 B를 스택에 넣는다
그다음 B의 인접노드인 D와 E 선택지 중에서 선택을 한다.

만약 D를 선택했을 때 다음 인접 노드, 즉 길이 없을 경우 사용자는 B로 돌아가게 되며 스택에서 D를 없앤다.


- B를 선택할 경우, B를 방문한 뒤에 B를 스택에 넣는다. 즉, 갈림길에서 B를 넣는 게 아니라 갈림길에서 B로 들어간 후에 B를 스택에 넣는다
- 만약 B로 돌아간다 할 경우, B와 C 갈림길로 돌아가는 것이 아니라 B로 들어간 곳부터 시작하므로 D와 E 선택지를 정할 수 있다.

고민: 인접노드를 어떻게 체크할 것인지(B를 선택한다 했을 때 C는 어떻게 저장할 것인지... 만약 돌아온다면 C는 필요함)
*/


















