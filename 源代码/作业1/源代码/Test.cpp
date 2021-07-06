#include "f_announce.h"

//===============全局变量=============
int count_cmp = 0, count_move = 0;
//====================================


void Test()
{
    Per record;
    if (InitList(&record)) cout << "通讯录已初始化。" << endl;
    else
    {
        cout << "通讯录初始化失败。" << endl;
        return;
    }
    int choice = -1;
    int tchoice = -1;
    int tempa=0, tempb=0, tempc=0;

    while (choice != 0)
    {
        system("color 1F");
        printf("=================通讯录菜单================\n");
        printf("1. 导入联系人文件\n");
        printf("2. 添加联系人\n");
        printf("3. 删除联系人\n");
        printf("4. 显示联系人\n");
        printf("5. 查找联系人\n");
        printf("6. 清空联系人\n");
        printf("7. 保存通讯录\n");
        printf("0. 退出\n");
        printf("===========================================\n");
        printf("\n请选择:");
        scanf_s("%d", &choice);

        switch (choice)
        {
        case 1://导入联系人文件
            if (load_in(&record)) cout << "文件已读入" << endl;
            else cout << "文件读入失败" << endl;
            break;

        case 2://添加联系人
            insert_member(&record);
            break;

        case 3://删除联系人
            delete_member(&record);
            break;

        case 4://显示联系人
            display(&record);
            break;

        case 5://查找联系人
            search(&record);
            break;

        case 6://清空联系人
            ClearList(&record);
            break;

        case 7://保存通讯录
            save(&record);
            break;
        }
    }
}


//===============具体函数================


/*
函数名：load_in     
功能：读入文件 （contact.txt)
输入：存储联系人信息的Per结构体指针 S
*/
int load_in(Per* S)
{
    ifstream ifs;
    ifs.open("contact.txt", ios::in);
    if (!ifs.is_open()) return 0;
    char tmp[100];
    int f = 1, floadin = 0;
    int index;
start:;
    while (ifs >> tmp)
    {
        if (f)
        {
            if (strlen(tmp) != 11)
            {
                floadin = 0;     //非11位号码不存储
                f = 1 - f; continue;
            }
            for (int i = 0; i < 10; i++)  //非法号码不存储
            {
                if (!isdigit(tmp[i])) { floadin = 0; break; }
            }if (floadin == 0) { f = 1 - f; continue; }

            //哈希存储
            index = hash_change(tmp, S);
            int count = 0;
            while (!S->ifempty[index] && count <= S->listsize)
            {
                index = (index + 1) % S->hash_num;
                count++;
            }

            S->size++;
            if (count > S->listsize || S->size > S->listsize)
            {
                againmalloc(S);
                ifs.seekg(0, ios::beg);
                goto start;   //回到循环前重新开始哈希存储
            }
            S->ifempty[index] = false;
            strcpy_s(S->member[index].number, sizeof(S->member[index].number), tmp); //录号码
        }
        else
        {
            if (floadin == 0) floadin = 1; //非11位号码直接跳过
            else strcpy_s(S->member[index].name, sizeof(S->member[index].name), tmp);   //录姓名
        }
        f = 1 - f;
    }
    ifs.close();
    return 1;
}


/*
函数名：insert_member
功能：添加联系人
输入：存储联系人信息的Per结构体指针 S
*/
void insert_member(Per* S)
{
start :
    char tnumber[12];
    char tname[18];
    int ff=1;//是否要继续输入
    while (1)
    {
        cout << "请录入电话号码（11位）："; cin >> tnumber;  //号码过长会溢出
        cout << "请录入姓名："; cin >> tname;   //姓名过长会溢出
        if (strlen(tnumber) != 11)
        {
            cout << "电话号码必须是11位。" << endl;
            cout << "还想输入姓名吗？（选 1 继续，选 0 退出该函数）："; cin >> ff;
            if (!ff) break;
            continue;
        }
        else
        {
            int floadin = 1;
            for (int i = 0; i < 11; i++)
            {
                if (!isdigit(tnumber[i])) { floadin = 0;  break; }
            }if (floadin == 1) break;
            cout << "电话号码非法。" << "\n\n";  //floadin==0 时打印
            cout << "还想输入姓名吗？（选 1 继续，选 0 退出该函数）："; cin >> ff;
            if (!ff) break;
        }
    }if (!ff) return;
    S->size++;
    if (S->size > S->listsize)    //插入记录后，若总记录数大于最大容量，就扩容哈希表并重新进行哈希存储
    {
        againmalloc(S);
        load_in_saved(S);
    }
    int index = hash_change(tnumber, S);
    int count = 0;
    while (!S->ifempty[index] && count <= S->listsize)  //寻找符合条件的哈希地址
    {
        if (!strcmp(S->member[index].number, tnumber))
        {
            cout << "已存在该号码(一个号码只能对应一个人)。" << endl;
            S->size--;
            return;
        }
        index = (index + 1) % S->hash_num;
        count++;
    }
    //录入联系人
    strcpy_s(S->member[index].name, tname);
    strcpy_s(S->member[index].number, tnumber);
    S->ifempty[index] = false;
    cout << "已录入联系人。" << endl;
    save(S);
    cout << "还想输入姓名吗？（选 1 继续，选 0 退出该函数）："; cin >> ff;
    if (!ff) return;
    else goto start;
}


/*
函数名：delete_member
功能：删除联系人
输入：存储联系人信息的Per结构体指针 S
*/
void delete_member(Per* S)
{
    char tnumber[12];
    char tname[17];
    int choice=-1,index,t;
    vector<int>locate;
    while (choice != 0)
    {
        printf("----------------删除菜单----------------\n");
        printf("1.以电话号码为关键字删除（结果唯一）\n");
        printf("2.以姓名为关键字删除（结果不唯一，会把姓名对应的所有号码删除）\n");
        printf("0.退出该分支\n");
        printf("-----------------------------------------\n\n请选择：");
        cin >> choice;
        switch (choice)
        {
        case 1:  //以电话号码为关键字删除（结果唯一）
            while (1)
            {
                cout << "请输入要删除的电话号码（11位）："; cin >> tnumber;
                if (strlen(tnumber) != 11)
                {
                    cout << "电话号码必须是11位。" << endl;
                    continue;
                }
                else 
                {
                    int floadin = 1;
                    for (int i = 0; i < 11; i++)
                    {
                        if (!isdigit(tnumber[i])) { floadin = 0;  break; }
                    }if (floadin == 1) break;
                    cout << "电话号码非法。" << endl;  //floadin==0 时打印
                }
            }
            index = hash_change(tnumber, S); t = 0;
            while (strcmp(S->member[index].number, tnumber) != 0) //对应位置上号码不一致
            {
                index = (index + 1) % S->hash_num;
                t++;
                if (S->ifempty[index] || t == S->listsize - 1)
                {
                    cout << "不存在该联系人。" << endl;
                    return;
                }
            }
            strcpy_s(tnumber, "None");
            strcpy_s(tname, "None");
            strcpy_s(S->member[index].number, tnumber);
            strcpy_s(S->member[index].number, tname);
            S->ifempty[index] = true;
            S->size--;
            cout << "联系号码已删除。" << endl;
            break;

        case 2:  //以姓名为关键字删除（结果不唯一，会把姓名对应的所有号码删除）
            cout << "请输入要删除的联系人姓名："; cin >> tname;
            locate.clear();
            for (int i = 0; i < S->listsize; i++)
            {
                if (!S->ifempty[i] && !strcmp(S->member[i].name, tname)) locate.push_back(i);
            }
            if (locate.size() == 0)
            {
                cout << "不存在联系人" << tname << endl;
                break;
            }
            cout << "已删除联系人" << tname << endl;
            t = locate.size();
            strcpy_s(tnumber, "None");
            strcpy_s(tname, "None");
            for (int i = 0; i < t; i++)
            {
                strcpy_s(S->member[locate[i]].number, tnumber);
                strcpy_s(S->member[locate[i]].number, tname);
                S->ifempty[locate[i]] = true;
                S->size--;
            }
            break;
        }
        int f = 0;
        if (S->size < S->listsize / 2)    //当一半以上的空间没用存内容时，缩减空间为原来的3/4
        {                               //若缩为1/2则会出现插入联系人后又翻倍空间的现象
            save(S);
            declinemalloc(S);
            load_in_saved(S);
            f = 1;
        }
        if (f) cout << "删除该联系人后冗余空间过多，已缩减并重新构建哈希表。" << endl;
    }
    save(S);
}


/*
函数名：display
功能：显示所有学生数据
输入：存储联系人信息的Per结构体指针 S
*/
void display(Per* S)
{
    int temp = 0;
    int i = 0, n = S->size;
    if (n == 0)
    {
        cout << "通讯录为空" << endl;
        return;
    }
    while (temp <= n && i < S->listsize)
    {
        if (!S->ifempty[i])
        {
            cout << S->member[i].number << "   " << S->member[i].name <<"  "<<i << endl;
            //cout << i << endl;
            temp++;
        }
        i++;
    }
    cout << endl;
}


/*
函数名：search
功能：查找联系人
输入：存储联系人信息的Per结构体指针 S
*/
void search(Per* S)
{
    char tnumber[13];
    char tname[18];
    vector<int> locate;
    int choice = -1;
    int index,t;
    while (choice != 0)
    {
        printf("----------------查找菜单----------------\n");
        printf("1.以电话号码为关键字查找（结果唯一）\n");
        printf("2.以姓名为关键字查找（结果不唯一）\n");
        printf("0.退出该分支\n");
        printf("-----------------------------------------\n\n请选择：");
        cin >> choice;
        switch (choice)
        {
        case 1:  //以电话号码为关键字查找（结果唯一）
            while (1)
            {
                cout << "请输入要查找的电话号码（11位）："; cin >> tnumber;
                if (strlen(tnumber) != 11)
                {
                    cout << "电话号码必须是11位。" << endl;
                    continue;
                }
                else 
                {
                    int floadin = 1;
                    for (int i = 0; i < 11; i++)
                    {
                        if (!isdigit(tnumber[i])) { floadin = 0;  break; }
                    }if (floadin == 1) break;
                    cout << "电话号码非法。" << endl;  //floadin==0 时打印
                }
            }
            index = hash_change(tnumber, S); t = 0;
            while (strcmp(S->member[index].number, tnumber) != 0) //对应位置上号码不一致
            {
                index = (index + 1) % S->hash_num;
                t++;
                if (S->ifempty[index] || t == S->listsize - 1)
                {
                    cout << "不存在该联系人。" << endl;
                    return;
                }
            }
            cout << "找到联系人 " << S->member[index].name << endl;
            break;
        case 2:  //以姓名为关键字查找（结果不唯一）
            cout << "请输入要查找的联系人姓名："; cin >> tname;
            locate.clear();
            for (int i = 0; i < S->listsize; i++)
            {
                if (!S->ifempty[i] && !strcmp(S->member[i].name, tname)) locate.push_back(i);
            }
            if (locate.size() == 0)
            {
                cout << "不存在联系人" << tname << endl;
                break;
            }
            cout << tname << ":" << endl;
            t = locate.size();
            for (int i = 0; i < t; i++) cout << "    " << S->member[locate[i]].number << endl;
            break;
        }
    }
    return ;
}


/*
函数名：ClearList
功能：清空通讯录
输入：存储联系人信息的Per结构体指针 S
*/
void ClearList(Per* S)
{
    InitList(S);
    cout << "通讯录已清空。" << endl;
}


/*
函数名：save
功能：保存所有联系人数据    （存入‘connect_save.txt’）
输入：存储联系人信息的Per结构体指针 S
*/
void save(Per* S)
{
    int temp = 0;
    int i = 0, n = S->size;    
    ofstream ofs;
    ofs.open("connect_save.txt", ios::out | ios::trunc);
    if (n == 0)
    {
        cout << "通讯录为空" << endl;
        return;
    }
    while (temp <= n && i < S->listsize)
    {
        if (!S->ifempty[i])
        {
            ofs << S->member[i].number << "    " << S->member[i].name << endl;
            temp++;
        }
        i++;
    }
    ofs.close();
    cout << "通讯录已保存。" << endl;
}

/*
函数名：hash_change
功能：针对电话号码计算哈希地址       将11位电话号码的第5-8位构成的数字加上第9-11位构成的数字
*/
int hash_change(char* t, Per* S)
{
    int n1 = 0, n2 = 0;
    for (int i = 0; i <= 7; i++) n1 = n1 * 10 + (t[i] - '0');
    for (int i = 10; i >= 3; i--) n2 = n2 * 10 + (t[i] - '0');
    return (n1 + n2) % S->hash_num;
}





//-------读connect_save.txt------
int load_in_saved(Per* S)
{
    ifstream ifs;
    ifs.open("connect_save.txt", ios::in);
    if (!ifs.is_open()) return 0;
    char tmp[25];
    int f = 1;
    int index;
start:;
    while (ifs >> tmp)
    {
        if (f)
        {
            index = hash_change(tmp, S);
            int count = 0;
            while (!S->ifempty[index] && count <= S->listsize)
            {
                index = (index + 1) % S->hash_num;
                count++;
            }
            S->size++;
            if (count > S->listsize || S->size > S->listsize)
            {
                againmalloc(S);
                //ifs.clear();
                ifs.seekg(0, ios::beg);
                goto start;   //回到循环前重新开始哈希存储
            }
            S->ifempty[index] = false;
            strcpy_s(S->member[index].number, sizeof(S->member[index].number), tmp);
        }
        else
        {
            strcpy_s(S->member[index].name, sizeof(S->member[index].name), tmp);
        }
        f = 1 - f;
    }
    ifs.close();
    return 1;
}