#include<stdio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>

void input(char*,char*);
void play(char*,char*,char*);
void keybord(int* , int* , int* , int* );
void result(int*, char*, char*, int*, int*);
void done(char*,char*,int*);

int main()
{
	int i,j;
	int key;
	char set[7][7], set2[7][7],ext[3];
	srand(time());

	for (i = 0; i < 7; i++)
		for (j = 0; j < 7; j++)
		{
			set[i][j] = '?';
			set2[i][j] = '?';
		}

	input(set2,ext);
	play(set,set2,ext);
	printf("\nbye bye\n\n");
}

void input(char* set,char* ext) 
{
	int i,com;
	do 
	{
		for (i = 0; i < 3; i++)
		{
			ext[i] = rand()%26 + 65;
			printf("%c\n", ext[i]);
		}
	} while (ext[0] == ext[1] || ext[0] == ext[2] || ext[1] == ext[2]);

	for (i = 65; i < 91; i++)
	{
		if (ext[0] != i && ext[1] != i &&ext[2] != i)
		{
			do
			{
				com = rand() % 49;
			} while (set[com] != '?');
			set[com] = i;
		}
		do //왜 2번쓴건지 ?
		{
			com = rand() % 49;
		} while (set[com] != '?');
		set[com] = i;
	}
}

void play(char* set,char* set2,char* ext)
{
	int score = 1000, key = 0, i, count = 0;
	int point = 0, select1 = -1, select2 = -1;
	char ans[10];
	while (point!=-1)
	{
		system("cls");
		for (i = 0; i < 49; i++)
		{
			if (i == point) // 현재 지정하는 포인터
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); // 색 바꾸고
				if (i == select1 || i == select2) // 뒤집은 카드 1,2
					printf(" %c ", set2[i]);	// 결과 출력
				else
					printf(" %c ", set[i]);	// 아니면 랜덤카드
			}
			else if (i == select1 || i == select2) // 뒤집어 놓은 카드는
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5); 
				printf(" %c ", set2[i]); // 그대로 결과 출력
			}
			else						// 지정하지 않고 있는 다른 배열들은
			{
				if (set[i] != '?')
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
				else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf(" %c ",set[i]);		// 랜덤카드로 출력
			}

			if ((i + 1) % 7 == 0)
				printf("\n");
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("------------------- 점수 : %d 점 -------------------\n", score);
		printf("\n< 이동 : →←↑↓ , 확인 : Space bar , 종료 : ESC , 정답 : ! >\n");

		if (select1 != -1&& select2 != -1&&select1!=select2)	// 두개의 카드를 뒤집었다면
			result(&score,set, set2, &select1, &select2); 

		if (key == 33)
		{
			printf("정답은 ??? < 3개 같이 입력 > : ");
			while (1)
			{
				if (strlen(gets(ans)) > 10)
					printf("다시 입력!!!!!\n");
				else
					break;
			}
			done(ans, ext,&count);
			if (strcmp(ans, "111") == 0)
			{
				for (i = 0; i < 49; i++)
				{
					printf(" %c ", set2[i]);
					if ((i + 1) % 7 == 0)
						printf("\n");
				}
				return;
			}
			score -= count * 10;
		}
		keybord(&key, &point, &select1, &select2);
	}
}

void keybord(int* key, int* point,int* select1, int* select2)
{
		*key = getch();
		if (*key == 224)
		{
			*key = getch();
			if (*key == 72)
			{
				if (*point > 6)
					*point -= 7;
			}
			else if (*key == 80)
			{
				if (*point < 42)
					*point += 7;
			}
			else if (*key == 75)
			{
				if (*point % 7 != 0)
					*point -= 1;
			}
			else if (*key == 77)
			{
				if (*point % 7 != 6)
					*point += 1;
			}
		}
		else if (*key == 32)
		{
			if (*select1 != -1)
				*select2 = *point;
			else
				*select1 = *point;
		}
		else if (*key == 27)
			*point = -1;
}

void result(int* score,char* set, char* set2, int* select1, int* select2)
{
	*score -= 5; // 야박하게 5점 깍고 시작
	if (set2[*select1] == set2[*select2]) // 두 카드가 같은 카드라면
	{									// 랜덤카드에 결과카드 입력
		set[*select1] = set2[*select1];
		set[*select2] = set2[*select2];
	}
		*select1 = -1;				// 확인후 다시 카드 선택 리셋
		*select2 = -1;
}

void done(char* ans, char* ext,int* count)
{
	int i, j,k;
	*count += 1;
	for (j = 0,k=0; j < strlen(ans); j++)
		if ((ans[j] >= 65 && ans[j] <= 90) || (ans[j] >= 97 && ans[j] < 122))
		{
			printf(" %c ", ans[j]);
			k++;
		}
	printf("\n\n");
	if (k > 3)
	{
		printf("정답은 세개입니다 오답!!!!!!!\n");
		return;
	}
	for (k=0,i = 0; i < 3; i++)
	{
		for ( j = 0; j < strlen(ans); j++)
		{
			if ((ans[j] >= 65 && ans[j] <= 90) || (ans[j] >= 97 && ans[j] < 122))
			{
				if (ans[j] == ext[i] || ans[j] - 32 == ext[i])
				{
					printf(" %c ", ans[j]);
					k += 1;
				}
			}
		}
	}
	if (k == 3)
	{
		system("cls");
		printf("\n********* 축하합니다. 모두 정답입니다.**********\n\n");
		strcpy(ans, "111");
	}
	else if (k > 0)
		printf(" ---%d개 정답 !!\n", k);
	else
		printf("오답!!!!\n");
}
