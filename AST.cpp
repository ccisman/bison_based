#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<iomanip>
#include<fstream>

#include"tree.h"
#include"cpn.h"
#include"AST_compare.h"
#include"y.tab.h"


extern int yyparse(void);
extern FILE *yyin;
extern gtree* head;
void printtree(gtree* t);

#define keywordNum 20
using namespace std;

int sort_num = 0;
int gen_sel_num, gen_iter_num, gen_jump_num, gen_fun_num;
vector<aka> type_array;//别名表

string gen_sel()
{
	string temp = "";
	return temp + "sel" + to_string(gen_sel_num++);
}

string gen_iter()
{
	string temp = "";
	return temp + "iter" + to_string(gen_iter_num++);
}

string gen_jump()
{
	string temp = "";
	return temp + "jump" + to_string(gen_jump_num++);
}

string gen_fun()
{
	string temp = "";
	return temp + "fun" + to_string(gen_fun_num++);
}

bool judge_assign_statement(gtree *statement1)
{
	if (statement1->type==STATEMENT && statement1->child->type == EXPRESSION_STATEMENT && statement1->child->child->type == EXPRESSION
		&& statement1->child->child->child->type == ASSIGNMENT_EXPRESSION && statement1->child->child->child->child->type != CONDITIONAL_EXPRESSION)
		return true;
	else
		return false;
}

bool judge_call_postfix_expression(gtree *postfix_expression)
{
	if (postfix_expression->type == POSTFIX_EXPRESSION && postfix_expression->child != NULL
		&& postfix_expression->child->type == POSTFIX_EXPRESSION && postfix_expression->child->next != NULL
		&& postfix_expression->child->next->type == REMAIN && postfix_expression->child->next->place == "(")
		return true;
	else
		return false;
}

bool judge_call_statement(gtree *statement1)//判断statement1对应的是函数调用语句
{
	if (statement1->child!=NULL && statement1->child->type == EXPRESSION_STATEMENT && statement1->child->child->type == EXPRESSION 
		&& statement1->child->child->child->type == ASSIGNMENT_EXPRESSION && statement1->child->child->child->child->type == CONDITIONAL_EXPRESSION)
	{
		gtree *temp = statement1;
		while (temp->type != POSTFIX_EXPRESSION)
			temp = temp->child;
		if (judge_call_postfix_expression(temp))
			return true;
	}
	return false;
}

bool judge_label_statement(gtree *statement1)//判断statement1对应的是标签语句
{
	if (statement1->type == STATEMENT && statement1->child->type == LABELED_STATEMENT)
	{
			return true;
	}
	return false;
}

bool judge_return_statement(gtree *statement1)
{
	if (statement1->type == STATEMENT && statement1->child->type == JUMP_STATEMENT && statement1->child->child->type == RETURN)
		return true;
	return false;
}

bool judge_goto_statement(gtree *statement1)
{
	if (statement1->type == STATEMENT && statement1->child->type == JUMP_STATEMENT && statement1->child->child->type == GOTO)
		return true;
	return false;
}

bool judge_break_statement(gtree *statement1)
{
	if (statement1->type == STATEMENT && statement1->child->type == JUMP_STATEMENT && statement1->child->child->type == BREAK)
		return true;
	return false;
}


bool judge_expression_statement(gtree *statement1)
{
	if (statement1->type == STATEMENT && statement1->child->type == EXPRESSION_STATEMENT && statement1->child->child->type == EXPRESSION
		&& statement1->child->child->child->type == ASSIGNMENT_EXPRESSION && statement1->child->child->child->child->type == CONDITIONAL_EXPRESSION
		&& !judge_call_statement(statement1))
		return true;
	return false;
}

bool judge_inside_compound_statement(gtree *statement1)//
{
	gtree *temp = statement1;
	while (temp && temp->type != COMPOUND_STATEMENT)
		temp = temp->parent;
	if (temp==NULL || temp->parent->type == FUNCTION_DEFINITION)
		return false;
	return true;
}

void TraverseTree2(gtree *p)//对表达式进行操作
{
	if (p == NULL)return;
	if (p->type == COMPOUND_STATEMENT)
		p->place = gen_fun();
	TraverseTree2(p->child);
	TraverseTree2(p->next);

	if (p->type == EXPRESSION || p->type == ASSIGNMENT_EXPRESSION || p->type == UNARY_EXPRESSION || p->type == ASSIGNMENT_EXPRESSION || p->type == CONDITIONAL_EXPRESSION
		|| p->type == LOGICAL_OR_EXPRESSION || p->type == LOGICAL_AND_EXPRESSION || p->type == INCLUSIVE_OR_EXPRESSION || p->type == EXCLUSIVE_OR_EXPRESSION
		|| p->type == AND_EXPRESSION || p->type == EQUALITY_EXPRESSION || p->type == RELATIONAL_EXPRESSION || p->type == SHIFT_EXPRESSION || p->type == ADDITIVE_EXPRESSION
		|| p->type == MULTIPLICATIVE_EXPRESSION || p->type == CAST_EXPRESSION || (p->type == POSTFIX_EXPRESSION && !judge_call_postfix_expression(p)) || p->type == PRIMARY_EXPRESSION
		|| p->type == UNARY_OPERATOR || p->type == DECLARATION_SPECIFIERS || p->type == ARGUMENT_EXPRESSION_LIST || p->type == ASSIGNMENT_OPERATOR || p->type == CONSTANT_EXPRESSION)//内部变量声明place需要带有函数前缀
	{
		gtree *p1 = p->child;
		string temp_place = "";
		while (p1)
		{
			temp_place += p1->place;
			p1 = p1->next;
		}
		p->place = temp_place;
	}
	else if (p->type == SELECTION_STATEMENT)
	{
		p->place = gen_sel();
	}
	else if (p->type == ITERATION_STATEMENT)
	{
		p->place = gen_iter();
	}
	else if (p->type == JUMP_STATEMENT)
	{
		p->place = gen_jump();
	}
	else if (p->type == LABELED_STATEMENT)
	{
		gtree *p1 = p->child;
		string temp_place = " ";
		temp_place += p1->place;
		if (p1->next->place != ":")
			temp_place += p1->next->place;
		p->place = temp_place;
		p->parent->place = temp_place;
	}
	else if (p->type == STORAGE_CLASS_SPECIFIER)
	{
		if (p->child->place == "extern")
			p->place = "e";
		else
			p->place = p->child->place;
	}
	else if (p->type == TYPE_SPECIFIER)
	{
		gtree *p1 = p->child;
		if (p->child->place == "signed")
			p->place = 's';
		else if (p->child->place == "unsigned")
			p->place = 'u';
		else if (p->child->place=="int")//int
		{
			p->place = '1';
		}
		else if (p->child->place == "char")//char
		{
			p->place = '2';
		}
		else if (p->child->place == "float")//float
		{
			p->place = '3';
		}
		else if (p->child->place == "double")//double
		{
			p->place = '4';
		}
		else if (p->child->place == "long")//long
		{
			p->place = '5';
		}
		else if (p->child->place == "short")//short
		{
			p->place = '6';
		}
		else if (p->child->place == "void")//void
		{
			p->place = '7';
		}
	}
	else if (p->type == TYPE_QUALIFIER)
	{
		if (p->child->place == "const")
			p->place = "c";
		else if (p->child->place == "volatile")
			p->place = "v";
	}
	else if (p->type == FUNCTION_DEFINITION)//function_definition的place值为函数名
	{
		gtree *temp;
		if (p->child->type == DECLARATION_SPECIFIERS)
			if (p->child->next->child->type == POINTER)
				temp = p->child->next->child->next->child;
			else
				temp = p->child->next->child->child;
		else
			if (p->child->child->type == POINTER)
				temp = p->child->child->next->child;
			else
				temp = p->child->child->child;
		while (temp->type != IDENTIFIER)
			temp = temp->child;
		p->place = temp->place;
	}
	else if (p->type == POSTFIX_EXPRESSION && judge_call_postfix_expression(p))//postfix_expression为函数调用，则place为函数名_v
	{
		if (p->place=="postfix_expression")
			p->place = p->child->child->place + "_v";

		gtree *statement = p;
		while (statement->type != STATEMENT)
			statement = statement->parent;
		statement->contain_call_flag = true;
		statement->contain_call = p->child->child->place;
		/*gtree *assignment_expression = NULL , *statement = p;
		while (statement->type != STATEMENT)
		{
			if (statement->type == ASSIGNMENT_EXPRESSION && assignment_expression == NULL)
				assignment_expression = statement;
			statement = statement->parent;
		}
		if (assignment_expression->parent->type != ASSIGNMENT_EXPRESSION)
			p->place = p->child->child->place + "_call";
		else
			p->place = p->child->child->place + "_v";
		statement->place = p->place;*/
	}
	else if (p->type == STATEMENT)
	{
		if (p->child->type == SELECTION_STATEMENT || p->child->type == ITERATION_STATEMENT)
			p->place = p->child->place;
		else if (p->child->type == EXPRESSION_STATEMENT)
		{
			if (judge_assign_statement(p))
				p->place = p->child->child->child->place;
			else if (judge_call_statement(p))
			{
				gtree *temp = p, *temp_postfix_expression = NULL;
				while (temp->type != PRIMARY_EXPRESSION)
				{
					if (temp->type == POSTFIX_EXPRESSION && temp_postfix_expression == NULL)
						temp_postfix_expression = temp;
					temp = temp->child;
				}
				p->place = temp->place + "_call";
				temp_postfix_expression->place = temp->place + "_call";
			}
			else
				p->place = p->child->child->child->place;
		}
		else if (p->child->type == JUMP_STATEMENT)
			p->place = p->child->place;
	}
	else if (p->type == INIT_DECLARATOR)
	{
		if (p->child->child->type == POINTER)
			p->place = p->child->child->next->place;
		else
			p->place = p->child->child->place;
	}
	else if (p->type == DIRECT_DECLARATOR)
	{
		gtree *id = p;
		while (id->type != IDENTIFIER)
			id = id->child;
		p->place = id->place;
	}
	return;
}
void TraverseTree3(gtree *p)
{
	if (p == NULL)
		return;
	p->num = sort_num++;

	TraverseTree3(p->child);
	TraverseTree3(p->next);
}

void Traverse(gtree *p);

void reset_gen_ast()
{
	gen_fun_num = 0;
	gen_iter_num = 0;
	gen_jump_num = 0;
	gen_sel_num = 0;
	sort_num = 0;
}

gtree *&create_tree(string filename, bool pre_process_flag)//flag为true代表需要预处理
{
	reset_gen_ast();
	if (pre_process_flag == true)
	{
		//string filename = "a.txt";
		string filename1 = "z-" + filename;
		string whole_filename1 = newfile_dirname + filename1;
		ifstream fin;
		ofstream fout;
		fin.open(origin_dirname + filename, ios::in);
		if (!fin.is_open())
		{
			cout << filename << "文件不存在" << endl;
			exit(1);
		}
		string temp_s;
		istreambuf_iterator<char> beg(fin), end;
		temp_s = string(beg, end);
		fin.close();
		pre_process(temp_s);
		fout.open(whole_filename1, ios::out);
		/*if (!fout.is_open())
		{
			cout << filename << "文件不存在" << endl;
			exit(1);
		}*/
		fout << temp_s;
		fout.close();

		FILE *fp = fopen(whole_filename1.c_str(), "r");
		if (fp)
			yyin = fp;
		else
		{
			cout << filename1 << "文件不存在" << endl;
			exit(1);
		}

		
	}
	else
	{
		FILE *fp = fopen(filename.c_str(), "r");
		if (fp)
			yyin = fp;
		else
		{
			cout << filename << "文件不存在" << endl;
			exit(1);
		}
	}
	yyparse();
	printtree(head);
	//if (flag == true)
		TraverseTree2(head);
	TraverseTree3(head);
	//Traverse(head);

	return head;
}

void Traverse(gtree *p)//测试函数
{
	if (p == NULL) return;
	if (p->type == FUNCTION_DEFINITION)
		cout << p->place << endl;
	Traverse(p->child);
	Traverse(p->next);
}

vector<AST_change> compare_AST(gtree *tree1, gtree *tree2)
{



	vector<Mapping> M;
	top_down(tree1, tree2, M);
	for (unsigned int i = 0; i < M.size(); i++)
	{

		if (M[i].map1->type == STATEMENT_LIST && M[i].map1->parent->type == STATEMENT_LIST)
		{
			Mapping temp_m;
			temp_m.map1 = M[i].map1->child;
			temp_m.map2 = M[i].map2->child;
			M.push_back(temp_m);
		}
		if (M[i].map1->type == DECLARATION_LIST && M[i].map1->parent->type == DECLARATION_LIST)
		{
			Mapping temp_m;
			temp_m.map1 = M[i].map1->child;
			temp_m.map2 = M[i].map2->child;
			M.push_back(temp_m);
		}
	}
	vector<AST_change> changes = extract_change(tree1, tree2, M);
	//	cout << Equal_Tree(tree1, tree2);
	//	Traverse(tree2);
	return changes;
}