#include "gloabledef.h"

//联系人记录
typedef struct info
{
	char number[13];      //默认一个联系人只对应一个联系方式
	char name[29];
}information;

typedef struct Person
{
	info*  member;
	bool*  ifempty;   //1为空，0为非空 
	int size;
	int listsize;
	int hash_num;     //除留余数法的模数
}Per;


//函数声明
void Test();	//执行程序
int load_in(Per* S);	 //导入联系人文件

void insert_member(Per* S);//添加联系人

void delete_member(Per* S);//删除联系人

void display(Per* S);//显示联系人

void search(Per* S);//查找联系人

void ClearList(Per* S);//清空联系人

void save(Per* S);//保存通讯录

int hash_change(char *t,Per* S);//计算哈希地址

int load_in_saved(Per* S);
//=======辅助函数=======

/* 初始化通讯录  */

inline int InitList(Per* S)
{
	S->member = (info*)malloc(sizeof(info)* MAXSIZE);
	S->ifempty = (bool*)malloc(sizeof(bool) * MAXSIZE);
	if (!S->member || !S->ifempty) {
		printf("动态存储分配失败。\n");
		exit(OVERFLOW);
	}
	//内容初始化
	memset(S->member, 0, sizeof(info) * MAXSIZE);
	memset(S->ifempty, true, sizeof(bool) * MAXSIZE);
	S->size = 0; 
	S->listsize = MAXSIZE; 
	S->hash_num = MAXSIZE;
	return TRUE;
}

/*    扩充内存     */
inline void againmalloc(Per* S)
{
	S->size = 0;
	S->hash_num = 2 * S->listsize;
	S->listsize = 2 * S->listsize;
	info* p = (info*)realloc((S->member), S->listsize * (sizeof(info)));
	bool* q = (bool*)realloc((S->ifempty), S->listsize * sizeof(bool));
	if (!p || !q)
	{
		cout << "存储空间分配失败！" << endl;
		exit(OVERFLOW);
	}
	//内容初始化
	S->member = p;
	S->ifempty = q;
	memset(S->member, 0, sizeof(info) * S->listsize);
	memset(S->ifempty, true, sizeof(bool) * S->listsize);
	cout<<"空间已扩为2倍"<< endl;
}

/*     缩小内存    */   
inline void declinemalloc(Per* S)
{
	S->size = 0;
	S->size = 0;
	S->hash_num = 2 * S->listsize; 
	S->listsize = 2 * S->listsize;    //将容量缩减为原来的3/4
	info* p = (info*)realloc((S->member), S->listsize * (sizeof(info)));
	bool* q = (bool*)realloc((S->ifempty), S->listsize * sizeof(bool));
	if (!p || !q)
	{
		cout << "存储空间分配失败！" << endl;
		exit(OVERFLOW);
	}
	//内容初始化
	S->member = p;
	S->ifempty = q;
	memset(S->member, 0, sizeof(info) * S->listsize);
	memset(S->ifempty, true, sizeof(bool) * S->listsize);
	cout << "空间已缩减为原来的3/4" << endl;
}