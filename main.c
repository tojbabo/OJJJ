#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Theinventory { // �۰� ����ü
	char name[50];
	struct Book* link;
}Theinventory;

typedef struct Book { // å ����ü
	char name[50];
	char stuff[10];
	char price[10];
	struct Book* link;
}Book;

void greeting() { printf("Welcome to the booksore program!\n\n"); } // ù�λ縦 �ϴ� �Լ���
void readDatabase(Theinventory*); // �����͸� �о�ͼ� �迭�� �����ϴ� �Լ�
char* printMenu(); // �޴��� ����ϴ� �Լ�
void displayinventory(Theinventory*); // ��� ����ϴ� �Լ�
void displayAuthorsWork(Theinventory*); // Ư�� ������ å ���� ���
void addBook(Theinventory*); // ���ο� ��� �Է��ϴ� �Լ�
void changePrice(Theinventory*); // Ư�� ������ Ư�� å�� ������ �ٲٴ� �Լ�
void changeQty(Theinventory*); // Ư�� ������ Ư�� å�� ����� �ٲٴ� �Լ�
void totalQty(Theinventory*); // ��� �ִ� ��� å�� ���� ���
void calculateTotalAmount(Theinventory*); // ��� �ִ� ��� å�� ������ �� ���

Theinventory input(char* eng,int* num); // ���� �����͸� ����üȭ �ϴ� �Լ�
void merge_sort_Author(Theinventory*,int,int); // ���ڸ� �̸������� �����ϴ� �Լ�
void merge(Theinventory*, int, int, int); // �̸������� �����ϴ� �Լ�
char* strConfirm(); // �Է��� ���ڿ��� �ٵ�� �Լ�
void bookview(Book*); // å�� ������ ����ϴ� �Լ�
int check(char*,int); // �Է� ���ڿ��� �˻��ϴ� �Լ�

int main(void) {
	int flag;
	char choice[10];

	greeting(); // ù�λ縦 �ϴ� �Լ�	

	Theinventory theinventory[20] = {NULL}; // �۰��� ����ü�迭

	readDatabase(theinventory); // ���Ͽ������� �о�� �Լ�

	flag = 1; 

	while (flag) { 
		strcpy(choice, printMenu()); // choice�� ���ڿ��� �޾ƿ� �Լ�

		if (strcmp(choice, "1") == 0)  // 1 �Է�
			displayinventory(theinventory); //1�� �޴� : �迭 �� å ���� ���

		else if (strcmp(choice, "2") == 0)
			displayAuthorsWork(theinventory); //2�� �޴� : Ư�� ���� å ���� ���

		else if (strcmp(choice, "3") == 0)
			addBook(theinventory); // 3�� �޴� : ���� �� å �߰� �Է�

		else if (strcmp(choice, "4") == 0)
			changePrice(theinventory); // 4�� �޴� : Ư�� å ���� ����

		else if (strcmp(choice, "5") == 0)
			changeQty(theinventory); // 5�� �޴� : Ư�� å ��� ����

		else if (strcmp(choice, "6") == 0)
			totalQty(theinventory); // 6�� �޴� : ��� �ִ� ��� å�� ���� ���

		else if (strcmp(choice, "7") == 0)
			calculateTotalAmount(theinventory); // 7�� �޴� : ��� �ִ� ��� å�� ������ �� ���

		else if (strcmp(choice, "8") == 0) { //8�� �޴� : ����
			printf("Thank you for using this program\n");
			flag = 0;
		}
		else // �߸� �Է����� ���
			printf("Invalid choice\n");
	}
	return 0;
}

void readDatabase(Theinventory* theinventory) {
	int i = 0, j = 0, k, num = 0; // i, j : �������� ������ ���� ����, num : ���Ͽ��� �о�� �� ��
	char eng[50] = { NULL }; // ȣ���� ������ �̸��� ������ ����
	Book* ptr=NULL; // �۰��� ������ å�� ����Ű�� �� ������
	Theinventory new; // �迭�� �����ϱ� �� �ӽ� ����ü
	FILE* stream=NULL;

	while (1) {
		printf("Enter the name of the file : ");
		scanf("%s", eng);
		if (stream = fopen(eng, "r") == 0) { // ������ �������� ������ ����
			printf("Error reading database\n\n");
			fflush(stdin);
		}
		else {
			new = input(eng,&num); // �ӽ� ����ü�� ���� �о��
			while(1){
				for (j = 0,k=0; j < i; j++) { // ���� ����� �迭��ŭ�� ���ư��� ������
					if (strcmp(theinventory[j].name, new.name) == 0) { // �ӽ÷� ����� �۰��� �̸��� �迭�� ������ ���
						ptr = theinventory[j].link; // å�� �����Ͱ� ���� ù��° å�� ����Ų��
						while (ptr->link != NULL) // ����Ų å�� ������ å�� �ƴҰ��
							ptr = ptr->link; // �������� ����ų������ ������ �̵�
						ptr->link = new.link; // ������ å�� ���� å�� ����Ű�� ����
						k = 1; // �ӽ÷� ����� ����ü�� �迭�� �־����� �ǹ�
					}
				}
				if (k==0) { // �� ����ü�� �迭�� �����ٸ�
					theinventory[i] = new; // �迭�� �������ϰ�
					i++; // ���� �迭�� �̵�
				}
				new = input(eng,&num); // �� ����ü�� �޾ƿ�
				if (new.link == NULL) // �� �۰� ����ü�� �ƹ��� å�� ���ٸ�
					break; // �ݺ��� ����
			}
			break;
		}
	}
}
char* printMenu() {
	char choice[10]; // choice�� ���� ����
	printf("\n-------------------------------\n");
	printf("Enter 1 to display the inventory\n");
	printf("Enter 2 to display the books by one author\n");
	printf("Enter 3 to add a book\n");
	printf("Enter 4 to change the price\n");
	printf("Enter 5 to change the qty on hand\n");
	printf("Enter 6 to view the total nmber of books in the inventory\n");
	printf("Enter 7 to see the total amount of the entire inventory\n");
	printf("Enter 8 to exit\n");
	printf("Enter your choice : ");
	scanf("%s", choice);
	return choice;
}
void displayinventory(Theinventory* inventory) {
	int i;
	Theinventory sort_inventory[20] = { NULL }; // ���ĵ� �迭�� ����� ����

	for (i = 0; i < 20 && inventory[i].link != NULL; i++) // �迭�� ó������ ���ڰ� �ִ� �κб�����
		sort_inventory[i] = inventory[i]; // ���ĵ� �迭�� �������� �迭 �Է�
	merge_sort_Author(sort_inventory, 0, i - 1); // ����
	i = 0;
	while (1) {
		if (inventory[i].link!= NULL) { // �۰��� å�� ���� ������ -> ����� �۰��� ���̻� ���� ������
			printf("\nThe author is :   %s\n", sort_inventory[i].name); //�۰��� �̸� ���
			bookview(sort_inventory[i].link); // å�� ����ϴ� �Լ� ȣ��
			i++;//���� �迭�� �̵�
		}
		else
			break;
	}
}
void displayAuthorsWork(Theinventory* inventory) {
	int i;
	char eng[50];
	char name[50];

	getchar();
	printf("\nEnter the author's first name : ");
	strcpy(eng, strConfirm()); // �۰��� first name ����
	printf("Enter the author's last name : ");
	strcpy(name, strConfirm()); // �۰��� last name �Է�
	strcat(name, " "); // �۰��� last name�� first name ������ ���� �߰�
	strcat(name, eng); // �۰��� �̸� ���̱�

	for (i = 0; inventory[i].link != NULL; i++) { // �迭���� �ش� ���ڰ� �ִ��� Ȯ��
		if (strcmp(inventory[i].name, name) == 0) { // ã������ ���ڰ� �����ϴ� ���
			bookview(inventory[i].link); // bookview �Լ��� �ش� ���� å ���
			return; // ����� �Լ� ����
		}
	}													//�ݺ������� �Լ��� ������� �ʾ��� ���
	printf("Sorry, but no books by %s in the inventory\n", name); // �迭�� �ش� ���ڰ� ���� �������� ���� ���
}
void addBook(Theinventory* inventory) {

	int i,j;
	char name[50];
	Theinventory new; // ���� �߰��� �۰�
	Book* pointer;  // ���ο� å�� �߰��� ��ġ
	Book* node; // ���� �߰��� å

	node = (Book*)malloc(sizeof(Book));
	new.link = node; // ���ο� �۰��� å�� ���ο� å���� ����
	node->link = NULL; // ���ο� å�� ��ũ�� �ΰ����� �ʱ�ȭ

	getchar();
	printf("\nEnter the author's first name : ");
	strcpy(name, strConfirm()); // �۰��� first name ����
	printf("Enter the author's last name : ");
	strcpy(new.name, strConfirm()); // �۰��� last name �Է�
	strcat(new.name, " "); // �۰��� last name�� first name ������ ���� �߰�
	strcat(new.name, name); // �۰��� �̸� ���̱�

	printf("Enter the title : ");
	strcpy(node->name, strConfirm()); // node�� ���ο� å Ÿ��Ʋ ����

	for (i = 0; inventory[i].link != NULL; i++) { // �۰� �迭�� ���ο� �۰��� ��ġ�ϴ��� �˻��ϴ� �ݺ���
		if (strcmp(inventory[i].name, new.name) == 0) // ���ο� �۰��� �̸��� ������ �迭�� ������ ���
		{
			pointer = inventory[i].link; // pointer�� �ش��۰��� å���� �Ѿ
			while (1) // pointer�� ������ å�� ����Ű�� �ϴ� �ݺ���
			{
				if (strcmp(pointer->name, node->name) == 0) // ���ο� å�� ������ �����Ҷ� �������
				{
					printf("This book is already in the Inventory and cannot be added again\n");
					return; // �Լ� ����
				}
				if (pointer->link == NULL) // pointer�� ������ å�� ��� ����
					break;
				else						//������ å�� �ƴҰ��
					pointer = pointer->link; // ���� å���� pointer �̵�
			}
			pointer->link = node; // pointer�� �� ������ å�� ����ų�� ���ο� å�� �״��� å���� ����
			i = 0;
			break; // �ݺ��� ����
		}
	}
	if (inventory[i].link == NULL) { // ���ο� �۰��� �迭�� �������� �������, �ݺ����� ������ �۰����� ���������
		inventory[i] = new;  // ������ �۰� �ڿ� ���ο� �۰� �߰�
		inventory[i + 1].link = NULL;
	}
	while (1) {
		printf("Enter the qtr : ");
		gets(name); // å�� ��� �� ����
		if (check(name,1)) { // �Է¿� ������ ������
			strcpy(node->stuff, name); // ������ �Է��� ����
			break;
		}
		else // ������ �ִٸ�
			printf("Invalid input for qty.!!\n"); // ���� ����
	}

	while (1){
		printf("Enter the price : ");
		gets(name); // å�� �� ����
		if (check(name,2)) { // �Է¿� ������ ������
			strcpy(node->price, name); // ������ ����
			break;
		}
		else // ������ �ִٸ�
			printf("Invalid input for price.\n"); // ���� ����
	}
}
void changePrice(Theinventory* inventory) {
	int i;
	char name[50];
	char bname[50];
	char price[50];
	Book* ptr; // å�� ����ų ������

	getchar();
	printf("\nEnter the author's first name : ");
	strcpy(bname, strConfirm()); // �۰��� first name ����
	printf("Enter the author's last name : ");
	strcpy(name, strConfirm()); // �۰��� last name �Է�
	strcat(name, " "); // �۰��� last name�� first name ������ ���� �߰�
	strcat(name, bname); // �۰��� �̸� ���̱�

	for (i = 0; inventory[i].link != NULL; i++)	{ // �Է��� ������ �̸��� ã�� �ݺ���
		if (strcmp(inventory[i].name, name) == 0) { // ����ü�� �ش� ���ڰ� �����Ұ��
			ptr = inventory[i].link; // �����͸� �ش� ������ å���� ����Ŵ
			i = -1;
			break;
			}
		}	
	if (i == -1) { // ���� �ݺ������� ���ڸ� ã���� ���
		printf("Enter the title : ");
		strcpy(bname, strConfirm()); // bname�� ã�� å �Է�
		while (ptr != NULL) { // ������ å�� �ش� å�� �����ϴ��� ã�� �ݺ���
			if (strcmp(ptr->name, bname) == 0) { // ������ å�� ��ġ�ϴ°� ������
				while (1) {
					printf("Enter the price : ");
					gets(price); // ������ price �Է�
					if (check(price, 2)) { // �Է��� ���� ������ �������
						printf("Price will be updated from %s to %s\n", ptr->price, price);
						strcpy(ptr->price, price); // �ش� å�� ���� ����
						return;
					}
					else // price �Է¿� ������ �־��� ���
						printf("Invalid input for the new price.\n"); //���� ����
				}
			}
			ptr = ptr->link;
		}
		if (ptr == NULL) // ������ å�� �ش� å�� �������� ������ 
		{
			printf("No book with the title %s by %s. \n", bname, name); // �������� ���
			return;
		}
	}
	else {
		printf("No suh author in you database. So you cannot change the price. \n");
		return;
	}

}
void changeQty(Theinventory* inventory) {
	int i;
	char name[50];
	char bname[50];
	char qty[50];
	Book* ptr; // å�� ����ų ������

	getchar();
	printf("\nEnter the author's first name : ");
	strcpy(bname, strConfirm()); // �۰��� first name ����
	printf("Enter the author's last name : ");
	strcpy(name, strConfirm()); // �۰��� last name �Է�
	strcat(name, " "); // �۰��� last name�� first name ������ ���� �߰�
	strcat(name, bname); // �۰��� �̸� ���̱�

	for (i = 0; inventory[i].link != NULL; i++) { // �Է��� ������ �̸��� ã�� �ݺ���
		if (strcmp(inventory[i].name, name) == 0) { // ����ü�� �ش� ���ڰ� �����Ұ��
			ptr = inventory[i].link; // �����͸� �ش� ������ å���� ����Ŵ
			i = -1;
			break;
		}
	}
	if (i == -1) { // ���� �ݺ������� ���ڸ� ã���� ���
		printf("Enter the title : ");
		strcpy(bname, strConfirm()); // bname�� ã�� å �Է�
		while (ptr != NULL) { // ������ å�� �ش� å�� �����ϴ��� ã�� �ݺ���
			if (strcmp(ptr->name, bname) == 0) { // ������ å�� ��ġ�ϴ°� ������
				while (1) {
					printf("Enter the qty : ");
					gets(qty); // ������ qty �Է�
					if (check(qty,1)) { // �Է��� ���� ������ �������
						printf("qty will be updated from %s to %s\n", ptr->stuff, qty);
						strcpy(ptr->stuff, qty); // �ش� å�� ��� ����
						return;
					}
					else // qty �Է¿� ������ �־��� ���
						printf("Invalid input for the new qty.\n"); //���� ����
				}
			}
			ptr = ptr->link;
		}
		if (ptr == NULL) // ������ å�� �ش� å�� �������� ������ 
		{
			printf("No book with the title %s by %s. \n", bname, name); // �������� ���
			return;
		}
	}
	else {
		printf("No suh author in you database. So you cannot change the qty. \n");
		return;
	}

}
void totalQty(Theinventory* inventory) {
	int i, total;
	Book* ptr;

	for (i = 0, total = 0; inventory[i].link != NULL; i++) // �迭 �� ��� ���ڸ� üũ�ϴ� �ݺ���
		for (ptr = inventory[i].link; ptr != NULL; ptr = ptr->link) // ������ ó�� å���� ������ å����
			total += atoi(ptr->stuff); // �� å�� ����� total�� �ջ�
	printf("The total number of books is %d\n", total); // total�� ���
}
void calculateTotalAmount(Theinventory* inventory) {
	int i;
	double total;
	Book* ptr;

	for (i = 0, total = 0; inventory[i].link != NULL; i++) // �迭�� ��� üũ�ϴ� �ݺ���
		for (ptr = inventory[i].link; ptr != NULL; ptr = ptr->link) // ó�� å���� ������ å����
			total += atof(ptr->price); // �� å�� ������ total�� �ջ�
	printf("The total value of the inventory is $%.2lf\n", total); // total �� ���
}

Theinventory input(char* eng,int* num) {
	FILE* stream;
	int j;
	char read[100];
	stream = fopen(eng, "r"); // ���� ����
	Theinventory new; //�о�� ������ ������ �ӽ� ����ü
	Book* node = NULL; // ����Ʈ�� ����Ű�� ������
	node = (Book*)malloc(sizeof(Book));
	new.link = node;
	node->link = NULL;

	for (j = 0; j <= *num; j++) { // num��° ���� ������ ���� ����
		if (fgets(read, 100, stream) == 0) { // ���������� �о��� ���
			new.link = NULL; // �ӽ� ����ü�� ��ũ�� NULL ���� 
			return new; // �״�� ��ȯ
		}
	}
	for (j = 0; read[j] != '\0'; j++) { // ���� �о�� ���� �˻��ϴ� ����
		if (read[j] == ' ') // �о�� �� �� ����� ����ٷ� ��ü
			read[j] = '_';
		else if (read[j] == ',') // �о�� �ٿ� ��ǥ�� ����� ��ü
			read[j] = ' ';
	}
	sscanf(read, "%s %s %s %s", new.name, node->name, node->stuff, node->price); // �о�� ���� ������ ������ ����
	new.name[strlen(new.name)] = '\0'; // �������� ���Ṯ�ڸ� �Է�
	node->name[strlen(node->name)] = '\0';// �������� ���Ṯ�ڸ� �Է�
	node->price[strlen(node->price)] = '\0';// �������� ���Ṯ�ڸ� �Է�
	node->stuff[strlen(node->stuff)] = '\0';// �������� ���Ṯ�ڸ� �Է�

	for (j = 0; new.name[j]!='\0'; j++) { // �۰��� �̸��� �˻��ϴ� ����
		if (new.name[j] >= 65 && new.name[j] <= 90) // �۰��� �̸� �� ��� �빮�ڴ� �ҹ��ڷ� �ٲ�
			new.name[j] += 32;
		if (j == 0)
			new.name[j] -= 32; // �۰��� �̸� �Ǿ�, ���ڴ� �빮�ڷ�
		else if (new.name[j-1] == '_') { //�۰��� ��Ʈ ������ ������ �۽�Ʈ ������ ���۵ɋ�
			new.name[j-1] = ' '; //����ٷ� ��ü �Ǿ��� ���⸦ �ٽ� ����� ����
			new.name[j] -= 32; //�۽�Ʈ ���� ù���ڴ� �빮�ڷ�
		}
	}
	for (j = 0; node->name[j]!='\0'; j++) { //å�� �̸��� �˻��ϴ� ����
		if (node->name[j] == '_')//����ٷ� ��ü�Ǿ��� ���⸦
			node->name[j] = ' '; // �ٽ� �����

	}
	*num=*num+1; //���������� �� ���� �о����� ���� �� �� ����
	return new; // �״�� ���� �о�� ����ü ��ȯ
}
void merge_sort_Author(Theinventory* inventory,int left,int right) {
	int mid;

	if (left < right) { // �迭�� ���۰� ���� �ٸ��ٸ�
		mid = (left + right) / 2; //  �߰��� üũ
		merge_sort_Author(inventory, left, mid); // �߰��� �������� ���ʺκ� ���ȣ��
		merge_sort_Author(inventory, mid + 1, right); // �߰��� �������� ������ �κ� ���ȣ��
		merge(inventory, left, mid, right); // �迭�� �����ϴ� �Լ� ȣ��
	}
} 
void merge(Theinventory* inventory, int left, int mid, int right) {
	Theinventory sort_inventory[20]; // ���ĵ� �迭
	int s_left = left;
	int s_mid = mid+1;
	int index = left;

	while (s_left<=mid&&s_mid<=right) { // �� �迭�� ������ �ʾ��� ���� �ݺ�
		if (strcmp(inventory[s_left].name, inventory[s_mid].name)==-1) // ���� �迭�� ���� ������ �迭�� ������ ������
			sort_inventory[index++] = inventory[s_left++]; // �迭�� ���� �� �Է�
		else if (strcmp(inventory[s_left].name, inventory[s_mid].name)==1) // ������ �迭�� ���� �� ������
			sort_inventory[index++] = inventory[s_mid++]; // �迭�� ������ �� �Է�
	}
	if(s_left>mid) // ���� �迭�� ���� ��������
		while (index <= right)  // ������ ������ ���� �迭�� �Է�
			sort_inventory[index++] = inventory[s_mid++];
	else  //������ �迭�� ���� ��������
		while (index <= right)  // ���� ������ ���� �迭�� �Է�
			sort_inventory[index++] = inventory[s_left++];

	for (index = left; index <= right; index++) // �ش� �迭�� �� �������� ��������
		inventory[index] = sort_inventory[index]; // ������ �迭�� �Է�
}
char* strConfirm()
{
	int i;
	char eng[50];

	gets(eng);
	for (i = 0; i < strlen(eng); i++) {
		if (eng[i] >= 65 && eng[i] <= 90 || eng[i] >= 97 && eng[i] <= 122) {
			if (eng[i] >= 65 && eng[i] <= 90) // �빮�ڴ� ���� �ҹ��ڷ� �ٲ�
				eng[i] += 32;
			if (i == 0) // ù��° ���ڴ� �빮�ڷ� �ٲ�
				eng[i] -= 32;
			else if (i != 0 && eng[i - 1] == ' ') // ���� ���� ���ڵ� �빮�ڷ� �ٲ�
				eng[i] -= 32;
		}
	}
	return eng;
}
void bookview(Book* ptr) {
	Book* move;
	Book memory;
	Book first;

	first.name[0] = 0; // ������ ������ ������ ���� ���� ���� ���� �ش�
	while(1) { // å�� ��� ���������� ����� �ݺ���
	memory.name[0] = 127; // ������ ������ ������ ���� ���� ū ���� �ش�
		for (move = ptr; move != NULL; move = move->link) { // ù å���� ������ å���� Ȯ���ϴ� ������
			if (strcmp(move->name, memory.name) == -1 && strcmp(move->name, first.name) == 1) // ���� å�� ���� å���� ������, ������ ���� ���� å���� ������ 
				memory = *move; // ���� å�� ���� å ����
		}

		if (memory.name[0] == 127) // �ݺ����� ���� memory ������ ������ �ʾ��� ���
			break;
		else { // �ݺ����� ���ؼ� memory ������ ������ ���
			first = memory; // �ռ� �ݺ������� ���� ���� å�� ����
			printf("\n	The title is :  %s\n", first.name);
			printf("	The qty is :  %s\n", first.stuff);
			printf("	The price is :  %s\n\n	----\n", first.price); // ���
		}
	}	
}
int check(char* eng,int num)
{
	int i,j;
	switch (num)
	{
	case 1: // ���� �Է��� �˻� �Ҷ�
		for (i = 0; i < strlen(eng); i++) {
			if (eng[i] < 48 || eng[i]>57) // ���ڰ� �ƴ� ���ڰ� �Էµɽ� 
				return 0;					// 0 ��ȯ
		}
		break;

	case 2: // �Ǽ� �Է��� �˻� �Ҷ�
		for (i = 0,j=0; i < strlen(eng); i++) { 
			if ((eng[i] < 48 || eng[i]>57) && eng[i] != 46) { // ���� Ȥ�� �Ҽ����� �ƴҽ�
				j = -1;											// ����
				break;
			}
			else if (eng[i] == 46 && (i == 0 || strlen(eng) - i != 3)) { // �Ҽ����� �� ���̰ų� �Ҽ��� �ڿ� ���ڰ� �ΰ��� �ƴҰ��
				j = -1;													// ����
				break;
			}
			if (eng[i] == 46) // �Ҽ����� �ϳ��϶����� ���� üũ
				j++;
		}
		if (j != 1) // ������ ����ų� �Ҽ����� �ϳ��� �ƴҰ��
			return 0; // 0 ��ȯ
		break;

	}
	return 1;

}