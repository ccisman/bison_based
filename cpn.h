#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<stack>
#include<sstream>


#include"tree.h"
using namespace std;
//��ɫPetri���Ľṹ

//const string colorset[] = { "int","char","float","double","short","long" };

extern bool execute_flag;

class Arc
{
public:
	
	static int total_num;

	string source;//Դ������
	string target;//Ŀ�������
	string V;//��¼���ϵı���
	int id_num;
	int sourceNum = 0;
	int targetNum = 0;
	bool sourceP;//false����Դ�ڵ��Ǳ�Ǩ
	int type;//��¼�������ͣ�1����ֻ��������,2����������������,3����ֻ��������T->P,4����ֻ��������P->T��6����while�Ļ�ָ��
//	int weight = 1;//��¼���ϵ�Ȩֵ

	Arc(string s, string t, string v, bool s_P);
};


class Place
{
public:
	int id_num;
	static int total_num;
	string name;//���������ƣ����ڻ��ı�ǣ�
	string v_name;//������ű��������ı�����
	string str;//colorsetΪstring����charʱ�洢ֵ
	int token_num;//token�ĸ���
	string colorset_tag;//��Ӧcolorset����ֵ
	int *num;//colorsetΪ����ʱ�洢ֵ
	int n_num;//num����
	double *decimal;//colorsetΪС��ʱ�洢ֵ
	int n_decimal;//decimal����
	bool controlP;//���ֿ��ƿ������Ǳ�������
	bool ispoint;//��ʾ�Ƿ�Ϊָ��
	bool is_goto;//��ʶ�Ƿ�Ϊgoto
	string goto_label;//��ʶgoto����label
	//int current;//��ʾ��ǰλ�ã���������������
	vector<string> false_exit;//ѭ�����ٳ���

	vector<string> enter;//��ǿ�ʼ��Ǩ
	vector<string> exit;//��ǽ�β��Ǩ
	vector<string> enter_P;//�����ڿ����������������õ������ڿ���Ϊ()����)
	vector<string> control_T;//��ǿ�����Ӧ�Ŀ��Ʊ�Ǩ
	vector<string> call_P;//����begin�����а������е������Ŀ���
	vector<string> label_after_P;//��ǩ�������У���¼������
	string fun_P;//������ں�����begin����
	bool global;//ȫ�ֱ�����־
	string pre_executed_P;

public:
	Place(string n, string v_n, string colorset_t, bool c_P);
	void set_Place_value(int num);
	void set_Place_value(double decimal);
	void set_Place_value(string str);
};

class Transition
{
public:
	
	static int total_num;

	string name;//��Ǩ�����ƣ����ڻ��ı�ǣ�
	string booleanExpression;//���������ַ����洢bool���ʽ
	string v_Expression;//�����ڴ�ű������ʽ
//	int num = 0;
	int id_num;
	int current_P_num;
	bool controlT;//���ֿ����ڻ��Ǳ�����
	//bool TorF;//�����ڵ�ֵ

	Transition(string n, bool c_T, int current_P_num);
	void set_C_Transition_value(string str);
	void set_V_Transition_value(string str);
};

class C_Petri
{
public:
	int p_num;//��������
	int t_num;//��Ǩ����
	int arcnum;//������


	vector<Place> place;//����
	vector<Transition> transition;//��Ǩ
	vector<Arc> arc;//��


public:
	C_Petri();
	void operator=(C_Petri p1)
	{
		arc = p1.arc;
		arcnum = p1.arcnum;
		place = p1.place;
		p_num = p1.p_num;
		transition = p1.transition;
		t_num = p1.t_num;
	}
	void Add_Place(Place p);
	void Add_Place(string name, string v_name, string colorset_tag, bool controlP, bool t, int n, double dec, string s, int array_size, bool ispoint);//t=true��ʾ�г�ʼֵ


	void Add_Transition(Transition t);
	void Add_Transition(string name, bool controlT, string s, int current_P_num);


	void Add_Arc(Arc a);
	void Add_Arc(string source, string target, string V, bool sourceP);

	string find_place(string t_name, string V);//ͨ����Ǩ�ͻ��ϵı���ֵ�ҵ���Ӧ����,���ؿ���name

	bool Add_Place_enter(string name, string s);
	bool Add_Place_enter(string name, vector<string> s);
	bool Add_Place_exit(string name, string s);
	bool Add_Place_exit(string name, vector<string> s);
	//	bool Add_Place_controlT(string name, vector<string> s);
	bool is_enable(string t);//�жϱ�Ǩt�Ƿ�ɷ���
	bool guard(string booleanExpression, string T); //guard�����Ĵ���
	//bool guard_process(string booleanExpression, string T);
	string Delete_Arc(int i);
	string Delete_Arc(string source, string target);
	void Change_Arc(int i, string source, string target, string V, bool sourceP);
	void delete_all_connected_arc(string place);

	vector<string> get_enter(string name);
	vector<string> get_exit(string name);

	//	vector<string> get_controlT(string name);
	vector<string> enable_T(); //�����������пɷ����ı�Ǩ
	vector<string> find_all_place(string t_name);//�ұ�Ǩ������ǰ������

	int get_current_P_num(string T);
	void set_point_flag(string p_name);
	bool get_point_flag(string p_name);
	void set_false_exit(string p_name, vector<string> false_exit);
	vector<string> get_false_exit(string p_name);
	void set_enter_P(string p_name, vector<string> enter_P);
	vector<string> get_enter_P(string p_name);
	void set_control_T(string p_name, vector<string> control_T);
	vector<string> get_control_T(string p_name);
	void clear_enter(string p_name);
	void set_arc_type(string source, string target, int type);
	int get_arc_type(string source, string target);
	void add_call_P(string p_name, string call_P);
	vector<string> get_call_P(string p_name);
	void set_fun_P(string p_name, string fun_P);
	string get_fun_P(string p_name);
	void set_global(string p_name);
	bool get_global(string p_name);
	void set_pre_executed_P(string p_name, string pre_executed_P);
	string get_pre_executed_P(string p_name);
	void add_label_P(string p_name, string call_P);
	vector<string> get_label_P(string p_name);
	void release();
};


string find_P_name(C_Petri petri, string v_name);
string find_T_name(C_Petri petri, string v_name);
double change_and_cal(string s, vector<Place> place, int current);

C_Petri changeAnalyse(C_Petri &petri, vector<string> change_places);

int string_replace(string &s1, const string &s2, const string &s3);

void SplitString(const string& s, vector<string>& v, const string& c);

void splitExpression(string &s, vector<string>& v);

void ast_to_cpn(C_Petri &petri, gtree *p, int addition);//additionΪ0��ʾֱ�ӹ����������ʾ����仯ʱ����

void reset_gen_cpn();

void process_label(C_Petri &petri);//�����ǩ���