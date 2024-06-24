#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//3.数据的设计
//3.1用什么处理数据 ---容器---链表
//3.2数据的结构------>图书信息
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
//创建表头：表头就是一个结构体变量
struct Node* createHead()
{
	//动态内存申请
	struct Node* headNode = (struct Node*)malloc(sizeof(struct Node));
	//初始化变量
	headNode->next = NULL;
	return headNode;
}
//创建节点：为插入做准备（数据->结构体变量）
struct Node* createNode(struct bookInfo data)
{
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}
//插入:(表头法)
void insertNodeByHead(struct Node* headNode, struct bookInfo data)
{
	struct Node* newNode = createNode(data);
	newNode->next = headNode->next;
	headNode->next = newNode;
}
//尾插法
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
		printf("删除成功\n");
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
	printf("书名\t价格\t数量\n");
	while (pMove != NULL)
	{
		printf("%s\t%.1f\t%d\n", pMove->data.name, pMove->data.price, pMove->data.num);
		pMove = pMove->next;
	}
}
void menu()//1.界面
{
	printf("            简单的图书管理系统            \n");
	printf("**************************Menum***************************\n");
	printf("* 1 登记图书                         2 浏览图书      *\n");
	printf("* 3 借阅图书                         4 归还图书      *\n");
	printf("* 5 图书排序                         6 删除图书      *\n");
	printf("* 7 查找图书                         8 更新图书      *\n");
	printf("* 9 保存信息                         0 退出系统      *\n");
	printf("**********************************************************\n");
	printf("请输入(0~9):");
}
//文件写操作
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
//文件读操作
void readInfoFromFile(const char* fileName, struct Node* headNode)
{
	FILE* fp = fopen(fileName, "r");//第一次打开文件不存在
	if (fp == NULL)
	{
		fp = fopen(fileName, "w+");//不存在则创建出来
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
void keydown()//2.菜单模块
{
	int key = 0;
	struct bookInfo tempBook;//产生临时变量存储书籍信息
	scanf_s("%d",&key);
	switch (key)
	{
	case 1:
		printf("登记图书\n");
		printf("请输入书籍信息(name,price,num):");
		scanf_s("%19s", tempBook.name, sizeof(tempBook.name)); // 限制输入长度以防止溢出
		scanf_s("%f", &tempBook.price);
		scanf_s("%d", &tempBook.num); 
		insertNodeByHead(list, tempBook);
		saveInfoToFile("boolinfo.txt", list);
		break;
	case 2:
		printf("浏览图书\n");
		printf(list);
		break; 
	case 3:
		printf("借阅图书\n");
		printf("请输入借阅书籍名字:");
		scanf_s("%19s", tempBook.name, sizeof(tempBook.name)); // 限制输入长度以防止溢出
		result = searchByName(list, tempBook.name);
		if (result == NULL)
		{
			printf("没有相关书籍无法借阅\n");
		}
		else {
			if (result->data.num > 0) { // 如果书籍数量大于0，则可以借阅
				result->data.num--; // 减少书籍数量
				printf("借阅成功\n");
				saveInfoToFile("bookinfo.txt", list); // 保存借阅后的信息到文件
			}
			else {
				printf("当前书籍无库存借阅失败\n");
			}
		}
		break;		 
	case 4:
		printf("归还图书\n");//当前书籍数量+1
		printf("请输入归还书籍名字:");
		scanf_s("%s", tempBook.name);
		result = searchByName(list, tempBook.name);
		if (result == NULL)
		{
			printf("该书来源非法\n");
		}
		else
		{
			result->data.num++;
			printf("书籍归还成功\n");
		}
		break; 
	case 5:
		printf("图书排序\n");
		bubbleSortList(list);
		break;
	case 6:
		printf("删除图书\n");
		printf("请输入删除书名:");
		scanf_s("%s", tempBook.name);
		deleteNodeByName(list, tempBook.name);
		saveInfoToFile("boolinfo.txt", list);
		break;
	case 7:
		printf("查找图书\n");
		printf("请输入要查找的书名:");
		scanf_s("%s", tempBook.name);
		result = searchByName(list, tempBook.name);
		if (result == NULL)
		{
			printf("未找到相关信息\n");
		}
		else
		{
			printf("书名\t价格\t数量\n");
			printf("%s\t%.1f\t%d\n", result->data.name, result->data.price, result->data.num);
		}
		break;
	case 8:
		printf("更新图书\n");
		printf("请输入更新书籍信息(name,price,num):");
		scanf_s("%19s", tempBook.name, sizeof(tempBook.name)); // 限制输入长度以防止溢出
		scanf_s("%f", &tempBook.price);
		scanf_s("%d", &tempBook.num);
		insertNodeByHead(list, tempBook);
		saveInfoToFile("boolinfo.txt", list);
		break;
	case 9:
		printf("保存信息\n");		 		 
		printf("请输入保存书籍信息(name,price,num):");
		scanf_s("%19s", tempBook.name, sizeof(tempBook.name)); // 限制输入长度以防止溢出
		scanf_s("%f", &tempBook.price);
		scanf_s("%d", &tempBook.num);
		insertNodeByHead(list, tempBook);
		saveInfoToFile("boolinfo.txt", list);
		break;
	case 0:
		printf("退出系统\n");
		printf("退出成功\n");
		system("pause");
		exit(0);//关闭整个程序
		break;
	default:
		printf("输入错误");
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