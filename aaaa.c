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
		do //�� 2�������� ?
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
			if (i == point) // ���� �����ϴ� ������
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2); // �� �ٲٰ�
				if (i == select1 || i == select2) // ������ ī�� 1,2
					printf(" %c ", set2[i]);	// ��� ���
				else
					printf(" %c ", set[i]);	// �ƴϸ� ����ī��
			}
			else if (i == select1 || i == select2) // ������ ���� ī���
			{
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5); 
				printf(" %c ", set2[i]); // �״�� ��� ���
			}
			else						// �������� �ʰ� �ִ� �ٸ� �迭����
			{
				if (set[i] != '?')
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 3);
				else
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
				printf(" %c ",set[i]);		// ����ī��� ���
			}

			if ((i + 1) % 7 == 0)
				printf("\n");
		}
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
		printf("------------------- ���� : %d �� -------------------\n", score);
		printf("\n< �̵� : ����� , Ȯ�� : Space bar , ���� : ESC , ���� : ! >\n");

		if (select1 != -1&& select2 != -1&&select1!=select2)	// �ΰ��� ī�带 �������ٸ�
			result(&score,set, set2, &select1, &select2); 

		if (key == 33)
		{
			printf("������ ??? < 3�� ���� �Է� > : ");
			while (1)
			{
				if (strlen(gets(ans)) > 10)
					printf("�ٽ� �Է�!!!!!\n");
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
	*score -= 5; // �߹��ϰ� 5�� ��� ����
	if (set2[*select1] == set2[*select2]) // �� ī�尡 ���� ī����
	{									// ����ī�忡 ���ī�� �Է�
		set[*select1] = set2[*select1];
		set[*select2] = set2[*select2];
	}
		*select1 = -1;				// Ȯ���� �ٽ� ī�� ���� ����
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
		printf("������ �����Դϴ� ����!!!!!!!\n");
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
		printf("\n********* �����մϴ�. ��� �����Դϴ�.**********\n\n");
		strcpy(ans, "111");
	}
	else if (k > 0)
		printf(" ---%d�� ���� !!\n", k);
	else
		printf("����!!!!\n");
}
