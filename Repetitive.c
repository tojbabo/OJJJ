#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct order // 좌표가 저장될 구조체
{
	int x, y; // x y 멤버 변수
}order;

void input(char**, int, int); // 원하는 행렬을 정하고 출력여부와 반복여부를 물을 함수
void check(char*); // 입력받은 문자열을 다듬는 함수
char** init(int*, int*); // 파일로 부터 배열에 넣는 함수
void show(char**, int, int); // 배열을 보여주는 함수
void fill(int, int, int, int, char**, char*); // 시작 점부터 퍼뜨려 나가는 함수

int main()
{
	int row, column, max_row, max_column; // row : 시작 행, column : 시작 열 , max_row : set의 행크기, max_column : set의 열크기
	char** set = NULL; // 파일에서 읽어온 배열
	char yon[5]; // 의사 여부를 저장할 변수
	set = init(&max_row, &max_column); // 파일로 부터 읽어서 set에 저장함

	show(set, max_row, max_column); // 현재 배열을 보여줌

	input(set, max_row, max_column); 
	
	return 0;
}

void  input(char** set, int max_row, int max_column)
{
	int row, column;
	char yon[20];


	while (1)
	{
		printf("Enter the row number < 0 ~ %d > : ", max_row - 1);
		while (1) // 0부터 배열의 최대 행을 넘어가지 않는 값만 입력 받는다
		{
			scanf("%s", yon); // 원하는 행을 문자열로 받아온다
			for (row = 0; row<strlen(yon); row++) // 받아온 문자열중 숫자가 아닌것 문자가 있는지 검사하는 반복문
				if (yon[row] < 48 || yon[row]>57)
				{
					printf("Error! enter again : "); // 문자열중 숫자가 아닌 문자가 있다면 오류
					row = -1;
					break;
				}
			if (row != -1)  // 문자열중 모든 문자가 숫자라면
			{
				row = atoi(yon); // row 에 행 값을 입력
				if (row<0 || row>max_row - 1) // row가 최소-최대 사이에 존재 하지 않는다면
					printf("Error, enter again : "); // 오류 출력
				else
					break;
			}
		}

		printf("Enter the column number < 0 ~ %d > : ", max_column - 1);
		while (1) // 0에서 부터 배열의 최대 열을 넘어가지 않는 값만 입력 받는다
		{
			scanf("%s", yon); // 원하는 열값을 문자열로 입력 받는다
			for (column = 0; column<strlen(yon); column++) // 입력받은 문자열 중 숫자가 아닌 문자가 있는지 검사하는 반복문
				if (yon[column] < 48 || yon[column]>57) //문자열중 숫자가 아닌 문자가 존재한다면
				{
					printf("Error! enter again : "); // 오류 출력
					column = -1;
					break;
				}
			if (column != -1) // 문자열중 모든 문자가 숫자라면
			{
				column = atoi(yon); // column에 입력
				if (column<0 || column>max_column - 1) // column의 값이 최소-최대를 벗어날시 오류출력
					printf("Error, enter again : ");
				else
					break;
			}
		}

		printf("Do you want to print out step by step ? < yes / no> : "); //단계별로 출력 여부를 물음
		while (1)
		{
			scanf("%s", yon);  // 문자열로 입력받음
			check(yon); // 문자열을 모두 소문자화
			if (strcmp(yon, "yes") == 0 || strcmp(yon, "no") == 0) // 입력받은 내용이 yes 혹은 no 일때
			{
				fill(row, column, max_row, max_column, set, yon); // fill 함수 호출
				break;
			}
			else
				printf("Enter again yes or no : "); // yes 혹은 no 가 아닐때 재 입력
		}

		printf("Go again ? < yes / no > : "); // 반복할 여부를 물음
		while (1)
		{
			scanf("%s", yon);
			check(yon);
			if (strcmp(yon, "no") == 0) // 다시 하고 싶지 않다면 반복문을 벗어난다
				return 0;
			else if (strcmp(yon, "yes") == 0) // 다시 하고 싶다면
				break; // 반복할 여부를 묻는 반복문만 빠져나옴
			else
				printf("Enter again. yes or no : "); // 그 외 문자 입력시 재입력
		}
	}
}

void check(char* eng)
{
	int i;
	for (i = 0; i<strlen(eng); i++) 
	{
		if (eng[i] >= 65 && eng[i] <= 90)
			eng[i] += 32; // 문자열중 대문자는 소문자로 변환
	}
}

char** init(int* max_r, int* max_c)
{
	int x = 0, y = 0, i; // x : 파일의 총 행 수가 저장될 변수, y : 파일의 한줄의 데이터가 몇개인지 저장될 변수, i : 반복문을 돌리기 위한 변수
	char** set = NULL; // 파일에서 읽어올 데이터가 저장될 배열 포인터
	char read[100]; // 파일에서 한줄씩 읽어서 저장할 배열
	char name[20]; // 파일의 이름을 저장할 배열
	FILE* stream = NULL;
	while (1) { // 입력한 해당 파일이 존재할때 까지 입력을 받는 반복문
		printf("Enter the name of data file : ");
		scanf("%s", name);
		if (stream = fopen(name, "r") == 0) // 파일이 존재하지 않는다면
			printf("error! please retry\n"); // 파일 재입력
		else
			break;
	}
	stream = fopen(name, "r"); // 사용자가 원하는 파일을 읽어옴
	while (1) // 해당 파일의 처음부터 끝까지 사이즈를 계산하는 반복문
	{
		if (fgets(read, 100, stream) == 0)
			break;
		x++; // 한줄 한줄 읽어나갈떄 마다 x 값 증가
	}
	y = strlen(read); // 한줄의 데이터가 총 몇개인지 y에 저장
	rewind(stream); // 파일의 포인터를 맨 앞으로 다시 옮김
	set = (char**)malloc(x); // 파일의 열 수 만큼 2차원 포인터에 행 수를 동적 할당
	for (i = 0; i < x; i++) // 파일의 데이터 수 만큼 
		set[i] = (char*)malloc(y); // 각각의 행에 열 생성

	for (i = 0; i < x; i++) // 파일의 맨 처음부터 끝까지 읽어올 반복문
	{
		fgets(read, 100, stream);
		strcpy(set[i], read); // 한줄한줄 마다 각각의 배열에 입력
		set[i][y] = '\0'; // 각각의 행 맨 마지막 열엔 종료 문자 입력
	}
	fclose(stream); // 파일 닫음
	*max_r = x; // 파일 속 데이터의 최대 행 수 저장
	*max_c = y; // 파일 속 데이터의 최대 열 수 저장
	return set; // 만들어진 2차원 배열 반환.
}

void show(char** set, int max_r, int max_c)
{
	int i, j;
	printf("\n");
	for (i = 0; i < max_r; i++) // 배열의 내용을 출력할 반복문
	{
		printf("		");
		for (j = 0; j <= max_c; j++)
			printf(" %c ", set[i][j]); // 배열의 해당 행렬에 해당하는 데이터 출력
		printf("\n");
	}
	printf("\n");
}

void fill(int x, int y, int max_x, int max_y, char** set, char* yon)
{
	int index = -1; // 스택의 인덱스를 가리킬 변수
	order* stk; // 좌표값이 저장될 스택
	stk= (order*)malloc(sizeof(int)*(max_y*max_x));
	if (set[x][y] != '-') // 해당 좌표의 내용이 - 이 아닐경우
	{
		printf("\nthis is not blank \n\n");
		return;
	}
	while (1)
	{
		if (set[x][y] == '-') // 해당 좌표의 내용이 - 이 맞을 경우
		{
			set[x][y] = '@'; // - 을 @로 바꿈
			index += 1; // 인덱스를 0으로 옮기면서
			stk[index].x = x; // 스택에 해당 좌표를 입력
			stk[index].y = y; 
			if (strcmp(yon, "yes") == 0)// 과정을 출력하고 싶을경우
				show(set, max_x, max_y);  // @로 바뀔때마다 출력
		}
		if (x>0 && set[x - 1][y] == '-') // 해당 위치에서 위쪽이 '-'일 경우
			x--; // 좌표이동
		else 
			if (y>0 && set[x][y - 1] == '-') // 해당 위치에서 왼쪽이 '-'일 경우
				y--; // 좌표이동
			else
				if (y<max_y-1&&set[x][y + 1] == '-') // 해당 위치에서 오른쪽이 '-'일 경우
					y++; // 좌표이동
				else
					if (x<max_x-1&&set[x + 1][y] == '-') // 해당 위치에서 아래쪽이 '-'일 경우
						x++; // 좌표이동
					else
						if (index == 0) // 저장된 좌표가 없을경우
						{
							free(stk); 
							break; // 반복문에서 빠져나옴
						}
						else // 저장된 좌표가 있을경우
						{
							index--; // 이전 좌표로 인덱스를 넘기고
							x = stk[index].x; // 각각의 좌표에 이전 좌표값 입력
							y = stk[index].y;
						}
	}
	if (strcmp(yon, "no") == 0) // 결과만을 출력하기 원한다면
		show(set, max_x, max_y); // 마지막에 배열 출력
}