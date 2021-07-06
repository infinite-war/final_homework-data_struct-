#include "gloabledef.h"

//��ϵ�˼�¼
typedef struct info
{
	char number[13];      //Ĭ��һ����ϵ��ֻ��Ӧһ����ϵ��ʽ
	char name[29];
}information;

typedef struct Person
{
	info*  member;
	bool*  ifempty;   //1Ϊ�գ�0Ϊ�ǿ� 
	int size;
	int listsize;
	int hash_num;     //������������ģ��
}Per;


//��������
void Test();	//ִ�г���
int load_in(Per* S);	 //������ϵ���ļ�

void insert_member(Per* S);//�����ϵ��

void delete_member(Per* S);//ɾ����ϵ��

void display(Per* S);//��ʾ��ϵ��

void search(Per* S);//������ϵ��

void ClearList(Per* S);//�����ϵ��

void save(Per* S);//����ͨѶ¼

int hash_change(char *t,Per* S);//�����ϣ��ַ

int load_in_saved(Per* S);
//=======��������=======

/* ��ʼ��ͨѶ¼  */

inline int InitList(Per* S)
{
	S->member = (info*)malloc(sizeof(info)* MAXSIZE);
	S->ifempty = (bool*)malloc(sizeof(bool) * MAXSIZE);
	if (!S->member || !S->ifempty) {
		printf("��̬�洢����ʧ�ܡ�\n");
		exit(OVERFLOW);
	}
	//���ݳ�ʼ��
	memset(S->member, 0, sizeof(info) * MAXSIZE);
	memset(S->ifempty, true, sizeof(bool) * MAXSIZE);
	S->size = 0; 
	S->listsize = MAXSIZE; 
	S->hash_num = MAXSIZE;
	return TRUE;
}

/*    �����ڴ�     */
inline void againmalloc(Per* S)
{
	S->size = 0;
	S->hash_num = 2 * S->listsize;
	S->listsize = 2 * S->listsize;
	info* p = (info*)realloc((S->member), S->listsize * (sizeof(info)));
	bool* q = (bool*)realloc((S->ifempty), S->listsize * sizeof(bool));
	if (!p || !q)
	{
		cout << "�洢�ռ����ʧ�ܣ�" << endl;
		exit(OVERFLOW);
	}
	//���ݳ�ʼ��
	S->member = p;
	S->ifempty = q;
	memset(S->member, 0, sizeof(info) * S->listsize);
	memset(S->ifempty, true, sizeof(bool) * S->listsize);
	cout<<"�ռ�����Ϊ2��"<< endl;
}

/*     ��С�ڴ�    */   
inline void declinemalloc(Per* S)
{
	S->size = 0;
	S->size = 0;
	S->hash_num = 2 * S->listsize; 
	S->listsize = 2 * S->listsize;    //����������Ϊԭ����3/4
	info* p = (info*)realloc((S->member), S->listsize * (sizeof(info)));
	bool* q = (bool*)realloc((S->ifempty), S->listsize * sizeof(bool));
	if (!p || !q)
	{
		cout << "�洢�ռ����ʧ�ܣ�" << endl;
		exit(OVERFLOW);
	}
	//���ݳ�ʼ��
	S->member = p;
	S->ifempty = q;
	memset(S->member, 0, sizeof(info) * S->listsize);
	memset(S->ifempty, true, sizeof(bool) * S->listsize);
	cout << "�ռ�������Ϊԭ����3/4" << endl;
}