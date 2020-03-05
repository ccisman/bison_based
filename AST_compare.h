#pragma once

#include"tree.h"
#include"cpn.h"
#include<vector>
#define minHeight 4



class tree_height
{
public:
	gtree *point;
	int height;
};

class HighIndexList
{
public:
	vector<tree_height> list;

	void order();

	void push(gtree *t);
	vector<gtree *>pop();
	int peekMax();
	void open(gtree *t);
};

class Mapping
{
public:
	gtree *map1;
	gtree *map2;
	int num;
};

class AST_change
{
public:
	Mapping m;
	vector<pair<int, int>> modify, add, del, move;
};


vector<AST_change> extract_change(gtree *t1, gtree *t2, vector<Mapping> &M);

vector<AST_change> compare_AST(gtree *tree1, gtree *tree2);

void top_down(gtree *t1, gtree *t2, vector<Mapping> &M);


string find_P_name(C_Petri petri, string v_name);

string find_P_name_1(C_Petri petri, string v_name, int current);

//string find_T_name(C_Petri petri, string v_name);

string gen_P();

vector<string> evolution(C_Petri &petri, vector<AST_change> changes);

bool exist_in(vector<string> v, string s);

bool exist_in_M(Mapping m, vector<Mapping> M);