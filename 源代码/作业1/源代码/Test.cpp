#include "f_announce.h"

//===============ȫ�ֱ���=============
int count_cmp = 0, count_move = 0;
//====================================


void Test()
{
    Per record;
    if (InitList(&record)) cout << "ͨѶ¼�ѳ�ʼ����" << endl;
    else
    {
        cout << "ͨѶ¼��ʼ��ʧ�ܡ�" << endl;
        return;
    }
    int choice = -1;
    int tchoice = -1;
    int tempa=0, tempb=0, tempc=0;

    while (choice != 0)
    {
        system("color 1F");
        printf("=================ͨѶ¼�˵�================\n");
        printf("1. ������ϵ���ļ�\n");
        printf("2. �����ϵ��\n");
        printf("3. ɾ����ϵ��\n");
        printf("4. ��ʾ��ϵ��\n");
        printf("5. ������ϵ��\n");
        printf("6. �����ϵ��\n");
        printf("7. ����ͨѶ¼\n");
        printf("0. �˳�\n");
        printf("===========================================\n");
        printf("\n��ѡ��:");
        scanf_s("%d", &choice);

        switch (choice)
        {
        case 1://������ϵ���ļ�
            if (load_in(&record)) cout << "�ļ��Ѷ���" << endl;
            else cout << "�ļ�����ʧ��" << endl;
            break;

        case 2://�����ϵ��
            insert_member(&record);
            break;

        case 3://ɾ����ϵ��
            delete_member(&record);
            break;

        case 4://��ʾ��ϵ��
            display(&record);
            break;

        case 5://������ϵ��
            search(&record);
            break;

        case 6://�����ϵ��
            ClearList(&record);
            break;

        case 7://����ͨѶ¼
            save(&record);
            break;
        }
    }
}


//===============���庯��================


/*
��������load_in     
���ܣ������ļ� ��contact.txt)
���룺�洢��ϵ����Ϣ��Per�ṹ��ָ�� S
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
                floadin = 0;     //��11λ���벻�洢
                f = 1 - f; continue;
            }
            for (int i = 0; i < 10; i++)  //�Ƿ����벻�洢
            {
                if (!isdigit(tmp[i])) { floadin = 0; break; }
            }if (floadin == 0) { f = 1 - f; continue; }

            //��ϣ�洢
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
                goto start;   //�ص�ѭ��ǰ���¿�ʼ��ϣ�洢
            }
            S->ifempty[index] = false;
            strcpy_s(S->member[index].number, sizeof(S->member[index].number), tmp); //¼����
        }
        else
        {
            if (floadin == 0) floadin = 1; //��11λ����ֱ������
            else strcpy_s(S->member[index].name, sizeof(S->member[index].name), tmp);   //¼����
        }
        f = 1 - f;
    }
    ifs.close();
    return 1;
}


/*
��������insert_member
���ܣ������ϵ��
���룺�洢��ϵ����Ϣ��Per�ṹ��ָ�� S
*/
void insert_member(Per* S)
{
start :
    char tnumber[12];
    char tname[18];
    int ff=1;//�Ƿ�Ҫ��������
    while (1)
    {
        cout << "��¼��绰���루11λ����"; cin >> tnumber;  //������������
        cout << "��¼��������"; cin >> tname;   //�������������
        if (strlen(tnumber) != 11)
        {
            cout << "�绰���������11λ��" << endl;
            cout << "�������������𣿣�ѡ 1 ������ѡ 0 �˳��ú�������"; cin >> ff;
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
            cout << "�绰����Ƿ���" << "\n\n";  //floadin==0 ʱ��ӡ
            cout << "�������������𣿣�ѡ 1 ������ѡ 0 �˳��ú�������"; cin >> ff;
            if (!ff) break;
        }
    }if (!ff) return;
    S->size++;
    if (S->size > S->listsize)    //�����¼�����ܼ�¼��������������������ݹ�ϣ�����½��й�ϣ�洢
    {
        againmalloc(S);
        load_in_saved(S);
    }
    int index = hash_change(tnumber, S);
    int count = 0;
    while (!S->ifempty[index] && count <= S->listsize)  //Ѱ�ҷ��������Ĺ�ϣ��ַ
    {
        if (!strcmp(S->member[index].number, tnumber))
        {
            cout << "�Ѵ��ڸú���(һ������ֻ�ܶ�Ӧһ����)��" << endl;
            S->size--;
            return;
        }
        index = (index + 1) % S->hash_num;
        count++;
    }
    //¼����ϵ��
    strcpy_s(S->member[index].name, tname);
    strcpy_s(S->member[index].number, tnumber);
    S->ifempty[index] = false;
    cout << "��¼����ϵ�ˡ�" << endl;
    save(S);
    cout << "�������������𣿣�ѡ 1 ������ѡ 0 �˳��ú�������"; cin >> ff;
    if (!ff) return;
    else goto start;
}


/*
��������delete_member
���ܣ�ɾ����ϵ��
���룺�洢��ϵ����Ϣ��Per�ṹ��ָ�� S
*/
void delete_member(Per* S)
{
    char tnumber[12];
    char tname[17];
    int choice=-1,index,t;
    vector<int>locate;
    while (choice != 0)
    {
        printf("----------------ɾ���˵�----------------\n");
        printf("1.�Ե绰����Ϊ�ؼ���ɾ�������Ψһ��\n");
        printf("2.������Ϊ�ؼ���ɾ���������Ψһ�����������Ӧ�����к���ɾ����\n");
        printf("0.�˳��÷�֧\n");
        printf("-----------------------------------------\n\n��ѡ��");
        cin >> choice;
        switch (choice)
        {
        case 1:  //�Ե绰����Ϊ�ؼ���ɾ�������Ψһ��
            while (1)
            {
                cout << "������Ҫɾ���ĵ绰���루11λ����"; cin >> tnumber;
                if (strlen(tnumber) != 11)
                {
                    cout << "�绰���������11λ��" << endl;
                    continue;
                }
                else 
                {
                    int floadin = 1;
                    for (int i = 0; i < 11; i++)
                    {
                        if (!isdigit(tnumber[i])) { floadin = 0;  break; }
                    }if (floadin == 1) break;
                    cout << "�绰����Ƿ���" << endl;  //floadin==0 ʱ��ӡ
                }
            }
            index = hash_change(tnumber, S); t = 0;
            while (strcmp(S->member[index].number, tnumber) != 0) //��Ӧλ���Ϻ��벻һ��
            {
                index = (index + 1) % S->hash_num;
                t++;
                if (S->ifempty[index] || t == S->listsize - 1)
                {
                    cout << "�����ڸ���ϵ�ˡ�" << endl;
                    return;
                }
            }
            strcpy_s(tnumber, "None");
            strcpy_s(tname, "None");
            strcpy_s(S->member[index].number, tnumber);
            strcpy_s(S->member[index].number, tname);
            S->ifempty[index] = true;
            S->size--;
            cout << "��ϵ������ɾ����" << endl;
            break;

        case 2:  //������Ϊ�ؼ���ɾ���������Ψһ�����������Ӧ�����к���ɾ����
            cout << "������Ҫɾ������ϵ��������"; cin >> tname;
            locate.clear();
            for (int i = 0; i < S->listsize; i++)
            {
                if (!S->ifempty[i] && !strcmp(S->member[i].name, tname)) locate.push_back(i);
            }
            if (locate.size() == 0)
            {
                cout << "��������ϵ��" << tname << endl;
                break;
            }
            cout << "��ɾ����ϵ��" << tname << endl;
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
        if (S->size < S->listsize / 2)    //��һ�����ϵĿռ�û�ô�����ʱ�������ռ�Ϊԭ����3/4
        {                               //����Ϊ1/2�����ֲ�����ϵ�˺��ַ����ռ������
            save(S);
            declinemalloc(S);
            load_in_saved(S);
            f = 1;
        }
        if (f) cout << "ɾ������ϵ�˺�����ռ���࣬�����������¹�����ϣ��" << endl;
    }
    save(S);
}


/*
��������display
���ܣ���ʾ����ѧ������
���룺�洢��ϵ����Ϣ��Per�ṹ��ָ�� S
*/
void display(Per* S)
{
    int temp = 0;
    int i = 0, n = S->size;
    if (n == 0)
    {
        cout << "ͨѶ¼Ϊ��" << endl;
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
��������search
���ܣ�������ϵ��
���룺�洢��ϵ����Ϣ��Per�ṹ��ָ�� S
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
        printf("----------------���Ҳ˵�----------------\n");
        printf("1.�Ե绰����Ϊ�ؼ��ֲ��ң����Ψһ��\n");
        printf("2.������Ϊ�ؼ��ֲ��ң������Ψһ��\n");
        printf("0.�˳��÷�֧\n");
        printf("-----------------------------------------\n\n��ѡ��");
        cin >> choice;
        switch (choice)
        {
        case 1:  //�Ե绰����Ϊ�ؼ��ֲ��ң����Ψһ��
            while (1)
            {
                cout << "������Ҫ���ҵĵ绰���루11λ����"; cin >> tnumber;
                if (strlen(tnumber) != 11)
                {
                    cout << "�绰���������11λ��" << endl;
                    continue;
                }
                else 
                {
                    int floadin = 1;
                    for (int i = 0; i < 11; i++)
                    {
                        if (!isdigit(tnumber[i])) { floadin = 0;  break; }
                    }if (floadin == 1) break;
                    cout << "�绰����Ƿ���" << endl;  //floadin==0 ʱ��ӡ
                }
            }
            index = hash_change(tnumber, S); t = 0;
            while (strcmp(S->member[index].number, tnumber) != 0) //��Ӧλ���Ϻ��벻һ��
            {
                index = (index + 1) % S->hash_num;
                t++;
                if (S->ifempty[index] || t == S->listsize - 1)
                {
                    cout << "�����ڸ���ϵ�ˡ�" << endl;
                    return;
                }
            }
            cout << "�ҵ���ϵ�� " << S->member[index].name << endl;
            break;
        case 2:  //������Ϊ�ؼ��ֲ��ң������Ψһ��
            cout << "������Ҫ���ҵ���ϵ��������"; cin >> tname;
            locate.clear();
            for (int i = 0; i < S->listsize; i++)
            {
                if (!S->ifempty[i] && !strcmp(S->member[i].name, tname)) locate.push_back(i);
            }
            if (locate.size() == 0)
            {
                cout << "��������ϵ��" << tname << endl;
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
��������ClearList
���ܣ����ͨѶ¼
���룺�洢��ϵ����Ϣ��Per�ṹ��ָ�� S
*/
void ClearList(Per* S)
{
    InitList(S);
    cout << "ͨѶ¼����ա�" << endl;
}


/*
��������save
���ܣ�����������ϵ������    �����롮connect_save.txt����
���룺�洢��ϵ����Ϣ��Per�ṹ��ָ�� S
*/
void save(Per* S)
{
    int temp = 0;
    int i = 0, n = S->size;    
    ofstream ofs;
    ofs.open("connect_save.txt", ios::out | ios::trunc);
    if (n == 0)
    {
        cout << "ͨѶ¼Ϊ��" << endl;
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
    cout << "ͨѶ¼�ѱ��档" << endl;
}

/*
��������hash_change
���ܣ���Ե绰��������ϣ��ַ       ��11λ�绰����ĵ�5-8λ���ɵ����ּ��ϵ�9-11λ���ɵ�����
*/
int hash_change(char* t, Per* S)
{
    int n1 = 0, n2 = 0;
    for (int i = 0; i <= 7; i++) n1 = n1 * 10 + (t[i] - '0');
    for (int i = 10; i >= 3; i--) n2 = n2 * 10 + (t[i] - '0');
    return (n1 + n2) % S->hash_num;
}





//-------��connect_save.txt------
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
                goto start;   //�ص�ѭ��ǰ���¿�ʼ��ϣ�洢
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