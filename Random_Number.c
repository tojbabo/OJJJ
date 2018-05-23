#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void menu(); // 메뉴에서 다른 함수로 넘어가는 루프를 돌리는 함수
void menu2(); // 메뉴를 출력하는 함수
void readData(float*, FILE*); // 난수를 발생시키는 함수
void searchNumber(float*, int*, FILE*); // 오름차순 순번의 숫자를 찾기 위한 함수

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
	float num; // 소수 입력과 같은 오류를 방지하기 위해 실수형으로 선언한다
	float  n; // n : 난수의 갯수를 저장하는 변수 , num : 메뉴 번호가 입력될 변수
	int* p = NULL; // 난수가 저장될 동적메모리 포인터
	FILE* stream = NULL;

	while (1) // 메뉴 프로그램을 종료하기 전까지 계속 반복되도록 하는 무한 루프
	{
		num = 0;
		scanf("%f", &num); // 무한루프에 걸리게 하기 위해서 메뉴 값은 루프 안에서 따로 받는다
		getchar();
		if (num == 1.0) // 1번을 입력시 난수를 발생시키는 함수로
			readData(&n, stream);
		else if (num == 2.0)// 2번을 입력시 난수의 위치를 찾는 함수로
			searchNumber(&n, p, stream);
		else if (num == 3.0)// 3번 입력 시 프로그램 종료
			return;
		else        // 메뉴 이외의 번호를 입력하였을 때 오류 발생 메뉴 재입력
			printf("\n잘못 입력 하였습니다. 메뉴 번호를 다시 입력해 주세요 : ");
	}
}


void readData(float* n, FILE* stream)
{
	char eng; // 난수 생성 y/n 를 받는 변수
	int i; // 난수를 

	printf("\n난수를 생성 하시겠습니까 <y/n> : ");
	scanf("%c", &eng);
	getchar();

	while (1)
	{
		if (eng == 'y' || eng == 'Y') // 대 소문자에 의한 오류 방지
		{
			printf("\n몇개의 난수를 생성 하시겠습니까 : ");
			if ((scanf("%f", n) == 0) || *n == 0) // 갯수에 숫자가 아닌 문자를 넣거나 0을 넣었을때 오류 발생
			{
				printf("\n오류 입니다. 생성할 난수를 다시 입력하세요\n");
				getchar();
			}
			else if ((i = *n) != *n) // 갯수에 소수를 입력하였을때에도 오류 발생
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
						fprintf(stream, "\n"); // 난수가 10개가 생성될 때마다 한 줄씩 띄운다
				}
				printf("\n0과 100사이의 난수 %.0f개가 생성 되었습니다.\n", *n);
				menu2(); // 작업이 정상적으로 완료 되었을때 다시 메뉴 호출
				fclose(stream); // 파일 작업 후엔 스트림 닫기
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
			scanf("%c", &eng); // y/n 문 같은경우 반복문 밖에 있기 때문에 잘못 입력하였을 경우 재 입력을 받는다
			getchar();
		}
	}
}

void searchNumber(float* n, int* p, FILE* stream)
{
	float num; // 찾고자 하는 순번, 소수와 같은 입력 오류를 방지하기 위해 실수형으로 선언한다
	int i, j; // i,j : 루프문을 돌리기 위한 변수
	int sml = 0, big, max, count; // sml : 순번에 맞는 가장 작은 수, count : 순번에 맞는 가장 작은 수가 있는 위치 , big : 비교할 수, max : 가장 큰 난수
	if (((stream = fopen("data.dat", "r")) == NULL) || (*n) <= 0) // 파일이 없거나 난수를 생성하지 않았을때는 오류, 
	{															// 1번 메뉴를 실행하지 않을시, 파일이 존재하지 않거나, 난수를 0개 발생시켰을시 *n값은 0이다
		printf("\n난수가 생성되지 않았습니다.\n");
		printf("\n원하는 메뉴 번호를 다시 입력하시오 : ");
		return;
	}
	else
	{
		printf("\n몇 번째 오름순번의 숫자를 찾으시겠습니까 ? : ");
		while (1) // 함수가 정상작동 하기 전까지는 무한 반복하게 된다
		{
			if (scanf("%f", &num) == 0 || num == 0) // 찾으려는 숫자에 문자를 입력하거나 0을 입력하였을때 오류 발생
			{
				getchar();
				printf("\n오류 입니다. 찾으려는 오름순번의 숫자를 다시 입력하세요 : ");
			}
			else if ((i = num) != num) // 찾으려는 난수에 소수를 입력하였을 경우 오류가 발생한다.
			{
				printf("\n오류 입니다. 찾으려는 오름순번의 숫자를 다시 입력하세요 : \n");
				getchar();
			}
			else
			{
				p = (int*)malloc(sizeof(int)*(int)(*n)); // 난수의 갯수만큼 동적메모리 생성
				for (i = 0, max = 0; i < *n; i++) // 난수의 처음부터 끝까지 반복문을 돌리면서 동적메모리에 난수 저장
				{
					fscanf(stream, "%d", &p[i]); // 난수가 저장된 파일에서 차례대로 배열에 저장 
					if (max <= p[i])
						max = p[i]; // 난수를 배열에 저장함과 동시에 난수 중 가장 큰 난수를 max에 저장
				}
				for (i = 0; i < num; i++) // 찾으려는 오름차순 순번num 번만큼 반복
				{
					if (sml == max) // 가장 큰 난수와 지금까지 찾아온 숫자가 같을경우 오류발생
					{
						printf("\n%.0f 번째로 작은 난수는 존재하지 않습니다.\n", num);
						fclose(stream); // 함수가 정상적으로 끝날때 스트림을 종료한다
						free(p); // 함수가 정상적으로 마쳐질때 동적메모리도 종료
						menu2();
						return;
					}
					for (j = 0, big = max; j < *n; j++) // 난수의 처음부터 끝까지 가장 작은 수를 찾는 루프
					{
						if (p[j] > sml&&p[j] <= big) // 현재의 j번째 난수가 가장 작은 난수인지 판별하는 조건문
						{
							big = p[j]; // j번째 난수가 조건에 맞다면 현재의 난수로 지정
							count = j; // 현재의 난수의 위치를 기억해 놓는다
						}
					}
					sml = big; // 루프문이 끝날때 까지 남아있는 현재의 난수가 가장 작은 난수다
				}
				printf("\n%.0f번째로 작은 숫자는 %d이며 %d번째에 위치해 있습니다. \n", num, sml, count + 1);
				fclose(stream); // 함수가 정삭적으로 끝마칠때는 스트림 종료
				free(p); // 함수가 정상적으로 끝마쳐질때는 동적메모리도 종료
				menu2(); // 함수가 정상적으로 끝날때 메뉴함수 다시 호출
				return;
			}
		}
	}
}