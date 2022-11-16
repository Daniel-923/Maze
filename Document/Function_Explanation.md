

# 1. 시간 , 2. 플레이어 각도로 좌표 이동

```c++
auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

fPlayerX += sinf(fPlayerA) * fSpeed * fElapsedTime;;
			fPlayerY += cosf(fPlayerA) * fSpeed * fElapsedTime;;
```

프로그램의 속도, 즉 단위 시간 당 cpu 할당량 상관없이 사용자가 2개의 입력 사이의 시간차이와 속도를 가지고 현재 플레이어가 움직인 거리를 계산한다.

cpu 할당량 1초에 1칸씩 간다고 했을 때 사용자가 3칸 가기 위해 키보드를 누르면 cpu타임은 3초입니다.

만약 cpu 실행속도가 느려지면 3칸가는게 1칸으로 줄어듭니다. 왜냐하면 계산량이 줄어들기 때문입니다.

이 코드는 속도와 위에서 구한 시간, 각도를 곱해서 플레이어가 움직인 거리를 구합니다.

여기서 삼각함수를 통해서 어느 각도든 간단히 계산하기 위해 sin cos을 사용


(속도와 시간을 통해 현재 플레이어가 움직인 거리를 알아낸다.)
(sin cosf을 통해서 어느 각도든 좌표 이동을 간단하게 계산 가능)

(시스템의 에러, 버그 가능성이 일어나 실행시간이 변경될 수도 있다)
(cpu 할당량 1초에 1칸씩 간다고 했을 때 사용자가 1칸 가기 위해 키보드를 눌렀는데 cpu에서 랙으로 인해 1초를 3초로 계산해 3칸을 이동시킬 수 있다.)


# 3. 미로생성 알고리즘

![image](https://user-images.githubusercontent.com/81403841/202256556-26a418df-d233-479d-9b82-f932dd89638d.png)




# 4. fDepth(ray casting), distanceToWall

ray casting 개념과 depth, distanceToWall 예시를 설명


distanceToWall은 빛이 플레이어 위치에서부터 벽에 부딪힌 지점까지의 거리를 나타낸다.


depth는 이 거리가 어느 지점 이상일 때 안보이게 나타내기 위해 사용됨.





아래 코드를 통해서 distancetowall을 통해 중간에 만났을 때 저장된 이 값을 통해 벽을 나타냄

```c++
// Incrementally cast ray from player, along ray angle, testing for
			// intersection with a block
			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += fStepSize;
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

```




(rayangle은 fov값을 이용한 바라보는 시야각도들, fEye는 방향을 찾아내는(갖는) 벡터값) //그냥 알고 있어야 할 개념
(벡터 값을 곱해서 여러 방향에서 distanceToWall를 더해가며 벽과 부딪혔는지 계산한다)




 
# 5. 벽 사이가 가까우면(중첩하면) 중첩한 부분을 표시 안하고 boundary 처리를 한다.
그래서 벽이 몇개 씩 존재하는 것처럼 보임.

boundary는 벽과 벽 사이에 있는 경계선을 나타냄

벽끼리 가까이(붙어) 있는 거를 추출하기 위해 정렬하는 코드


```c++
// Ray is inbounds so test to see if the ray cell is a wall block
					if (map.c_str()[nTestX * nMapWidth + nTestY] == '#')
					{
						// Ray has hit wall
						bHitWall = true;

						// To highlight tile boundaries, cast a ray from each corner
						// of the tile, to the player. The more coincident this ray
						// is to the rendering ray, the closer we are to a tile
						// boundary, which we'll shade to add detail to the walls
						vector<pair<float, float>> p;

						// Test each corner of hit tile, storing the distance from
						// the player, and the calculated dot product of the two rays
						for (int tx = 0; tx < 2; tx++)
							for (int ty = 0; ty < 2; ty++)
							{
								// Angle of corner to eye
								float vy = (float)nTestY + ty - fPlayerY;
								float vx = (float)nTestX + tx - fPlayerX;
								float d = sqrt(vx * vx + vy * vy);
								float dot = (fEyeX * vx / d) + (fEyeY * vy / d);
								p.push_back(make_pair(d, dot));
							}

						// Sort Pairs from closest to farthest
						sort(p.begin(), p.end(), [](const pair<float, float> &left, const pair<float, float> &right)
							 { return left.first < right.first; });

						// First two/three are closest (we will never see all four)
						float fBound = 0.0021;
						if (acos(p.at(0).second) < fBound)
							bBoundary = true;
						if (acos(p.at(1).second) < fBound)
							bBoundary = true;
						if (acos(p.at(2).second) < fBound)
							bBoundary = true;
					}

```
