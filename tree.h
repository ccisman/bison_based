#pragma once
#include<iostream>
#include<string>

using namespace std;
#define MAX_LENGTH 3000
#define MAX_WORD_NUM 1000
#define MAX_BUFF_SIZE 40
#define tb_sum 20
#define kong -1
#define ERROR1 -1

//enum item
//{
//	�ؼ��� = 1, ��ʶ��, �޷�������, ��ֵ��, ���, ���, ð��, �ָ���, ע�ͺ�, ������, ������, ��������, ��������, �������, �Ҵ�����, ��ĸ, �޷���ʵ��, ������, �ַ���, �ַ�,
//	����, ����˵��, ��������, ����, ʵ��, ����ͷ��, ����˵��, ��������, ��ö�ٳ���, ���ͱ�ʶ��, ��������, �������, ������, ������, ���ʽ,
//	��, ����, ���, ��ֵ���, �������, ����, ѭ�����, ������, �����, ��������, ȱʡ, �����������, ֵ������, �����, �����, д���, �������, ������, ����, break���
//};

static string dirname = ".\\rg\\";
static string origin_dirname = "D:\\ѧϰ����\\��Ŀ����\\petri��ģ\\combine_test\\";
static string newfile_dirname = ".\\newfile\\";

class gtree
{
public:
	//�������֣����������Ϣ
	int num;//����ڵ��
	string place;
	int No_Child;//���ӵĸ���
	//string code;
	//�������֣����������Ϣ
	gtree *next;//�ֵ�ָ��
	gtree *child;//����ָ��
	gtree *parent;//���ڵ�ָ��
	int type;//�ڵ�����syn
	string contain_call;//��Ű����ĺ����������V_name
	bool contain_call_flag;
	int record_P_num;


	void init(int root);//��ʼ��
	void expand(int num, gtree *t, int sum);
	void cnode(int syn, int syn_1, gtree *t);
	void newlable(int a);
	string newlable1(int a, int flag);
	//void gen(string a);
};

gtree *&create_tree(string filename, bool flag);


string get_gen_P();

gtree* find_last_sentence(gtree *node, bool &flag, bool &flag1);

bool judge_assign_statement(gtree *statement1);
bool judge_call_postfix_expression(gtree *postfix_expression);
bool judge_call_statement(gtree *statement1);
bool judge_return_statement(gtree *statement1);
bool judge_expression_statement(gtree *statement1);
void pre_process(string &s);