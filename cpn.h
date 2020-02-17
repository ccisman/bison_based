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
	vector<string> false_exit;

	vector<string> enter;//��ǿ�ʼ��Ǩ
	vector<string> exit;//��ǽ�β��Ǩ
	vector<string> enter_P;//�����ڿ����������������õ������ڿ���Ϊ()����)
	vector<string> control_T;//��ǿ�����Ӧ�Ŀ��Ʊ�Ǩ
//	vector<string> c_transition;//��¼��������ѭ�����Ŀ��Ʊ�Ǩ

	int call_flag;//����һЩ������,1����while����������к�������
	vector<string> information;//while�����ƿ������к������ã����_c��_()�Ŀ�����������ѭ��
	//bool call_last;//����������һ���

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

	int get_call_flag(string name);
	void set_call_flag(string name, int flag);
	vector<string> get_information(string name);
	void Add_information(string name, string information);
	//bool get_call_last(string name);
	//void set_call_last(string name, bool call_last);
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
	void release();
};


string find_P_name(C_Petri petri, string v_name);
string find_T_name(C_Petri petri, string v_name);
double change_and_cal(string s, vector<Place> place, int current);

C_Petri changeAnalyse(C_Petri &petri, vector<string> change_places);

int string_replace(string &s1, const string &s2, const string &s3);

void SplitString(const string& s, vector<string>& v, const string& c);

void splitExpression(string &s, vector<string>& v);

void ast_to_cpn(C_Petri &petri, gtree *p);

void reset_gen_cpn();