#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct Theinventory { // 작가 구조체
	char name[50];
	struct Book* link;
}Theinventory;

typedef struct Book { // 책 구조체
	char name[50];
	char stuff[10];
	char price[10];
	struct Book* link;
}Book;

void greeting() { printf("Welcome to the booksore program!\n\n"); } // 첫인사를 하는 함수ㅡ
void readDatabase(Theinventory*); // 데이터를 읽어와서 배열에 저장하는 함수
char* printMenu(); // 메뉴를 출력하는 함수
void displayinventory(Theinventory*); // 재고를 출력하는 함수
void displayAuthorsWork(Theinventory*); // 특정 저자의 책 정보 출력
void addBook(Theinventory*); // 새로운 재고를 입력하는 함수
void changePrice(Theinventory*); // 특정 저자의 특정 책의 가격을 바꾸는 함수
void changeQty(Theinventory*); // 특정 저자의 특정 책의 재고량을 바꾸는 함수
void totalQty(Theinventory*); // 재고에 있는 모든 책의 갯수 출력
void calculateTotalAmount(Theinventory*); // 재고에 있는 모든 책의 가격의 합 출력

Theinventory input(char* eng,int* num); // 읽은 데이터를 구조체화 하는 함수
void merge_sort_Author(Theinventory*,int,int); // 저자를 이름순으로 정렬하는 함수
void merge(Theinventory*, int, int, int); // 이름순으로 정렬하는 함수
char* strConfirm(); // 입력한 문자열을 다듬는 함수
void bookview(Book*); // 책의 정보를 출력하는 함수
int check(char*,int); // 입력 문자열을 검사하는 함수

int main(void) {
	int flag;
	char choice[10];

	greeting(); // 첫인사를 하는 함수	

	Theinventory theinventory[20] = {NULL}; // 작가의 구조체배열

	readDatabase(theinventory); // 파일에서부터 읽어올 함수

	flag = 1; 

	while (flag) { 
		strcpy(choice, printMenu()); // choice에 문자열을 받아올 함수

		if (strcmp(choice, "1") == 0)  // 1 입력
			displayinventory(theinventory); //1번 메뉴 : 배열 및 책 정보 출력

		else if (strcmp(choice, "2") == 0)
			displayAuthorsWork(theinventory); //2번 메뉴 : 특정 저자 책 정보 출력

		else if (strcmp(choice, "3") == 0)
			addBook(theinventory); // 3번 메뉴 : 저자 및 책 추가 입력

		else if (strcmp(choice, "4") == 0)
			changePrice(theinventory); // 4번 메뉴 : 특정 책 가격 변경

		else if (strcmp(choice, "5") == 0)
			changeQty(theinventory); // 5번 메뉴 : 특정 책 재고량 변경

		else if (strcmp(choice, "6") == 0)
			totalQty(theinventory); // 6번 메뉴 : 재고에 있는 모든 책의 갯수 출력

		else if (strcmp(choice, "7") == 0)
			calculateTotalAmount(theinventory); // 7번 메뉴 : 재고에 있는 모든 책의 가격의 합 출력

		else if (strcmp(choice, "8") == 0) { //8번 메뉴 : 종료
			printf("Thank you for using this program\n");
			flag = 0;
		}
		else // 잘못 입력했을 경우
			printf("Invalid choice\n");
	}
	return 0;
}

void readDatabase(Theinventory* theinventory) {
	int i = 0, j = 0, k, num = 0; // i, j : 루프문을 돌리기 위한 변수, num : 파일에서 읽어올 줄 수
	char eng[50] = { NULL }; // 호출할 파일의 이름을 저장할 변수
	Book* ptr=NULL; // 작가의 마지막 책을 가리키게 될 포인터
	Theinventory new; // 배열에 저장하기 전 임시 구조체
	FILE* stream=NULL;

	while (1) {
		printf("Enter the name of the file : ");
		scanf("%s", eng);
		if (stream = fopen(eng, "r") == 0) { // 파일이 존재하지 않으면 오류
			printf("Error reading database\n\n");
			fflush(stdin);
		}
		else {
			new = input(eng,&num); // 임시 구조체에 파일 읽어옴
			while(1){
				for (j = 0,k=0; j < i; j++) { // 현재 저장된 배열만큼만 돌아가는 루프문
					if (strcmp(theinventory[j].name, new.name) == 0) { // 임시로 저장된 작가의 이름이 배열에 존재할 경우
						ptr = theinventory[j].link; // 책의 포인터가 가장 첫번째 책을 가리킨다
						while (ptr->link != NULL) // 가리킨 책이 마지막 책이 아닐경우
							ptr = ptr->link; // 마지막을 가리킬때까지 포인터 이동
						ptr->link = new.link; // 마지막 책에 다음 책을 가리키게 저장
						k = 1; // 임시로 저장된 구조체가 배열에 있었음을 의미
					}
				}
				if (k==0) { // 새 구조체가 배열에 없었다면
					theinventory[i] = new; // 배열에 저장을하고
					i++; // 다음 배열로 이동
				}
				new = input(eng,&num); // 새 구조체를 받아옴
				if (new.link == NULL) // 새 작가 구조체가 아무런 책이 없다면
					break; // 반복문 종료
			}
			break;
		}
	}
}
char* printMenu() {
	char choice[10]; // choice를 받을 변수
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
	Theinventory sort_inventory[20] = { NULL }; // 정렬된 배열이 저장될 변수

	for (i = 0; i < 20 && inventory[i].link != NULL; i++) // 배열의 처음부터 저자가 있는 부분까지만
		sort_inventory[i] = inventory[i]; // 정렬될 배열에 기존이의 배열 입력
	merge_sort_Author(sort_inventory, 0, i - 1); // 정렬
	i = 0;
	while (1) {
		if (inventory[i].link!= NULL) { // 작가의 책이 없지 않을때 -> 출력할 작가가 더이상 없지 않을때
			printf("\nThe author is :   %s\n", sort_inventory[i].name); //작가의 이름 출력
			bookview(sort_inventory[i].link); // 책을 출력하는 함수 호출
			i++;//다음 배열로 이동
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
	strcpy(eng, strConfirm()); // 작가의 first name 저장
	printf("Enter the author's last name : ");
	strcpy(name, strConfirm()); // 작가의 last name 입력
	strcat(name, " "); // 작가의 last name과 first name 사이의 공백 추가
	strcat(name, eng); // 작가의 이름 붙이기

	for (i = 0; inventory[i].link != NULL; i++) { // 배열에서 해당 저자가 있는지 확인
		if (strcmp(inventory[i].name, name) == 0) { // 찾으려는 저자가 존재하는 경우
			bookview(inventory[i].link); // bookview 함수로 해당 저자 책 출력
			return; // 출력후 함수 종료
		}
	}													//반복문에서 함수가 종료되지 않았을 경우
	printf("Sorry, but no books by %s in the inventory\n", name); // 배열에 해당 저자가 없는 것임으로 문구 출력
}
void addBook(Theinventory* inventory) {

	int i,j;
	char name[50];
	Theinventory new; // 새로 추가될 작가
	Book* pointer;  // 새로운 책이 추가될 위치
	Book* node; // 새로 추가될 책

	node = (Book*)malloc(sizeof(Book));
	new.link = node; // 새로운 작가의 책을 새로운 책으로 지정
	node->link = NULL; // 새로운 책의 링크는 널값으로 초기화

	getchar();
	printf("\nEnter the author's first name : ");
	strcpy(name, strConfirm()); // 작가의 first name 저장
	printf("Enter the author's last name : ");
	strcpy(new.name, strConfirm()); // 작가의 last name 입력
	strcat(new.name, " "); // 작가의 last name과 first name 사이의 공백 추가
	strcat(new.name, name); // 작가의 이름 붙이기

	printf("Enter the title : ");
	strcpy(node->name, strConfirm()); // node에 새로운 책 타이틀 저장

	for (i = 0; inventory[i].link != NULL; i++) { // 작가 배열중 새로운 작가와 일치하는지 검사하는 반복문
		if (strcmp(inventory[i].name, new.name) == 0) // 새로운 작가의 이름이 기존의 배열에 존재할 경우
		{
			pointer = inventory[i].link; // pointer는 해당작가의 책으로 넘어감
			while (1) // pointer가 마지막 책을 가리키게 하는 반복문
			{
				if (strcmp(pointer->name, node->name) == 0) // 새로운 책이 기존에 존재할때 오류출력
				{
					printf("This book is already in the Inventory and cannot be added again\n");
					return; // 함수 종료
				}
				if (pointer->link == NULL) // pointer가 마지막 책일 경우 종료
					break;
				else						//마지막 책이 아닐경우
					pointer = pointer->link; // 다음 책으로 pointer 이동
			}
			pointer->link = node; // pointer가 맨 마지막 책을 가리킬때 새로운 책이 그다음 책으로 저장
			i = 0;
			break; // 반복문 종료
		}
	}
	if (inventory[i].link == NULL) { // 새로운 작가가 배열에 존재하지 않을경우, 반복문이 마지막 작가까지 돌았을경우
		inventory[i] = new;  // 마지막 작가 뒤에 새로운 작가 추가
		inventory[i + 1].link = NULL;
	}
	while (1) {
		printf("Enter the qtr : ");
		gets(name); // 책의 재고 수 저장
		if (check(name,1)) { // 입력에 문제가 없을때
			strcpy(node->stuff, name); // 변수에 입력을 저장
			break;
		}
		else // 문제가 있다면
			printf("Invalid input for qty.!!\n"); // 오류 문구
	}

	while (1){
		printf("Enter the price : ");
		gets(name); // 책의 값 저장
		if (check(name,2)) { // 입력에 문제가 없을때
			strcpy(node->price, name); // 변수에 저장
			break;
		}
		else // 문제가 있다면
			printf("Invalid input for price.\n"); // 오류 문구
	}
}
void changePrice(Theinventory* inventory) {
	int i;
	char name[50];
	char bname[50];
	char price[50];
	Book* ptr; // 책을 가리킬 포인터

	getchar();
	printf("\nEnter the author's first name : ");
	strcpy(bname, strConfirm()); // 작가의 first name 저장
	printf("Enter the author's last name : ");
	strcpy(name, strConfirm()); // 작가의 last name 입력
	strcat(name, " "); // 작가의 last name과 first name 사이의 공백 추가
	strcat(name, bname); // 작가의 이름 붙이기

	for (i = 0; inventory[i].link != NULL; i++)	{ // 입력한 저자의 이름을 찾는 반복문
		if (strcmp(inventory[i].name, name) == 0) { // 구조체에 해당 저자가 존재할경우
			ptr = inventory[i].link; // 포인터를 해당 저자의 책으로 가리킴
			i = -1;
			break;
			}
		}	
	if (i == -1) { // 위의 반복문에서 저자를 찾았을 경우
		printf("Enter the title : ");
		strcpy(bname, strConfirm()); // bname에 찾을 책 입력
		while (ptr != NULL) { // 저자의 책중 해당 책이 존재하는지 찾을 반복문
			if (strcmp(ptr->name, bname) == 0) { // 저자의 책중 일치하는게 있을때
				while (1) {
					printf("Enter the price : ");
					gets(price); // 변경할 price 입력
					if (check(price, 2)) { // 입력한 값에 오류가 없을경우
						printf("Price will be updated from %s to %s\n", ptr->price, price);
						strcpy(ptr->price, price); // 해당 책의 가격 변경
						return;
					}
					else // price 입력에 오류가 있었을 경우
						printf("Invalid input for the new price.\n"); //오류 문구
				}
			}
			ptr = ptr->link;
		}
		if (ptr == NULL) // 저자의 책중 해당 책이 존재하지 않을때 
		{
			printf("No book with the title %s by %s. \n", bname, name); // 오류문구 출력
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
	Book* ptr; // 책을 가리킬 포인터

	getchar();
	printf("\nEnter the author's first name : ");
	strcpy(bname, strConfirm()); // 작가의 first name 저장
	printf("Enter the author's last name : ");
	strcpy(name, strConfirm()); // 작가의 last name 입력
	strcat(name, " "); // 작가의 last name과 first name 사이의 공백 추가
	strcat(name, bname); // 작가의 이름 붙이기

	for (i = 0; inventory[i].link != NULL; i++) { // 입력한 저자의 이름을 찾는 반복문
		if (strcmp(inventory[i].name, name) == 0) { // 구조체에 해당 저자가 존재할경우
			ptr = inventory[i].link; // 포인터를 해당 저자의 책으로 가리킴
			i = -1;
			break;
		}
	}
	if (i == -1) { // 위의 반복문에서 저자를 찾았을 경우
		printf("Enter the title : ");
		strcpy(bname, strConfirm()); // bname에 찾을 책 입력
		while (ptr != NULL) { // 저자의 책중 해당 책이 존재하는지 찾을 반복문
			if (strcmp(ptr->name, bname) == 0) { // 저자의 책중 일치하는게 있을때
				while (1) {
					printf("Enter the qty : ");
					gets(qty); // 변경할 qty 입력
					if (check(qty,1)) { // 입력한 값에 오류가 없을경우
						printf("qty will be updated from %s to %s\n", ptr->stuff, qty);
						strcpy(ptr->stuff, qty); // 해당 책의 재고량 변경
						return;
					}
					else // qty 입력에 오류가 있었을 경우
						printf("Invalid input for the new qty.\n"); //오류 문구
				}
			}
			ptr = ptr->link;
		}
		if (ptr == NULL) // 저자의 책중 해당 책이 존재하지 않을때 
		{
			printf("No book with the title %s by %s. \n", bname, name); // 오류문구 출력
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

	for (i = 0, total = 0; inventory[i].link != NULL; i++) // 배열 속 모든 저자를 체크하는 반복문
		for (ptr = inventory[i].link; ptr != NULL; ptr = ptr->link) // 저자의 처음 책부터 마지막 책까지
			total += atoi(ptr->stuff); // 각 책의 재고량을 total에 합산
	printf("The total number of books is %d\n", total); // total값 출력
}
void calculateTotalAmount(Theinventory* inventory) {
	int i;
	double total;
	Book* ptr;

	for (i = 0, total = 0; inventory[i].link != NULL; i++) // 배열을 모두 체크하는 반복문
		for (ptr = inventory[i].link; ptr != NULL; ptr = ptr->link) // 처음 책부터 마지막 책까지
			total += atof(ptr->price); // 각 책의 가격을 total에 합산
	printf("The total value of the inventory is $%.2lf\n", total); // total 값 출력
}

Theinventory input(char* eng,int* num) {
	FILE* stream;
	int j;
	char read[100];
	stream = fopen(eng, "r"); // 파일 오픈
	Theinventory new; //읽어온 정보를 저장할 임시 구조체
	Book* node = NULL; // 리스트를 가리키는 포인터
	node = (Book*)malloc(sizeof(Book));
	new.link = node;
	node->link = NULL;

	for (j = 0; j <= *num; j++) { // num번째 줄의 정보를 읽을 변수
		if (fgets(read, 100, stream) == 0) { // 마지막줄을 읽었을 경우
			new.link = NULL; // 임시 구조체의 링크에 NULL 저장 
			return new; // 그대로 반환
		}
	}
	for (j = 0; read[j] != '\0'; j++) { // 현재 읽어온 줄을 검사하는 루프
		if (read[j] == ' ') // 읽어온 줄 중 띄어쓰기는 언더바로 대체
			read[j] = '_';
		else if (read[j] == ',') // 읽어온 줄에 쉼표는 띄어쓰기로 대체
			read[j] = ' ';
	}
	sscanf(read, "%s %s %s %s", new.name, node->name, node->stuff, node->price); // 읽어온 줄을 각각의 변수에 저장
	new.name[strlen(new.name)] = '\0'; // 마지막에 종료문자를 입력
	node->name[strlen(node->name)] = '\0';// 마지막에 종료문자를 입력
	node->price[strlen(node->price)] = '\0';// 마지막에 종료문자를 입력
	node->stuff[strlen(node->stuff)] = '\0';// 마지막에 종료문자를 입력

	for (j = 0; new.name[j]!='\0'; j++) { // 작가의 이름을 검사하는 루프
		if (new.name[j] >= 65 && new.name[j] <= 90) // 작가의 이름 중 모든 대문자는 소문자로 바꿈
			new.name[j] += 32;
		if (j == 0)
			new.name[j] -= 32; // 작가의 이름 맨앞, 글자는 대문자로
		else if (new.name[j-1] == '_') { //작가의 라스트 네임이 끝나고 퍼스트 네임이 시작될떄
			new.name[j-1] = ' '; //언더바로 대체 되었던 띄어쓰기를 다시 띄어쓰기로 변경
			new.name[j] -= 32; //퍼스트 네임 첫글자는 대문자로
		}
	}
	for (j = 0; node->name[j]!='\0'; j++) { //책의 이름을 검사하는 루프
		if (node->name[j] == '_')//언더바로 대체되었던 띄어쓰기를
			node->name[j] = ' '; // 다시 띄어쓰기로

	}
	*num=*num+1; //성공적으로 한 줄을 읽었을때 읽은 줄 수 저장
	return new; // 그대로 새로 읽어온 구조체 반환
}
void merge_sort_Author(Theinventory* inventory,int left,int right) {
	int mid;

	if (left < right) { // 배열의 시작과 끝이 다르다면
		mid = (left + right) / 2; //  중간을 체크
		merge_sort_Author(inventory, left, mid); // 중간을 기점으로 왼쪽부분 재귀호출
		merge_sort_Author(inventory, mid + 1, right); // 중간을 기점으로 오른쪽 부분 재귀호출
		merge(inventory, left, mid, right); // 배열을 정렬하는 함수 호출
	}
} 
void merge(Theinventory* inventory, int left, int mid, int right) {
	Theinventory sort_inventory[20]; // 정렬될 배열
	int s_left = left;
	int s_mid = mid+1;
	int index = left;

	while (s_left<=mid&&s_mid<=right) { // 두 배열이 끝나지 않았을 동안 반복
		if (strcmp(inventory[s_left].name, inventory[s_mid].name)==-1) // 왼쪽 배열의 값이 오른쪽 배열의 값보다 작을때
			sort_inventory[index++] = inventory[s_left++]; // 배열에 왼쪽 값 입력
		else if (strcmp(inventory[s_left].name, inventory[s_mid].name)==1) // 오른쪽 배열의 값이 더 작을때
			sort_inventory[index++] = inventory[s_mid++]; // 배열에 오른쪽 값 입력
	}
	if(s_left>mid) // 왼쪽 배열이 먼저 끝났을때
		while (index <= right)  // 오른쪽 나머지 값을 배열에 입력
			sort_inventory[index++] = inventory[s_mid++];
	else  //오른쪽 배열이 먼저 끝났을때
		while (index <= right)  // 왼쪽 나머지 값을 배열에 입력
			sort_inventory[index++] = inventory[s_left++];

	for (index = left; index <= right; index++) // 해당 배열의 맨 좌측부터 우측까지
		inventory[index] = sort_inventory[index]; // 기존의 배열에 입력
}
char* strConfirm()
{
	int i;
	char eng[50];

	gets(eng);
	for (i = 0; i < strlen(eng); i++) {
		if (eng[i] >= 65 && eng[i] <= 90 || eng[i] >= 97 && eng[i] <= 122) {
			if (eng[i] >= 65 && eng[i] <= 90) // 대문자는 전부 소문자로 바꿈
				eng[i] += 32;
			if (i == 0) // 첫번째 글자는 대문자로 바꿈
				eng[i] -= 32;
			else if (i != 0 && eng[i - 1] == ' ') // 공백 뒤의 글자도 대문자로 바꿈
				eng[i] -= 32;
		}
	}
	return eng;
}
void bookview(Book* ptr) {
	Book* move;
	Book memory;
	Book first;

	first.name[0] = 0; // 기존의 변수가 가질수 없는 가장 작은 값을 준다
	while(1) { // 책의 재고를 연속적으로 출력할 반복문
	memory.name[0] = 127; // 기존의 변수가 가질수 없는 가장 큰 값을 준다
		for (move = ptr; move != NULL; move = move->link) { // 첫 책부터 마지막 책까지 확인하는 루프문
			if (strcmp(move->name, memory.name) == -1 && strcmp(move->name, first.name) == 1) // 현재 책이 기억된 책보다 빠르고, 이전의 가장 빠른 책보다 느릴때 
				memory = *move; // 기억된 책에 현재 책 저장
		}

		if (memory.name[0] == 127) // 반복문을 통해 memory 변수가 변하지 않았을 경우
			break;
		else { // 반복문을 통해서 memory 변수가 변했을 경우
			first = memory; // 앞선 반복문에서 가장 빠른 책이 저장
			printf("\n	The title is :  %s\n", first.name);
			printf("	The qty is :  %s\n", first.stuff);
			printf("	The price is :  %s\n\n	----\n", first.price); // 출력
		}
	}	
}
int check(char* eng,int num)
{
	int i,j;
	switch (num)
	{
	case 1: // 정수 입력을 검사 할때
		for (i = 0; i < strlen(eng); i++) {
			if (eng[i] < 48 || eng[i]>57) // 숫자가 아닌 문자가 입력될시 
				return 0;					// 0 반환
		}
		break;

	case 2: // 실수 입력을 검사 할때
		for (i = 0,j=0; i < strlen(eng); i++) { 
			if ((eng[i] < 48 || eng[i]>57) && eng[i] != 46) { // 숫자 혹은 소숫점이 아닐시
				j = -1;											// 오류
				break;
			}
			else if (eng[i] == 46 && (i == 0 || strlen(eng) - i != 3)) { // 소숫점이 맨 앞이거나 소숫점 뒤에 숫자가 두개가 아닐경우
				j = -1;													// 오류
				break;
			}
			if (eng[i] == 46) // 소숫점이 하나일때마다 객수 체크
				j++;
		}
		if (j != 1) // 오류가 생겼거나 소숫점이 하나가 아닐경우
			return 0; // 0 반환
		break;

	}
	return 1;

}