#ifndef PRODUCT_H
#define PRODUCT_H
#define max_to_string 30
#include<cstdio>
#include<iostream>
#include<algorithm>
#include<set>
#include<vector>
#include<string>
#include<cstdlib>
#include "cpn.h"
#include "RG.h"
#include"SBA.h"
#define max_array_num 10000
using namespace std;

/*********************************Global_functions***********************************/
bool judgeF(string s);            //�ж���F����c����
int convert(RG rg, C_Petri ptnet, int i, int j);
int sumtoken(C_Petri ptnet, RG rg, string s, int statenum);         //���������token�ĺ� ��������s��  p1,p2,p3,p4,  ����ʽ 
int localBA(SBA ba, string val);                                                 //����data  �����Զ��� �ڽӱ���±� 
int localRG(RG rg, string val);                                                 //����data  ���ؿɴ�ͼ �ڽӱ���±� 
bool handleFLTLF(C_Petri ptnet, RG rg, string s, int statenum);  //����F����  
void handleLTLCstep(C_Petri ptnet, RG rg, int &front_sum, int &latter_sum, string s, int statenum);//����ǰ�벿�ֺ�;��벿�ֺ� ;����  ��{�ĺ�һ���ַ���ʼ; handleLTLC�Ķ��� 
bool handleLTLC(C_Petri ptnet, RG rg, string s, int statenum);   //����C����  
/*******************************Global_functions End**********************************/

typedef struct Product
{                                      //���Զ�����״̬�ڵ� �� <RGname,BAname> ���� 
	string RGname;           //�ɴ�ͼ�ڵ����� 
	int RGname_id;
	string BAname;           //�Զ����ڵ����� 
	int BAname_id;
	int pro_id;                          //��

};

typedef struct 	T       //��Ǩ��ϵ   ���������Զ�����״̬�ڵ����  
{
	Product s;             //��Ǩ����ʼ�ڵ� 
	Product e;            //��Ǩ��Ŀ��ڵ� 
	//����< �Զ����������
	/*bool operator <(const T &a)const
	{
		if (a.s.RGname != s.RGname)            //����s.RGname����
			return a.s.RGname < s.RGname;

		else if (a.s.BAname != s.BAname)      //s.RGname��ͬ ��s.BAname����
			return a.s.BAname < s.BAname;

		else if (a.e.RGname != e.RGname)     //s.RGname s.BAname����ͬ ����e.RGname����
			return a.e.RGname < e.RGname;

		else if (a.e.BAname != e.BAname)      //���涼����
			return a.e.BAname < e.BAname;

		else if (a.s.id != s.id)
			return a.s.id < s.id;

		else return  a.e.id < e.id;
	}*/
};

typedef struct node   //�洢���Զ�������ʽǰ����
{
	int to;        //to�Ǹõ�ָ����յ� ;
	int next;     //next�ǵ�i����ͬ������һ���ߵĴ洢λ�� 
};

class Product_Automata
{
private:
	vector<Product> status;      //���Զ���״̬����<RGname,BAname> 
	vector<T> transition;           //��Ǩ���� 
	vector<Product> initial_status; //��ʼ״̬����
	vector<Product> isaccept;       //�ɽ���״̬���ϼ���
	vector<int> isAccept_id;           //�ɽ���״̬���ϵ�id�� 
	int *heads;
	int *DFN;
	int *LOW;
	int *stack;
	int *visit;
	node *edge;
	bool result;
public:
	Product_Automata();
	void getProduct(C_Petri ptnet, RG rg, SBA ba);//���ɽ��Զ���
	void addisaccept(SBA ba, int i, Product n);//���ӿɽ���״̬ 
	void addinitial_status(SBA ba, int i, int j, Product n);//���ӳ�ʼ״̬ 
	void addstatus(Product n);//���ӽ��Զ�����״̬ 
	void addtransitionR(RG rg, SBA ba, int i, int j, Product n, ofstream &outfile);
	void addtransitionF(RG rg, SBA ba, int i, int j, Product n, ofstream &outfile);//���ӽ��Զ�����Ǩ��״̬ 
	void tarjan(int x, node *edge, int *LOW, int *DFN, int *stack, int *visit, int *heads);                 //dfs Ѱ��ǿ��ͨ����  ����ڼ������ڴ����ݹ���ǵ㡣
	void ModelChecker(C_Petri ptnet, RG rg, SBA ba, string propertyid);
	void initate_array();
	~Product_Automata();
	//void garbowbfs(int cur, int temper);

};
bool isLabel(C_Petri ptnet, RG rg, SBA ba, int vi, int sj, ofstream &ofe);       //vi�ǿɴ�ͼ���±� sj���Զ������±� 
void add(int x, int y, node *edge, int *heads);             //ͨ��Ǩ�ƹ�ϵ �洢���Զ�����ͼ 


#endif
