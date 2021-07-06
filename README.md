大一下“数据结构与算法”课设（通讯录）
基于哈希存储的简易通讯录

①数据结构：
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

②函数声明：
void Test();	//执行程序
int load_in(Per* S);	 //导入联系人文件
void insert_member(Per* S);//添加联系人
void delete_member(Per* S);//删除联系人
void display(Per* S);//显示联系人
void search(Per* S);//查找联系人
void ClearList(Per* S);//清空联系人
void save(Per* S);//保存通讯录
int hash_change(char *t,Per* S);//计算哈希地址
int load_in_saved(Per* S);//从“connect_save.txt”中读文件
inline int InitList(Per* S) //初始化通讯录
inline void againmalloc(Per* S)//扩充内存
inline void declinemalloc(Per* S)//缩小内存



③功能菜单：
===========通讯录菜单==========
1. 导入联系人文件\n");
2. 添加联系人
3. 删除联系人
4. 显示联系人
5. 查找联系人
6. 清空联系人
7. 保存通讯录
0. 退出


===========删除菜单============
1.以电话号码为关键字删除（结果唯一）
2.以姓名为关键字删除（结果不唯一，会把姓名对应的所有号码删除）
0.退出该分支

===========查找菜单============
1.以电话号码为关键字查找（结果唯一）
2.以姓名为关键字查找（结果不唯一，会把姓名对应的所有号码删除）
0.退出该分支
