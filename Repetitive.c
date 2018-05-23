#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct order // ��ǥ�� ����� ����ü
{
	int x, y; // x y ��� ����
}order;

void input(char**, int, int); // ���ϴ� ����� ���ϰ� ��¿��ο� �ݺ����θ� ���� �Լ�
void check(char*); // �Է¹��� ���ڿ��� �ٵ�� �Լ�
char** init(int*, int*); // ���Ϸ� ���� �迭�� �ִ� �Լ�
void show(char**, int, int); // �迭�� �����ִ� �Լ�
void fill(int, int, int, int, char**, char*); // ���� ������ �۶߷� ������ �Լ�

int main()
{
	int row, column, max_row, max_column; // row : ���� ��, column : ���� �� , max_row : set�� ��ũ��, max_column : set�� ��ũ��
	char** set = NULL; // ���Ͽ��� �о�� �迭
	char yon[5]; // �ǻ� ���θ� ������ ����
	set = init(&max_row, &max_column); // ���Ϸ� ���� �о set�� ������

	show(set, max_row, max_column); // ���� �迭�� ������

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
		while (1) // 0���� �迭�� �ִ� ���� �Ѿ�� �ʴ� ���� �Է� �޴´�
		{
			scanf("%s", yon); // ���ϴ� ���� ���ڿ��� �޾ƿ´�
			for (row = 0; row<strlen(yon); row++) // �޾ƿ� ���ڿ��� ���ڰ� �ƴѰ� ���ڰ� �ִ��� �˻��ϴ� �ݺ���
				if (yon[row] < 48 || yon[row]>57)
				{
					printf("Error! enter again : "); // ���ڿ��� ���ڰ� �ƴ� ���ڰ� �ִٸ� ����
					row = -1;
					break;
				}
			if (row != -1)  // ���ڿ��� ��� ���ڰ� ���ڶ��
			{
				row = atoi(yon); // row �� �� ���� �Է�
				if (row<0 || row>max_row - 1) // row�� �ּ�-�ִ� ���̿� ���� ���� �ʴ´ٸ�
					printf("Error, enter again : "); // ���� ���
				else
					break;
			}
		}

		printf("Enter the column number < 0 ~ %d > : ", max_column - 1);
		while (1) // 0���� ���� �迭�� �ִ� ���� �Ѿ�� �ʴ� ���� �Է� �޴´�
		{
			scanf("%s", yon); // ���ϴ� ������ ���ڿ��� �Է� �޴´�
			for (column = 0; column<strlen(yon); column++) // �Է¹��� ���ڿ� �� ���ڰ� �ƴ� ���ڰ� �ִ��� �˻��ϴ� �ݺ���
				if (yon[column] < 48 || yon[column]>57) //���ڿ��� ���ڰ� �ƴ� ���ڰ� �����Ѵٸ�
				{
					printf("Error! enter again : "); // ���� ���
					column = -1;
					break;
				}
			if (column != -1) // ���ڿ��� ��� ���ڰ� ���ڶ��
			{
				column = atoi(yon); // column�� �Է�
				if (column<0 || column>max_column - 1) // column�� ���� �ּ�-�ִ븦 ����� �������
					printf("Error, enter again : ");
				else
					break;
			}
		}

		printf("Do you want to print out step by step ? < yes / no> : "); //�ܰ躰�� ��� ���θ� ����
		while (1)
		{
			scanf("%s", yon);  // ���ڿ��� �Է¹���
			check(yon); // ���ڿ��� ��� �ҹ���ȭ
			if (strcmp(yon, "yes") == 0 || strcmp(yon, "no") == 0) // �Է¹��� ������ yes Ȥ�� no �϶�
			{
				fill(row, column, max_row, max_column, set, yon); // fill �Լ� ȣ��
				break;
			}
			else
				printf("Enter again yes or no : "); // yes Ȥ�� no �� �ƴҶ� �� �Է�
		}

		printf("Go again ? < yes / no > : "); // �ݺ��� ���θ� ����
		while (1)
		{
			scanf("%s", yon);
			check(yon);
			if (strcmp(yon, "no") == 0) // �ٽ� �ϰ� ���� �ʴٸ� �ݺ����� �����
				return 0;
			else if (strcmp(yon, "yes") == 0) // �ٽ� �ϰ� �ʹٸ�
				break; // �ݺ��� ���θ� ���� �ݺ����� ��������
			else
				printf("Enter again. yes or no : "); // �� �� ���� �Է½� ���Է�
		}
	}
}

void check(char* eng)
{
	int i;
	for (i = 0; i<strlen(eng); i++) 
	{
		if (eng[i] >= 65 && eng[i] <= 90)
			eng[i] += 32; // ���ڿ��� �빮�ڴ� �ҹ��ڷ� ��ȯ
	}
}

char** init(int* max_r, int* max_c)
{
	int x = 0, y = 0, i; // x : ������ �� �� ���� ����� ����, y : ������ ������ �����Ͱ� ����� ����� ����, i : �ݺ����� ������ ���� ����
	char** set = NULL; // ���Ͽ��� �о�� �����Ͱ� ����� �迭 ������
	char read[100]; // ���Ͽ��� ���پ� �о ������ �迭
	char name[20]; // ������ �̸��� ������ �迭
	FILE* stream = NULL;
	while (1) { // �Է��� �ش� ������ �����Ҷ� ���� �Է��� �޴� �ݺ���
		printf("Enter the name of data file : ");
		scanf("%s", name);
		if (stream = fopen(name, "r") == 0) // ������ �������� �ʴ´ٸ�
			printf("error! please retry\n"); // ���� ���Է�
		else
			break;
	}
	stream = fopen(name, "r"); // ����ڰ� ���ϴ� ������ �о��
	while (1) // �ش� ������ ó������ ������ ����� ����ϴ� �ݺ���
	{
		if (fgets(read, 100, stream) == 0)
			break;
		x++; // ���� ���� �о���� ���� x �� ����
	}
	y = strlen(read); // ������ �����Ͱ� �� ����� y�� ����
	rewind(stream); // ������ �����͸� �� ������ �ٽ� �ű�
	set = (char**)malloc(x); // ������ �� �� ��ŭ 2���� �����Ϳ� �� ���� ���� �Ҵ�
	for (i = 0; i < x; i++) // ������ ������ �� ��ŭ 
		set[i] = (char*)malloc(y); // ������ �࿡ �� ����

	for (i = 0; i < x; i++) // ������ �� ó������ ������ �о�� �ݺ���
	{
		fgets(read, 100, stream);
		strcpy(set[i], read); // �������� ���� ������ �迭�� �Է�
		set[i][y] = '\0'; // ������ �� �� ������ ���� ���� ���� �Է�
	}
	fclose(stream); // ���� ����
	*max_r = x; // ���� �� �������� �ִ� �� �� ����
	*max_c = y; // ���� �� �������� �ִ� �� �� ����
	return set; // ������� 2���� �迭 ��ȯ.
}

void show(char** set, int max_r, int max_c)
{
	int i, j;
	printf("\n");
	for (i = 0; i < max_r; i++) // �迭�� ������ ����� �ݺ���
	{
		printf("		");
		for (j = 0; j <= max_c; j++)
			printf(" %c ", set[i][j]); // �迭�� �ش� ��Ŀ� �ش��ϴ� ������ ���
		printf("\n");
	}
	printf("\n");
}

void fill(int x, int y, int max_x, int max_y, char** set, char* yon)
{
	int index = -1; // ������ �ε����� ����ų ����
	order* stk; // ��ǥ���� ����� ����
	stk= (order*)malloc(sizeof(int)*(max_y*max_x));
	if (set[x][y] != '-') // �ش� ��ǥ�� ������ - �� �ƴҰ��
	{
		printf("\nthis is not blank \n\n");
		return;
	}
	while (1)
	{
		if (set[x][y] == '-') // �ش� ��ǥ�� ������ - �� ���� ���
		{
			set[x][y] = '@'; // - �� @�� �ٲ�
			index += 1; // �ε����� 0���� �ű�鼭
			stk[index].x = x; // ���ÿ� �ش� ��ǥ�� �Է�
			stk[index].y = y; 
			if (strcmp(yon, "yes") == 0)// ������ ����ϰ� �������
				show(set, max_x, max_y);  // @�� �ٲ𶧸��� ���
		}
		if (x>0 && set[x - 1][y] == '-') // �ش� ��ġ���� ������ '-'�� ���
			x--; // ��ǥ�̵�
		else 
			if (y>0 && set[x][y - 1] == '-') // �ش� ��ġ���� ������ '-'�� ���
				y--; // ��ǥ�̵�
			else
				if (y<max_y-1&&set[x][y + 1] == '-') // �ش� ��ġ���� �������� '-'�� ���
					y++; // ��ǥ�̵�
				else
					if (x<max_x-1&&set[x + 1][y] == '-') // �ش� ��ġ���� �Ʒ����� '-'�� ���
						x++; // ��ǥ�̵�
					else
						if (index == 0) // ����� ��ǥ�� �������
						{
							free(stk); 
							break; // �ݺ������� ��������
						}
						else // ����� ��ǥ�� �������
						{
							index--; // ���� ��ǥ�� �ε����� �ѱ��
							x = stk[index].x; // ������ ��ǥ�� ���� ��ǥ�� �Է�
							y = stk[index].y;
						}
	}
	if (strcmp(yon, "no") == 0) // ������� ����ϱ� ���Ѵٸ�
		show(set, max_x, max_y); // �������� �迭 ���
}