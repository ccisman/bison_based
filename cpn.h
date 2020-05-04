#pragma once
#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<stack>
#include<sstream>


#include"tree.h"
using namespace std;
//有色Petri网的结构

//const string colorset[] = { "int","char","float","double","short","long" };

extern bool execute_flag;

class Arc
{
public:
	
	static int total_num;

	string source;//源的名称
	string target;//目标的名称
	string V;//记录弧上的变量
	int id_num;
	int sourceNum = 0;
	int targetNum = 0;
	bool sourceP;//false代表源节点是变迁
	int type;//记录弧的类型，1代表只允许反向找,2代表正反都不允许,3代表只允许正向T->P,4代表只允许正向P->T，6代表while的回指弧
//	int weight = 1;//记录弧上的权值

	Arc(string s, string t, string v, bool s_P);
};


class Place
{
public:
	int id_num;
	static int total_num;
	string name;//库所的名称（用于弧的标记）
	string v_name;//用来存放变量库所的变量名
	string str;//colorset为string或者char时存储值
	int token_num;//token的个数
	string colorset_tag;//对应colorset的数值
	int *num;//colorset为整型时存储值
	int n_num;//num数量
	double *decimal;//colorset为小数时存储值
	int n_decimal;//decimal数量
	bool controlP;//区分控制库所还是变量库所
	bool ispoint;//表示是否为指针
	bool is_goto;//标识是否为goto
	string goto_label;//标识goto语句的label
	//int current;//表示当前位置，用于区分作用域
	vector<string> false_exit;//循环语句假出口

	vector<string> enter;//标记开始变迁
	vector<string> exit;//标记结尾变迁
	vector<string> enter_P;//标记入口库所（包含函数调用的语句入口库所为()库所)
	vector<string> control_T;//标记库所对应的控制变迁
	vector<string> call_P;//函数begin库所中包含所有调用它的库所
	vector<string> label_after_P;//标签库所特有，记录上下文
	string fun_P;//存放所在函数的begin库所
	bool global;//全局变量标志
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

	string name;//变迁的名称（用于弧的标记）
	string booleanExpression;//控制哨用字符串存储bool表达式
	string v_Expression;//变量哨存放变量表达式
//	int num = 0;
	int id_num;
	int current_P_num;
	bool controlT;//区分控制哨还是变量哨
	//bool TorF;//控制哨的值

	Transition(string n, bool c_T, int current_P_num);
	void set_C_Transition_value(string str);
	void set_V_Transition_value(string str);
};

class C_Petri
{
public:
	int p_num;//库所个数
	int t_num;//变迁个数
	int arcnum;//弧个数


	vector<Place> place;//库所
	vector<Transition> transition;//变迁
	vector<Arc> arc;//弧


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
	void Add_Place(string name, string v_name, string colorset_tag, bool controlP, bool t, int n, double dec, string s, int array_size, bool ispoint);//t=true表示有初始值


	void Add_Transition(Transition t);
	void Add_Transition(string name, bool controlT, string s, int current_P_num);


	void Add_Arc(Arc a);
	void Add_Arc(string source, string target, string V, bool sourceP);

	string find_place(string t_name, string V);//通过变迁和弧上的变量值找到对应库所,返回库所name

	bool Add_Place_enter(string name, string s);
	bool Add_Place_enter(string name, vector<string> s);
	bool Add_Place_exit(string name, string s);
	bool Add_Place_exit(string name, vector<string> s);
	//	bool Add_Place_controlT(string name, vector<string> s);
	bool is_enable(string t);//判断变迁t是否可发生
	bool guard(string booleanExpression, string T); //guard函数的处理
	//bool guard_process(string booleanExpression, string T);
	string Delete_Arc(int i);
	string Delete_Arc(string source, string target);
	void Change_Arc(int i, string source, string target, string V, bool sourceP);
	void delete_all_connected_arc(string place);

	vector<string> get_enter(string name);
	vector<string> get_exit(string name);

	//	vector<string> get_controlT(string name);
	vector<string> enable_T(); //返回网内所有可发生的变迁
	vector<string> find_all_place(string t_name);//找变迁的所有前驱库所

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

void ast_to_cpn(C_Petri &petri, gtree *p, int addition);//addition为0表示直接构建，其余表示程序变化时构建

void reset_gen_cpn();

void process_label(C_Petri &petri);//处理标签语句