#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<stdio.h>  
#include<string>
#include<iomanip>
#include<fstream>
#include<stack>
#include<sstream>
#include<vector>
#include<time.h>
#include<io.h>

#include"tree.h"
#include"cpn.h"
#include"RG.h"
#include"AST_compare.h"

string rg_dirname = ".\\rg\\";
string rg_sliceOnly_dirname = ".\\rg_sliceOnly\\";
string origin_dirname = "D:\\学习资料\\项目资料\\petri建模\\test7\\";
string newfile_dirname = ".\\newfile\\";

#define keywordNum 20

using namespace std;
int tb_num = 0;
int nextnum = 100;//标号(从100开始)
int labelnum = 1;
int labelnum1 = 1;


void SplitString(const string& s, vector<string>& v, const string& c)
{
	string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}
void splitExpression(string &s, vector<string>& v)//传入字符串和结果集合，将字符串根据>,<等分割并放入结果集
{
	string s1;
	bool flag_shut = false;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == '(' || s[i] == ')')
			continue;
		else if (s[i] == '#')
			flag_shut = true;
		if (s[i] == '<' || s[i] == '>')
		{
			if (s[i + 1] == '=')
				i++;
			
			v.push_back(s1);
			s1.clear();
			flag_shut = false;
		}
		else if (s[i] == '=')
		{
			v.push_back(s1);
			s1.clear();
			flag_shut = false;
		}
		else if (s[i] == '|')
		{
			if (s[i + 1] == '|')
				i++;
			v.push_back(s1);
			s1.clear();
			flag_shut = false;
		}
		else if (s[i] == '&')
		{
			if (s[i + 1] == '&')
				i++;
			v.push_back(s1);
			s1.clear();
			flag_shut = false;
		}
		else if (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/')
		{
			v.push_back(s1);
			s1.clear();
			flag_shut = false;
		}
		else if (s[i] == '!' || s[i] == '=')
		{
			if (s[i + 1] == '=')
				i++;
			//else
				//cout << "出现异常!=或==" << endl;
			v.push_back(s1);
			s1.clear();
			flag_shut = false;
		}
		else if (s[i] == '%')
		{
			v.push_back(s1);
			s1.clear();
			flag_shut = false;
		}
		else if (flag_shut == false)
			s1 = s1 + s[i];
	}

	v.push_back(s1);
}


int string_replace(string &s1, const string &s2, const string &s3)//在s1中找到s2字串，替换成s3
{
	string::size_type pos = 0;
	string::size_type a = s2.size();
	string::size_type b = s3.size();
	while ((pos = s1.find(s2, pos)) != string::npos)
	{
		s1.replace(pos, a, s3);
		pos += b;
	}
	return 0;
}


int find_T_exist(vector<Transition> transition, string s)//找库所中是否有v_name等于s的，并且返回有几个
{
	vector<string> v;
	for (int i = transition.size() - 1; i >= 0; i--)
	{
		if (transition[i].booleanExpression == "")
			SplitString(transition[i].v_Expression, v, "#");
		else
			SplitString(transition[i].booleanExpression, v, "#");
		if (v[0] == s)
		{
			if (v.size() == 1)
			{
				return 1;
			}
			else
			{
				return atoi(v[1].c_str()) + 1;

			}
		}
	}
	return 0;
}

void travel_tree(gtree *tree,ofstream &out,vector<pair<string,int>> &list)
{

	if (tree == NULL)
		return;
	string parent_place, child_place;
	int parent_no, child_no;
	if (tree->parent != NULL)
	{
		
		parent_place = tree->parent->place;
		parent_no = tree->parent->num;
		child_place = tree->place;
		child_no = tree->num;
		out << "node" << to_string(parent_no) << "[label=\"" << parent_place << "\"]" << endl;
		out << "node" << to_string(child_no) << "[label=\"" << child_place << "\"]" << endl;
		out << "node" << to_string(parent_no) << "->" << "node" << to_string(child_no) << ";" << endl;

	}
	travel_tree(tree->child, out, list);
	travel_tree(tree->next, out, list);


	//out.close();
}

void intofile_tree(gtree *tree)
{
	ofstream out;
	vector<pair<string,int>> list;
	out.open("tree.dot", ios::out);

	out << "digraph G{" << endl;
	travel_tree(tree, out, list);
	out << "}" << endl;
	out.close();
	
}





//evolution部分结束
//********************//
//********************//

bool exist_in(vector<string> v, string s);


void initial_changeAnalyse_cpn(C_Petri &petri1, C_Petri &petri, vector<string> change_P, vector<string> change_T, vector<Arc> &change_Arc);


void onlybuildCPN(gtree *tree,string new_filename, C_Petri &petri)
{
	reset_gen_cpn();
	Place::total_num = 0;
	Transition::total_num = 0;
	Arc::total_num = 0;
	clock_t start, finish;


	//************************生成cpn
	create_CPN(petri, tree);

	//************************输出cpn
	output_CPN(petri, "output");
}

void get_names(string dirname, vector<string> &filelist)
{
	struct _finddata_t fa;
	long fHandle;
	if ((fHandle = _findfirst(dirname.c_str(), &fa)) == -1L)//这里可以改成需要的目录 
	{
		printf("当前目录下没有new.c文件\n");
		return;
	}
	else
		do
		{
			//if (strstr(fa.name, "new.c") != NULL)
			if (strstr(fa.name, "new.c") != NULL)
				filelist.push_back(fa.name);
		} while (_findnext(fHandle, &fa) == 0);

}

vector<string> get_criteria(C_Petri petri, string filename)
{
	vector<string> result;
	string xml = filename;
	xml.replace(xml.find("new.c"), 5, "LTL.xml");

	char xml1[100], xml2[100];
	strcpy(xml1, xml.c_str());
	xml.replace(xml.find("LTL.xml"), 7, "LTL1.xml");
	strcpy(xml2, xml.c_str());

	xml_trans_C(petri, xml1, xml2, result);
	for (int i = 0; i < petri.p_num; i++)
	{
		string temp = petri.place[i].v_name;
		if (temp == "main begin")
		{
			result.push_back(petri.place[i].name);
			break;
		}
	}
	return result;
}

//功能：构建CPN以及生成可达图
//输入：语法树tree
//输出：通过引用形参传出，CPN和可达图
void DirectBuild(gtree *tree, string new_filename, C_Petri &petri, RG &rg)
{
	reset_gen_cpn();
	Place::total_num = 0;
	Transition::total_num = 0;
	Arc::total_num = 0;
	clock_t start, finish;


	//************************生成cpn
	create_CPN(petri, tree);

	//************************输出cpn
	output_CPN(petri, "output");

	//************************生成可达图
	vector<string> v;
	//RG rg(petri); //定义可达图
	rg.init_RG(petri);
	create_RG(rg);
	print_RG(rg, rg_dirname + new_filename);
}

//功能：输出语法树
//输入：源程序名
//输出：将语法树以png图片格式放在tree.png中
void out_tree(string filename)
{
	gtree *tree;
	C_Petri petri;
	RG rg;
	tree = create_tree(filename, false);
	intofile_tree(tree);
	makeGraph("tree.dot", "tree.png");
}

//功能：对源程序和变化后的源程序进行变化影响分析，
//		将对源程序构建CPN以及可达图，并进行变化检测、模型修改、变化影响分析
//输入：源程序和变化后程序名
//输出：将直接建模以及变化影响分析生成的可达图放在rg文件夹下
//		并在program_time.txt文件中记录运行时间、可达图节点数、CPN中库所变迁数等
void compare(string filename, string new_filename)
{
	ofstream out;
	out.open("program(time).txt", ios::out | ios::app);
	if (out.fail())
	{
		cout << "open program(time) failed!" << endl;
		exit(1);
	}
	out << "旧文件：" << filename << "\t" << "新文件：" << new_filename << endl;
	clock_t start, finish, temp;

	gtree *tree1 = create_tree(filename,true);
	//gtree *tree1_noexecute = create_tree(filename, true);
	//intofile_tree(tree1);
	gtree *tree2 = create_tree(new_filename, true);
	//gtree *tree2_noexecute = create_tree(new_filename, true);
	C_Petri petri, petri_new;
	RG rg, rg_new;
	start = clock();
	DirectBuild(tree2,  new_filename, petri_new, rg_new);
	
	//************************模型检测
	//model_check(petri_new, rg_new);
	finish = clock();
	out << "可达图节点个数：" << rg_new.rgnode.size() << endl;
	out << "库所个数:" << rg_new.petri.p_num << endl;
	out << "变迁个数:" << rg_new.petri.t_num << endl;
	out << "直接构建时间：" << (finish - start) / 1000.0 << "秒" << endl;
	rg_new.release();
	petri_new.release();
	//************************直接构建结束
	execute_flag = false;
	DirectBuild(tree1, filename, petri, rg);


	C_Petri petri_afterevolution;
	
	//************************转换LTL公式
	//char originXMLfile[50] = "formulaC.xml";
	//char desXMLfile[50] = "NewFormulaC.xml";
	//xml_trans_C(petri, originXMLfile, desXMLfile);


	vector<string> change_places;
	vector<AST_change> changes;
	string main_begin;
	C_Petri petri1;
	//不带执行弧的结果
	start = clock();

	changes = compare_AST(tree1, tree2);
	

	change_places = evolution(petri, changes);


	//************************变化影响分析部分

	main_begin = find_P_name(petri, "main begin");
	change_places.push_back(main_begin);

	temp = clock() - start;

	petri_afterevolution.arc = petri.arc;
	petri_afterevolution.arcnum = petri.arcnum;
	petri_afterevolution.place = petri.place;
	petri_afterevolution.p_num = petri.p_num;
	petri_afterevolution.transition = petri.transition;
	petri_afterevolution.t_num = petri.t_num;
	
	//petri1 = changeAnalyse(petri, change_places);
	//
	//output_CPN(petri1, "output");
	//
	//RG rg_noexecute(petri1); //定义可达图
	//create_RG(rg_noexecute);
	//print_RG(rg_noexecute, rg_dirname + "changeAnalyse-" + filename);
	////model_check(petri1, rg_noexecute);
	//finish = clock();
	//out << "可达图节点个数：" << rg_noexecute.rgnode.size() << endl;
	//out << "库所个数:" << rg_noexecute.petri.p_num << endl;
	//out << "变迁个数:" << rg_noexecute.petri.t_num << endl;
	//out << "不带执行弧变化影响分析时间：" << (finish - start + temp) / 1000.0 << "秒" << endl;

	//带执行弧的变化影响分析
	execute_flag = true;
	//************************模型演化部分
	
	//	vector<string> array_this_sentence, array_this_sentence_after, array_father_sentence, array_last_sentence, array_next_sentence, array_next_father_sentence, array_next_next_sentence;

	start = clock();

	//changes = compare_AST(tree1, tree2);


	//change_places = evolution(petri, changes);

	////output_CPN(petri, "output");

	////RG rg1(petri); //定义可达图
	////create_RG(rg1);
	////print_RG(rg1);


	////************************变化影响分析部分

	//main_begin = find_P_name(petri, "main begin");
	//change_places.push_back(main_begin);

	petri1 = changeAnalyse(petri_afterevolution, change_places);
	
	output_CPN(petri1, "output");

	RG rg2(petri1); //定义可达图
	create_RG(rg2);
	print_RG(rg2, rg_dirname + "changeAnalyse-" + filename);
	//model_check(petri1, rg2);
	

	finish = clock();
	out << "可达图节点个数：" << rg2.rgnode.size() << endl;
	out << "库所个数:" << rg2.petri.p_num << endl;
	out << "变迁个数:" << rg2.petri.t_num << endl;
	out << "变化影响分析时间：" << (finish - start + temp) / 1000.0 << "秒" << endl;
	out << endl;
	rg2.release();
	petri.release();

	out.close();
}

//功能：对比源程序直接建模、普通切片以及加入执行弧后切片的结果
//		切片的初始库所集合通过变化检测中提取的库所确定
//输入：源程序和变化后程序名
//输出：将直接建模以及变化影响分析生成的可达图放在rg_sliceOnly文件夹下
//		并在program_time.txt文件中记录运行时间、可达图节点数、CPN中库所变迁数等
void compare1(string filename, string new_filename)
{
	ofstream out;
	out.open("program(time).txt", ios::out | ios::app);
	if (out.fail())
	{
		cout << "open program(time) failed!" << endl;
		exit(1);
	}
	out << "旧文件：" << filename << "\t" << "新文件：" << new_filename << endl;
	clock_t start, finish, temp, build_time;

	gtree *tree1 = create_tree(filename, true);
	//gtree *tree1_noexecute = create_tree(filename, true);
	//intofile_tree(tree1);
	gtree *tree2 = create_tree(new_filename, true);
	//gtree *tree2_noexecute = create_tree(new_filename, true);
	C_Petri petri, petri_new;
	RG rg, rg_new;
	start = clock();
	//DirectBuild(tree2, new_filename, petri_new, rg_new);
	onlybuildCPN(tree2, new_filename, petri_new);

	build_time = clock() - start;

	rg_new.init_RG(petri_new);

	create_RG(rg_new);
	print_RG(rg_new, rg_sliceOnly_dirname + new_filename);

	//************************模型检测
	//model_check(petri_new, rg_new,);
	finish = clock();

	out << "可达图节点个数：" << rg_new.rgnode.size() << endl;
	out << "库所个数:" << rg_new.petri.p_num << endl;
	out << "变迁个数:" << rg_new.petri.t_num << endl;
	out << "直接构建时间：" << (finish - start) / 1000.0 << "秒" << endl;
	rg_new.release();
	//petri_new.release();
	//************************直接构建结束
	execute_flag = false;
	//DirectBuild(tree2, filename, petri, rg);

	C_Petri petri_new1, petri_new2;
	/*petri_afterevolution.arc = petri_new.arc;
	petri_afterevolution.arcnum = petri_new.arcnum;
	petri_afterevolution.place = petri_new.place;
	petri_afterevolution.p_num = petri_new.p_num;
	petri_afterevolution.transition = petri_new.transition;
	petri_afterevolution.t_num = petri_new.t_num;*/
	petri_new1 = petri_new;

	/*petri_new1.arc = petri_new.arc;
	petri_new1.arcnum = petri_new.arcnum;
	petri_new1.place = petri_new.place;
	petri_new1.p_num = petri_new.p_num;
	petri_new1.transition = petri_new.transition;
	petri_new1.t_num = petri_new.t_num;*/
	petri_new2 = petri_new;
	

	//************************转换LTL公式
	//char originXMLfile[50] = "formulaC.xml";
	//char desXMLfile[50] = "NewFormulaC.xml";
	//xml_trans_C(petri, originXMLfile, desXMLfile);


	vector<string> change_places;
	vector<AST_change> changes;
	string main_begin;
	C_Petri petri1;
	//不带执行弧的结果
	

	changes = compare_AST(tree2, tree1);
	//adjust_del(changes);

	change_places = evolution(petri_new, changes);


	main_begin = find_P_name(petri_new, "main begin");
	change_places.push_back(main_begin);

	//temp = clock() - start;

	

	start = clock();

	petri1 = changeAnalyse(petri_new1, change_places);

	output_CPN(petri1, "output");

	RG rg_noexecute(petri1); //定义可达图
	create_RG(rg_noexecute);
	print_RG(rg_noexecute, rg_sliceOnly_dirname + "changeAnalyseNE-" + filename);
	//model_check(petri1, rg_noexecute);
	finish = clock();
	out << "可达图节点个数：" << rg_noexecute.rgnode.size() << endl;
	out << "库所个数:" << rg_noexecute.petri.p_num << endl;
	out << "变迁个数:" << rg_noexecute.petri.t_num << endl;
	out << "不带执行弧变化影响分析时间：" << (finish - start + build_time) / 1000.0 << "秒" << endl;

	//带执行弧的变化影响分析
	execute_flag = true;

	start = clock();
	petri1 = changeAnalyse(petri_new2, change_places);

	output_CPN(petri1, "output");

	RG rg_executed(petri1); //定义可达图
	create_RG(rg_executed);
	print_RG(rg_executed, rg_sliceOnly_dirname + "changeAnalyseE-" + filename);
	//model_check(petri1, rg_executed);
	finish = clock();
	out << "可达图节点个数：" << rg_executed.rgnode.size() << endl;
	out << "库所个数:" << rg_executed.petri.p_num << endl;
	out << "变迁个数:" << rg_executed.petri.t_num << endl;
	out << "变化影响分析时间：" << (finish - start + build_time) / 1000.0 << "秒" << endl;
	out << endl;


	out.close();
}

int main()
{
	//out_tree("1.txt");
	vector<string> filelist;

	string filename, new_filename;
	get_names(origin_dirname + "*", filelist);
	//compare("bor&zero_sum.c", "bor&zero_sum - new.c");
	for (unsigned int i = 0; i < filelist.size(); i++)
	{
		new_filename = filelist[i];
		filename = new_filename;
		string_replace(filename, " - new", "");
		compare(filename, new_filename);
	}




}

