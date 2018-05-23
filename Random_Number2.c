/*#include<stdio.h>
#include<stdlib.h>
#include<time.h>

void menu(); // �޴����� �ٸ� �Լ��� �Ѿ�� ������ ������ �Լ�
void menu2(); // �޴��� ����ϴ� �Լ�
void readData(int*, FILE*); // ������ �߻���Ű�� �Լ�
void searchNumber(int*, int*, FILE*); // �������� ������ ���ڸ� ã�� ���� �Լ�

int main()
{
	menu2();
	menu();
	return 0;
}

void menu2()
{
	puts("\n1. ���� ����");
	puts("2. �������� ���� ���� ã��");
	puts("3. ���α׷� ����");
	printf("\n���ϴ� �޴� ��ȣ�� �Է��ϼ��� : ");
}

void menu()
{
	float num;
	int  n; // n : ������ ������ �����ϴ� ���� , num : �޴� ��ȣ�� �Էµ� ����
	int* p = NULL; // ������ ����� �����޸� ������
	FILE* stream = NULL;

	while (1) // �޴� ���α׷��� �����ϱ� ������ ��� �ݺ��ǵ��� �ϴ� ���� ����
	{
		num = 0;
		scanf("%f", &num);
		getchar();
		if (num == 1.0) // 1���� �Է½� ������ �߻���Ű�� �Լ���
			readData(&n, stream);
		else if (num == 2.0)// 2���� �Է½� ������ ��ġ�� ã�� �Լ���
			searchNumber(&n, p, stream);
		else if (num == 3.0)// 3�� �Է� �� ���α׷� ����
			return;
		else        // �޴� �̿��� ��ȣ�� �Է��Ͽ��� �� ���� �߻� �޴� ���Է�
			printf("\n�߸� �Է� �Ͽ����ϴ�. �޴� ��ȣ�� �ٽ� �Է��� �ּ��� : \n");
	}
}


void readData(int* n, FILE* stream)
{
	char eng;
	int i;

	printf("\n������ ���� �Ͻðڽ��ϱ� <y/n> : ");
	scanf("%c", &eng);
	getchar();

	while (1)
	{
		if (eng == 'y' || eng == 'Y') // �� �ҹ��ڿ� ���� ���� ����
		{
			printf("\n��� ������ ���� �Ͻðڽ��ϱ� : ");
			if ((scanf("%d", n) == 0) || *n == 0) // ������ ���ڰ� �ƴ� ���ڸ� �ְų� 0�� �־����� ���� �߻�
			{
				printf("\n���� �Դϴ�. ������ ������ �ٽ� �Է��ϼ���\n");
				getchar();
			}
			else
			{
				getchar();
				srand(time(NULL)); // �߻� �ϴ� ������ ��� �ٸ��� �ϱ� ���� �õ� ����
				stream = fopen("data.dat", "w"); // ���� �̸� data.dat �� ���� ��� w
				for (i = 0; i < *n; i++) // ����ڰ� �Է��� ���� ���� n ����ŭ ���� �Է�
				{
					fprintf(stream, "%-5d ", rand() % 100); // stream�� n�� ��ŭ�� ���� ���� 
					if ((i + 1) % 10 == 0) // ���Ͽ� �ԷµǴ� ������ 10�� �� �ٶ�⸦ �ϱ� ���� ���ǹ�
						fprintf(stream, "\n");
				}
				printf("\n0�� 100������ ���� %d���� ���� �Ǿ����ϴ�.\n", *n);
				menu2(); // �۾��� ���������� �Ϸ� �Ǿ����� �ٽ� �޴� ȣ��
				fclose(stream);
				return;
			}
		}
		else if (eng == 'N' || eng == 'n') // �� �ҹ��ڿ� ���� ���� ����
		{
			printf("\n���ϴ� �޴� ��ȣ�� �ٽ� �Է��ϼ��� : "); // ���� ������ ������ ������� �޴��� ȣ������ ����
			return;
		}
		else
		{
			printf("\n���� �Դϴ�. �ٽ� �Է��� �ּ��� <y/n> : "); // �߸� �Է� �� ��� ���Է�
			scanf("%c", &eng);
			getchar();
		}
	}
}

void searchNumber(int* n, int* p, FILE* stream)
{
	int num, i, j; // num : ã���� �ϴ� ���� i,j : �������� ������ ���� ����
	int sml = 0, big, max, count; // sml : ������ �´� ���� ���� ��, count : ������ �´� ���� ���� ���� �ִ� ��ġ , big : ���� ��, max : ���� ū ����
	if (((stream = fopen("data.dat", "r")) == NULL) || (*n) <= 0) // ������ ���ų� ������ �������� �ʾ������� ����
	{
		printf("\n������ �������� �ʾҽ��ϴ�.\n");
		printf("\n���ϴ� �޴� ��ȣ�� �ٽ� �Է��Ͻÿ� : ");
		return;
	}
	else
	{
		printf("\n�� ��° ���������� ���ڸ� ã���ðڽ��ϱ� ? : ");
		while (1)
		{
			if (scanf("%d", &num) == 0 || num == 0) // ã������ ���ڿ� ���ڸ� �Է��ϰų� 0�� �Է��Ͽ����� ���� �߻�
			{
				getchar();
				printf("\n���� �Դϴ�. ã������ ���������� ���ڸ� �ٽ� �Է��ϼ��� : ");
			}
			else
			{
				p = (int*)malloc(sizeof(int)*(*n));
				for (i = 0, max = 0; i < *n; i++)
				{
					fscanf(stream, "%d", &p[i]); // ������ ����� ���Ͽ��� ���ʴ�� �迭�� ����
					if (max <= p[i])
						max = p[i]; // ������ �迭�� �����԰� ���ÿ� ���� �� ���� ū ������ max�� ����
				}
				for (i = 0; i < num; i++)
				{
					if (sml == max)
					{
						printf("\n%d ��°�� ���� ������ �������� �ʽ��ϴ�.\n", num);
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
				printf("\n%d��°�� ���� ���ڴ� %d�̸� %d��°�� ��ġ�� �ֽ��ϴ�. \n", num, sml, count + 1);
				fclose(stream);
				free(p);
				menu2();
				return;
			}
		}
	}
}*/