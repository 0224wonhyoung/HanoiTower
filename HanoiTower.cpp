#include <stdio.h>
#include <stack>
#include <bangtal.h>

using namespace std;

int N = 7; // 원판 갯수. 1이 제일 작은 원판

stack<int> tower[3];
int moveDirection[2][128];
int count1 = 0, count2 =0;

SceneID scene1;
ObjectID stick[4];
ObjectID piece[10];
ObjectID startButton;
TimerID timer1;


const char* imgFileLocate[11] = { 0, "Images/hanoi1.jpg", "Images/hanoi2.jpg", "Images/hanoi3.jpg",
"Images/hanoi4.jpg", "Images/hanoi5.jpg", "Images/hanoi6.jpg", "Images/hanoi7.jpg" };

bool playing = false; // 애니매이션 재생 중 or not


// n번 조각 이미지 오브젝트를 towerNum번 타워의 h번째로 이동
void locateImg(int n, int towerNum, int h) {
	locateObject(piece[n], scene1, 300 * towerNum +245, 50 + 35 * h);
}

void init() {
	for (int i = 1; i <= N; i++) {
		if (!tower[0].empty()) tower[0].pop();
		if (!tower[1].empty()) tower[1].pop();
		if (!tower[2].empty()) tower[2].pop();
	}
	for (int i = N; i >=1 ; i--) {
		tower[0].push(i);
		locateImg(i, 0, N - i);
	}
	count2 = 0;
}

void hanoi(int n, int from, int to, int temp) {
	if (n == 1) {
		moveDirection[0][count1] = from;
		moveDirection[1][count1] = to;
		count1++;
	}
	else {
		hanoi(n - 1, from, temp, to);
		moveDirection[0][count1] = from;
		moveDirection[1][count1] = to;
		count1++;
		hanoi(n - 1, temp, to, from);
	}
}

void move() {
	int from = moveDirection[0][count2];
	int to = moveDirection[1][count2];
	int num = tower[from].top();
	tower[from].pop();
	int height = tower[to].size();
	tower[to].push(num);	
	locateImg(num, to, height);
	
	count2++;
	
}

void mouseCallback(ObjectID object, int x, int y, MouseAction) {
	if (object == startButton) {
		playing = true;
		hideObject(startButton);
		init();

		setTimer(timer1, 0.3f);
		startTimer(timer1);
	}
}

void timerCallback(TimerID timer) {
	if (timer == timer1) {
		//printf("HERE\n");
		move();
		if (count1 == count2) {
			playing = false;
			setObjectImage(startButton, "Images/restart.png");
			showObject(startButton);
		}
		else {
			setTimer(timer1, 0.3f);
			startTimer(timer1);
		}
		
	}
}


int main()
{
	hanoi(N, 0, 1, 2);
	/*
	for (int i = 0; i < ct; i++) {
		printf("%d -> %d\n", moveDirection[0][i], moveDirection[1][i]);
	}
	printf("총 %d회", ct);*/
	setMouseCallback(mouseCallback);
	setTimerCallback(timerCallback);

	scene1 = createScene("하노이타워", "Images/배경.jpg");

	int i ;

	for (i = 0; i < 3; i++) {
		stick[i] = createObject("Images/tower.png");
		locateObject(stick[i], scene1, 300 * i + 350, 50);
		showObject(stick[i]);
	}

	for (i = 1; i <= N; i++) {
		piece[i] = createObject(imgFileLocate[i]);
		showObject(piece[i]);
	}

	startButton = createObject("Images/start.png");
	locateObject(startButton, scene1, 620, 500);
	showObject(startButton);

	timer1 = createTimer(0.3f);
	hideTimer();

	
	startGame(scene1);
}