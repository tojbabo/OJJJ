/*#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void menu(); // 메뉴에서 다른 함수로 넘어가는 루프를 돌리는 함수
void menu2(); // 메뉴를 출력하는 함수
void readData(int*, FILE*); // 난수를 발생시키는 함수
void searchNumber(int*, int*, FILE*); // 오름차순 순번의 숫자를 찾기 위한 함수

int main()
{
	menu2();
	menu();
	return 0;
}

void menu2()
{
	puts("\n1. 난수 생성");
	puts("2. 오름차순 순번 숫자 찾기");
	puts("3. 프로그램 종료");
	printf("\n원하는 메뉴 번호를 입력하세요 : ");
}

void menu()
{
	float num;
	int  n; // n : 난수의 갯수를 저장하는 변수 , num : 메뉴 번호가 입력될 변수
	int* p = NULL; // 난수가 저장될 동적메모리 포인터
	FILE* stream = NULL;

	while (1) // 메뉴 프로그램을 종료하기 전까지 계속 반복되도록 하는 무한 루프
	{
		num = 0;
		scanf("%f", &num);
		getchar();
		if (num == 1.0) // 1번을 입력시 난수를 발생시키는 함수로
			readData(&n, stream);
		else if (num == 2.0)// 2번을 입력시 난수의 위치를 찾는 함수로
			searchNumber(&n, p, stream);
		else if (num == 3.0)// 3번 입력 시 프로그램 종료
			return;
		else        // 메뉴 이외의 번호를 입력하였을 때 오류 발생 메뉴 재입력
			printf("\n잘못 입력 하였습니다. 메뉴 번호를 다시 입력해 주세요 : \n");
	}
}


void readData(int* n, FILE* stream)
{
	char eng;
	int i;

	printf("\n난수를 생성 하시겠습니까 <y/n> : ");
	scanf("%c", &eng);
	getchar();

	while (1)
	{
		if (eng == 'y' || eng == 'Y') // 대 소문자에 의한 오류 방지
		{
			printf("\n몇개의 난수를 생성 하시겠습니까 : ");
			if ((scanf("%d", n) == 0) || *n == 0) // 갯수에 숫자가 아닌 문자를 넣거나 0을 넣었을때 오류 발생
			{
				printf("\n오류 입니다. 생성할 난수를 다시 입력하세요\n");
				getchar();
			}
			else
			{
				getchar();
				srand(time(NULL)); // 발생 하는 난수가 계속 다르게 하기 위해 시드 설정
				stream = fopen("data.dat", "w"); // 파일 이름 data.dat 과 쓰기 모드 w
				for (i = 0; i < *n; i++) // 사용자가 입력한 난수 갯수 n 번만큼 난수 입력
				{
					fprintf(stream, "%-5d ", rand() % 100); // stream에 n개 만큼의 난수 저장 
					if ((i + 1) % 10 == 0) // 파일에 입력되는 난수가 10개 씩 줄띄기를 하기 위한 조건문
						fprintf(stream, "\n");
				}
				printf("\n0과 100사이의 난수 %d개가 생성 되었습니다.\n", *n);
				menu2(); // 작업이 정상적으로 완료 되었을때 다시 메뉴 호출
				fclose(stream);
				return;
			}
		}
		else if (eng == 'N' || eng == 'n') // 대 소문자에 의한 오류 방지
		{
			printf("\n원하는 메뉴 번호를 다시 입력하세요 : "); // 난수 생성을 원하지 않을경우 메뉴는 호출하지 않음
			return;
		}
		else
		{
			printf("\n오류 입니다. 다시 입력해 주세요 <y/n> : "); // 잘못 입력 할 경우 재입력
			scanf("%c", &eng);
			getchar();
		}
	}
}

void searchNumber(int* n, int* p, FILE* stream)
{
	int num, i, j; // num : 찾고자 하는 순번 i,j : 루프문을 돌리기 위한 변수
	int sml = 0, big, max, count; // sml : 순번에 맞는 가장 작은 수, count : 순번에 맞는 가장 작은 수가 있는 위치 , big : 비교할 수, max : 가장 큰 난수
	if (((stream = fopen("data.dat", "r")) == NULL) || (*n) <= 0) // 파일이 없거나 난수를 생성하지 않았을때는 오류
	{
		printf("\n난수가 생성되지 않았습니다.\n");
		printf("\n원하는 메뉴 번호를 다시 입력하시오 : ");
		return;
	}
	else
	{
		printf("\n몇 번째 오름순번의 숫자를 찾으시겠습니까 ? : ");
		while (1)
		{
			if (scanf("%d", &num) == 0 || num == 0) // 찾으려는 숫자에 문자를 입력하거나 0을 입력하였을때 오류 발생
			{
				getchar();
				printf("\n오류 입니다. 찾으려는 오름순번의 숫자를 다시 입력하세요 : ");
			}
			else
			{
				p = (int*)malloc(sizeof(int)*(*n));
				for (i = 0, max = 0; i < *n; i++)
				{
					fscanf(stream, "%d", &p[i]); // 난수가 저장된 파일에서 차례대로 배열에 저장
					if (max <= p[i])
						max = p[i]; // 난수를 배열에 저장함과 동시에 난수 중 가장 큰 난수를 max에 저장
				}
				for (i = 0; i < num; i++)
				{
					if (sml == max)
					{
						printf("\n%d 번째로 작은 난수는 존재하지 않습니다.\n", num);
						fclose(stream);
						free(p);
						menu2();
						return;
					}
					for (j = 0, big = max; j < *n; j++)
					{
						if (p[j] > sml&&p[j] <= big)
						{
							big = p[j];
							count = j;
						}
					}
					sml = big;
				}
				printf("\n%d번째로 작은 숫자는 %d이며 %d번째에 위치해 있습니다. \n", num, sml, count + 1);
				fclose(stream);
				free(p);
				menu2();
				return;
			}
		}
	}
}*/