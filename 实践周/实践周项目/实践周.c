#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//3.���ݵ����
//3.1��ʲô�������� ---����---����
//3.2���ݵĽṹ------>ͼ����Ϣ
struct bookInfo
{
	char name[20];
	float price;
	int num;
};
struct Node
{
	struct bookInfo data;
	struct Node* next;
};

struct Node* list = NULL;
//������ͷ����ͷ����һ���ṹ�����
struct Node* createHead()
{
	//��̬�ڴ�����
	struct Node* headNode = (struct Node*)malloc(sizeof(struct Node));
	//��ʼ������
	headNode->next = NULL;
	return headNode;
}
//�����ڵ㣺Ϊ������׼��������->�ṹ�������
struct Node* createNode(struct bookInfo data)
{
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
//����:(��ͷ��)
void insertNodeByHead(struct Node* headNode, struct bookInfo data)
{
	struct Node* newNode = createNode(data);
	newNode->next = headNode->next;
	headNode->next = newNode;
}
//β�巨
//void insertNodeByTail(struct Node* headNode, int data)
//{
//	struct Node* pMove = headNode;
//	while (pMove->next != NULL)
//	{
//		pMove = pMove->next;
//	}
//	struct Node* newNode = createNode(data);
//	pMove->next = newNode;
//}
void deleteNodeByName(struct Node* headNode,char*bookName)
{
	struct Node* posLeftNode = headNode;
	struct Node* posNode = headNode->next;
	while (posNode != NULL && strcmp(posNode->data.name,bookName) )
	{
		posLeftNode = posNode;
		posNode = posLeftNode->next;		 
	}
	if (posNode == NULL)
		return;
	else
	{
		printf("ɾ���ɹ�\n");
		posLeftNode->next = posNode->next;
		free(posNode);
		posNode = NULL;
	}
}
struct Node* searchByName(struct Node* headNode, char* bookName)
{
	struct Node* posNode = headNode->next;
	while (posNode != NULL && strcmp(posNode->data.name, bookName) != 0) 
	{
		posNode = posNode->next;
	}
	return posNode;
}
void printList(struct Node* headNode)
{
	struct Node* pMove = headNode->next;
	printf("����\t�۸�\t����\n");
	while (pMove != NULL)
	{
		printf("%s\t%.1f\t%d\n", pMove->data.name, pMove->data.price, pMove->data.num);
		pMove = pMove->next;
	}
}
void menu()//1.����
{
	printf("            �򵥵�ͼ�����ϵͳ            \n");
	printf("**************************Menum***************************\n");
	printf("* 1 �Ǽ�ͼ��                         2 ���ͼ��      *\n");
	printf("* 3 ����ͼ��                         4 �黹ͼ��      *\n");
	printf("* 5 ͼ������                         6 ɾ��ͼ��      *\n");
	printf("* 7 ����ͼ��                         8 ����ͼ��      *\n");
	printf("* 9 ������Ϣ                         0 �˳�ϵͳ      *\n");
	printf("**********************************************************\n");
	printf("������(0~9):");
}
//�ļ�д����
void saveInfoToFile(const char* fileName, struct Node* headNode)
{
	FILE* fp = fopen(fileName, "w");
	struct Node* pMove = headNode->next;
	while (pMove != NULL)
	{
		fprintf(fp, "%s\t%.1f\t%d\n", pMove->data.name, pMove->data.price, pMove->data.num);
		pMove = pMove->next;
	}
	fclose(fp);
}
//�ļ�������
void readInfoFromFile(const char* fileName, struct Node* headNode)
{
	FILE* fp = fopen(fileName, "r");//��һ�δ��ļ�������
	if (fp == NULL)
	{
		fp = fopen(fileName, "w+");//�������򴴽�����
	}
	struct bookInfo tempData;
	while (fscanf_s(fp, "%19s\t%f\t%d\n", tempData.name, &tempData.price, &tempData.num) != EOF)
	{
		insertNodeByHead(list, tempData);
	}
	fclose(fp);
}
void bubbleSortList(struct Node* headNode)
{
	for (struct Node* p = headNode->next; p != NULL; p = p->next)
	{
		for (struct Node* q = headNode->next; q->next != NULL; q = q->next)
		{
			if (q->data.price > q->next->data.price)
			{
				struct bookInfo tempData = q->data;
				q->data = q->next->data;
				q->next->data = tempData;
			}
		}
	}
	printList(headNode);
}
struct Node* result = NULL;
void keydown()//2.�˵�ģ��
{
	int key = 0;
	struct bookInfo tempBook;//������ʱ�����洢�鼮��Ϣ
	scanf_s("%d",&key);
	switch (key)
	{
	case 1:
		printf("�Ǽ�ͼ��\n");
		printf("�������鼮��Ϣ(name,price,num):");
		scanf_s("%19s", tempBook.name, sizeof(tempBook.name)); // �������볤���Է�ֹ���
		scanf_s("%f", &tempBook.price);
		scanf_s("%d", &tempBook.num); 
		insertNodeByHead(list, tempBook);
		saveInfoToFile("boolinfo.txt", list);
		break;
	case 2:
		printf("���ͼ��\n");
		printf(list);
		break; 
	case 3:
		printf("����ͼ��\n");
		printf("����������鼮����:");
		scanf_s("%19s", tempBook.name, sizeof(tempBook.name)); // �������볤���Է�ֹ���
		result = searchByName(list, tempBook.name);
		if (result == NULL)
		{
			printf("û������鼮�޷�����\n");
		}
		else {
			if (result->data.num > 0) { // ����鼮��������0������Խ���
				result->data.num--; // �����鼮����
				printf("���ĳɹ�\n");
				saveInfoToFile("bookinfo.txt", list); // ������ĺ����Ϣ���ļ�
			}
			else {
				printf("��ǰ�鼮�޿�����ʧ��\n");
			}
		}
		break;		 
	case 4:
		printf("�黹ͼ��\n");//��ǰ�鼮����+1
		printf("������黹�鼮����:");
		scanf_s("%s", tempBook.name);
		result = searchByName(list, tempBook.name);
		if (result == NULL)
		{
			printf("������Դ�Ƿ�\n");
		}
		else
		{
			result->data.num++;
			printf("�鼮�黹�ɹ�\n");
		}
		break; 
	case 5:
		printf("ͼ������\n");
		bubbleSortList(list);
		break;
	case 6:
		printf("ɾ��ͼ��\n");
		printf("������ɾ������:");
		scanf_s("%s", tempBook.name);
		deleteNodeByName(list, tempBook.name);
		saveInfoToFile("boolinfo.txt", list);
		break;
	case 7:
		printf("����ͼ��\n");
		printf("������Ҫ���ҵ�����:");
		scanf_s("%s", tempBook.name);
		result = searchByName(list, tempBook.name);
		if (result == NULL)
		{
			printf("δ�ҵ������Ϣ\n");
		}
		else
		{
			printf("����\t�۸�\t����\n");
			printf("%s\t%.1f\t%d\n", result->data.name, result->data.price, result->data.num);
		}
		break;
	case 8:
		printf("����ͼ��\n");
		printf("����������鼮��Ϣ(name,price,num):");
		scanf_s("%19s", tempBook.name, sizeof(tempBook.name)); // �������볤���Է�ֹ���
		scanf_s("%f", &tempBook.price);
		scanf_s("%d", &tempBook.num);
		insertNodeByHead(list, tempBook);
		saveInfoToFile("boolinfo.txt", list);
		break;
	case 9:
		printf("������Ϣ\n");		 		 
		printf("�����뱣���鼮��Ϣ(name,price,num):");
		scanf_s("%19s", tempBook.name, sizeof(tempBook.name)); // �������볤���Է�ֹ���
		scanf_s("%f", &tempBook.price);
		scanf_s("%d", &tempBook.num);
		insertNodeByHead(list, tempBook);
		saveInfoToFile("boolinfo.txt", list);
		break;
	case 0:
		printf("�˳�ϵͳ\n");
		printf("�˳��ɹ�\n");
		system("pause");
		exit(0);//�ر���������
		break;
	default:
		printf("�������");
			break;
	}

}
int main()
{
	list = createHead();
	readInfoFromFile("bookinfo.txt", list);
	while (1)
	{
		menu();
		keydown();
		system("pause");
		system("cls");
	}
	system("pause");
	return 0;
}