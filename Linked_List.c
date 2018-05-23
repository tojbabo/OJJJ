/*
다음은 단순연결리스트의 기능을 구현하는 프로그램이다.
구현할 기능은
1.	makeList – 복수개의 입력되는 데이터를 단순연결리스트로 만드는 기능
2.	viewList – 만들어진 단순연결리스트의 데이터를 순서대로 출력하는 기능
3.	findLast – 리스트의 맨 마지막 노드를 찾는 기능으로 그 포인터를 리턴함
4.	findPosition – 이 기능은 전달인자로 입력되는 데이터를 리스트에서 찾아 만약 리스트에 존재하면 1을 리턴하고 만약 데이터가 리스트에 존재하지 않으면 0을 리턴한다. 또한 리스트의 헤더 포인터외 찾는 데이터를 가르키는 포인터와 그 노드의 이전노드를 가르키는 포인터를 전달인자로 전달하여 만약 존재 한다며 그 포인터 값을 저장하여야 함.
5.	insertList – 새로운 데이터를 리스트에 삽입하는 기능으로 전달인자로 리스트 헤더 포인터, 어디 다음에 삽입해야 하는지 알려주는 포인터, 그리고 삽입할 데이터를 전달인자로 전달 받으며 이 기능에서 새로운 노드를 생성하여 삽입하여야 함. 또한, 리스트 생성, 즉 makeList에서 이 기능을 호출하여 리스트를 생성하여야 함.
6.	deleteList – 리스트에 원하는 데이터를 삭제하는 기능으로 전달인자로, 리스트 헤더 포인터, 그리고 삭제를 원하는 노드의 포인터 및 이전 포인터가 사용됨.

주의: insertList와 deleteList에서 원하는 노드의 포인터(삽입 혹은 삭제를 원하는 노드 및 그 이전 노드의 포인터)의 값을 구하기 위하여 findPosition기능 사용.
*/

#include<stdio.h>
#include<stdlib.h>

typedef struct node {
	int data;
	struct node* link;
}node;					//노드 타입 정의

void makeList(node**); // 리스트 생성함수.< 헤더 포인터의 포인터 >
void viewList(node*); // 리스트 출력함수
node* findLast(node*); //리스트의 마지막 노드를 찾는 함수< 헤더 포인터의 값 >
int findPosition(node*, node**, node**, int);
void insertList(node**, node*, int); // 리스트 삽입함수< 헤더포인터의 포인터, 타깃 포인터, 삽입 데이터 >
void deleteList(node**, node*, node*); //리스트 삭제함수 전달인자 < 헤더 포인터의 포인터, 이전포인터, 타깃포인터 >
//타깃 포인터는 findPosition에서 원하는 데이터의 위치이며 이전 포인터는 그 노드의 이전 노드의 포인터임

void main() {
	node *head = NULL; // 헤드 포인터
	node *pre = NULL; // 이전 포인터
	node *targetPtr = NULL; // 타겟 포인터
	int newData, target;
	int found;

	makeList(&head); // 노드 생성

	viewList(head); // 생성된 리스트 출력

	fflush(stdin); // 입력 버퍼를 지우는 작업

	printf("Enter data to insert : \n");
	while (1) { // 오류 발생시 다시실행시키는 루프문
		if (scanf("%d", &newData) != 0)
			break;
		else {
			printf("Error!! please retry!\n");
			getchar();
		}
	}


	printf("어디 다음에 삽입할까요 : <처음에 입력을 원하면 -1 입력>\n");
	while (1) { // 에러 발생시 반복시키는 루프
		if (scanf("%d", &target) != 0)
			break;
		else {
			printf("Error!! please retry!\n");
			getchar();
		}
	}

	if (target == -1) {
		targetPtr == NULL;  
		insertList(&head, targetPtr, newData);  
	}
	else {
		found = 0; 
		found = findPosition(head, &pre, &targetPtr, target);
		if (found == 1)
			insertList(&head, targetPtr, newData);
		else
			printf("There is no %d in the list\n", target);
	}

	viewList(head);

	//delete list
	printf("Enter data to delete from list\n");
	while (1) {//에러 발생시 반복시키는 루프
		if (scanf("%d", &target) != 0)
			break;
		else {
			printf("Error!! please retry!\n");
			getchar();
		}
	}

	printf("targeted data is  %d\n", target);

	found = 0;
	found = findPosition(head, &pre, &targetPtr, target);

	if (found == 1) {
		printf("found data is %d\n", targetPtr->data);
		deleteList(&head, pre, targetPtr);
	}
	else
		printf("There is no %d in the list\n", target);
	//view list data
	viewList(head);
}

node* findLast(node* hptr) {
	if (hptr != NULL) // 헤더포인터가 가리키는게 있을때
	{
		if (hptr->link == NULL) // 처음 데이터가 가리키는게 없다면
			return hptr; // 헤더 포인터가 가리키는 데이터 반환
		else
			return findLast(hptr->link); // 헤더포인터가 가르키는 데이터가 가리키는 데이터 반환
	}
	else 
		return NULL; // 헤더포인터가 가리키는게 없으면 NULL반환 
}

int findPosition(node *hptr, node **pre, node **targetPtr, int data) {
	*pre = NULL;
	*targetPtr = NULL;

	while (hptr != NULL&&hptr->data != data) { // 연결된 데이터들중 찾는 데이터 혹은 마지막 데이터까지 도는 루프문
		*pre = hptr; // 이전포인터는 헤더포인터가 되고
		hptr = hptr->link; // 헤더포인터는 다음 포인터를 가리킨다
	}
	if (hptr==NULL) // 루프문이 끝났을때 헤더포인터가 가리키는게 없을때
		return 0; // 0 반환
	else{ // 헤더포인터가 가르킨다 -> 찾으려는 데이터가 존재
		*targetPtr = hptr; // 현재 타겟포인터는 헤더포인터를 가리키고
		return 1; // 1 반환
	}
}

void insertList(node** hptr, node* pptr, int data) {
	node* new;

	new = (node*)malloc(sizeof(node)); // 새로운 노드 생성

	new->data = data;   //노드에 데이터 입력

	if (*hptr == NULL) { // 헤더가 가리키는게 없을때
		*hptr = new; // 헤더가 새로운 노드를 가리킴
		new->link = NULL;
	}
	else if (pptr == NULL) { // 이전포인터가 없을때
		new->link = *hptr; // 새로운 노드가 맨 앞으로 간다
		*hptr = new;
	}
	else {
		new->link = pptr->link; // 새로운 노드가 맨 뒤로간다
		pptr->link = new;
	}
}

void deleteList(node **hptr, node *pre, node *target) {
	if (pre == NULL)  // 맨 앞에 노드를 삭제시
		*hptr = target->link;  // 헤더포인터는 처음 노드가 가리키는 노드를 가리킨다
	else 
		pre->link = target->link; // 삭제하려는 노드 이전의 노드가 다음 노드를 가리킨다
	free(target); // 해당 노드를 삭제
}

void makeList(node** hptr) {
	node *pre = NULL;
	node *new = NULL;
	int data,num;
	
	printf("입력할 데이터는 < enter -1 to end >\n");
	while (1) { // 에러 발생시 다시 실행시키는 루프
		if (scanf("%d", &data) != 0) {
			getchar();
			break;
		}
		else {
			printf("Error!! please retry! \n");
			getchar();
		}
			
	}
	while (data != -1) { // 데이터에 -1을 입력할때까지
		insertList(hptr, pre, data);  // 노드 생성 및 입력
		pre = findLast(*hptr);  // 항상 마지막 노드를 가리켜 다음 노드가 맨 뒤에 생기게 한다

		printf("Enter data again\n");
		while (1) { // 에러 발생시 다시 실행시키는 루프
			if (scanf("%d", &data) != 0) {
				getchar();
				break;
			}
			else {
				printf("Error!! please retry! \n");
				getchar();
			}
		}
	}
}

void viewList(node *ptr) {
	while (ptr != NULL) {
		printf("%d ", ptr->data);
		ptr = ptr->link;
	}
	printf("\n");
}
