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


#define keywordNum 20

using namespace std;
int tb_num = 0;
int nextnum = 100;//标号(从100开始)
int labelnum = 1;
int labelnum1 = 1;



//string last_sentence = "", last_func = "";
//C_Petri petri;

void emit(string op, string arg1, string arg2, string result, int num, int flag)
{
	if (flag == 0)
		cout << num << ":(" << op << "  ," << arg1 << "  ," << arg2 << "  ," << result << ')' << endl;
	else if (flag == 1)
		cout << num << ": " << "  if " << arg1 << "  " << op << "  " << arg2 << "  goto " << result << endl;
	else if (flag == 2)
		cout << num << ": " << "goto " << result << endl;
	else
	{
	};
	nextnum++;

	ofstream out;
	out.open("out.txt", ios::app);
	if (flag == 0)
		out << num << ":(" << op << "  ," << arg1 << "  ," << arg2 << "  ," << result << ')' << endl;
	else if (flag == 1)
		out << num << ": " << "  if " << arg1 << "  " << op << "  " << arg2 << "  goto " << result << endl;
	else if (flag == 2)
		out << num << ": " << "goto " << result << endl;
	else
	{
	};
	out.close();
}





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
	//ofstream out;
	//out.open("tree.dot", ios::out);

	//out << "digraph G{" << endl;
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
		/*for (unsigned int i = 0; i < list.size(); i++)
		{
			if (list[i].first == parent)
			{
				list[i].second++;
				newparent = parent + to_string(list[i].second);
				break;
			}
		}
		if (newparent == "")
		{
			newparent = parent;
			list.push_back(make_pair(newparent,0));
		}
		for (unsigned int i = 0; i < list.size(); i++)
		{
			if (list[i].first == child)
			{
				list[i].second++;
				newchild = child + to_string(list[i].second);
				break;
			}
		}
		if (newchild == "")
		{
			newchild = child;
			list.push_back(make_pair(newchild, 0));
		}

		out << newparent << "->" << newchild << ";" << endl;
		*/
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

void intofile(C_Petri petri)
{
	ofstream out;
	out.open("output.txt", ios::out);
	//out << "Place:" << endl;
	//out << "-----------------------------------" << endl;

	string fillcolor = "chartreuse";
	for (int i = 0; i < petri.p_num; i++)
	{
		if (petri.place[i].controlP == false)
			out << "subgraph cluster_" << petri.place[i].name << "{label=\"" <<
			petri.place[i].v_name << "\"color=\"white\"" << petri.place[i].name <<
			"[shape=circle, style=\"filled\",color=\"black\",fillcolor=\"" << fillcolor << "\"]}" << endl;
		else
		{
			out << petri.place[i].name << "[shape=circle," << "label=\"" << petri.place[i].v_name << "\"]" << endl;
		}
	}
	//out << "-----------------------------------" << endl; 
	//out << "Transition:" << endl;
	//out << "-----------------------------------" << endl;
	for (int i = 0; i < petri.t_num; i++)
	{
		out << petri.transition[i].name << "[shape=box]" << endl;
	}
	//out << "-----------------------------------" << endl;
	//out << "Arc:" << endl;
	//out << "-----------------------------------" << endl;

	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].V != "#" && petri.arc[i].V != "executed#" && petri.arc[i].V != "executed")//隐式弧
			out << "{" << petri.arc[i].source << "," << petri.arc[i].target << "}" << endl;
		else if (petri.arc[i].V == "executed" || petri.arc[i].V == "relation")
			out << "{" << petri.arc[i].source << "," << petri.arc[i].target << "[style=\"dashed\"]}" << endl;
	}
	out.close();
}


void readGraph(string input, string output) //.txt 转 .dot
{

	const char* in = input.data();
	const char* ou = output.data();

	ifstream fin;
	fin.open(in, ios::in);

	ofstream fout;
	fout.open(ou, ios::out);

	fout << "digraph G{" << endl << "rankdir = LR" << endl;
	string s;
	while (getline(fin, s))
	{
		if (s[0] != '{') {
			fout << s << '\n';
			continue;
		}

		string u, v, lable;
		int n = s.length();
		int i = 1;
		//cout << s << n << endl;
		while (s[i] != ',') i++;
		//cout << i << endl;
		u += s.substr(1, i - 1);
		//cout << u << endl;
		int j = n - 2;
		while (s[j] != ',') j--;
		//cout << j << endl;
		v += s.substr(j + 1, n - 1 - j - 1);
		//cout << v << endl;
		//lable = s.substr(i + 1, j - i - 1);

		string edge = "";
		edge += u;
		edge += "->";
		edge += v;
		//edge += "[label=\"";
		//edge += lable;
		//edge += "\"];";
		fout << edge << endl;
	}
	fout << "}" << endl;
	fin.close();
	fout.close();
}

void makeGraph(string inputname, string outputname) //生成png图片
{
	string s = "";
	s += "dot -Tpng ";
	s += inputname;
	s += " -o ";
	s += outputname;
	const char* cmd = s.data();
	const char* iname = inputname.data();
	system(cmd);
}

void create_RG(RG &rg)
{
	stack<int> newNode;
	newNode.push(0);
	while (!newNode.empty())
	{
		int node_id = newNode.top();
		newNode.pop();
		rg.add_next(node_id, newNode);
	}
}

void print_RG(RG rg, string filename)
{
	ofstream fout;
	fout.open(filename, ios::out);
	fout << "可达图节点共有" << rg.node_num << "个" << endl;
	fout << "如下所示" << endl;
	fout << endl;
	for (int i = 0; i < rg.node_num; i++)
	{
		//cout << i << endl;
		fout << i << endl;
		for (unsigned int j = 0; j < rg.rgnode[i].m.size(); j++)
		{
			int n_n = rg.rgnode[i].m[j].n_n;
			//cout << "(" << rg.petri.place[j].v_name << "," << rg.rgnode[i].m[j].token_num << ",";
			fout << "(" << rg.petri.place[j].v_name << "," << rg.rgnode[i].m[j].token_num << ",";
			for (int k = 0; k < n_n; k++)
			{
				//cout << rg.rgnode[i].m[j].n[k] << " ";
				fout << rg.rgnode[i].m[j].n[k] << " ";
			}
			//cout << ")    ";
			fout << ")    ";
		}
		//cout << endl;
		fout << endl;
		//cout << "后继节点:";
		fout << "后继节点:";
		for (unsigned int j = 0; j < rg.rgnode[i].next.size(); j++)
		{
			//cout << rg.rgnode[i].next[j].num << "    经过变迁:" << rg.rgnode[i].next[j].T << "    ";
			fout << rg.rgnode[i].next[j].num << "    经过变迁:" << rg.rgnode[i].next[j].T << "    ";
		}
		//cout << endl;
		fout << endl;
	}

	fout.close();
}


//evolution部分结束
//********************//
//********************//

bool exist_in(vector<string> v, string s);

void initializing(C_Petri &petri)//初始化petri网，main_begin赋上token，连接main_begin和main_v
{
	///初始给main的token赋值
	for (int i = 0; i < petri.p_num; i++)
	{
		if (petri.place[i].v_name == "main begin")
		{
			petri.place[i].token_num = 1;
			break;
		}
	}

	string main_v = find_P_name(petri, "main_v");
	vector<string> main_exit_T = petri.get_exit(find_P_name(petri, "main begin"));
	for (unsigned int i = 0; i < main_exit_T.size(); i++)
		petri.Add_Arc(main_exit_T[i], main_v, "", false);
}

void initial_changeAnalyse_cpn(C_Petri &petri1, C_Petri &petri, vector<string> change_P, vector<string> change_T, vector<Arc> &change_Arc);

void create_CPN(C_Petri &petri, gtree *tree)
{
	ast_to_cpn(petri, tree, 0);
	process_label(petri);
	initializing(petri);
}

void output_CPN(C_Petri petri, string filePrefix)
{
	intofile(petri);

	readGraph(filePrefix + ".txt", filePrefix + ".dot");
	makeGraph(filePrefix + ".dot", filePrefix + ".png");
}

void DirectBuild(gtree *tree, string new_filename, C_Petri &petri, RG &rg)
{
	reset_gen_cpn();
	Place::total_num = 0;
	Transition::total_num = 0;
	Arc::total_num = 0;
	clock_t start, finish;
	//**************************************************
	//C_Petri petri;
	/*char content[MAX_LENGTH];
	ifstream fin;
	fin.open(new_filename, ios::in);
	if (!fin.is_open())
	{
		cout << "open failed!" << endl;
	}
	fin.read(content, MAX_LENGTH);
	fin.close();*/

	//************************创建语法树
	//gtree *tree = create_tree(new_filename);

	//************************生成cpn
	create_CPN(petri, tree);

	//************************输出cpn
	output_CPN(petri, "output");

	//************************生成可达图
	vector<string> v;
	//RG rg(petri); //定义可达图
	rg.init_RG(petri);
	create_RG(rg);
	print_RG(rg, dirname + new_filename);
}

void get_names(string dirname, vector<string> &filelist)
{
	struct _finddata_t fa;
	long fHandle;
	if ((fHandle = _findfirst(dirname.c_str(), &fa)) == -1L)//这里可以改成需要的目录 
	{
		printf("当前目录下没有txt文件\n");
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
	clock_t start, finish;

	gtree *tree1 = create_tree(filename,true);
	//intofile_tree(tree1);
	gtree *tree2 = create_tree(new_filename, true);
	C_Petri petri, petri_new;
	RG rg, rg_new;
	start = clock();
	DirectBuild(tree2,  new_filename, petri_new, rg_new);
	
	//************************模型检测
	model_check(petri_new, rg_new);
	finish = clock();

	out << "直接构建时间：" << (finish - start) << endl;
	rg_new.release();
	petri_new.release();
	//************************直接构建结束

	DirectBuild(tree1, filename, petri, rg);

	//************************转换LTL公式
	//char originXMLfile[50] = "formulaC.xml";
	//char desXMLfile[50] = "NewFormulaC.xml";
	//xml_trans_C(petri, originXMLfile, desXMLfile);





	//************************模型演化部分
	vector<string> change_places;
	//	vector<string> array_this_sentence, array_this_sentence_after, array_father_sentence, array_last_sentence, array_next_sentence, array_next_father_sentence, array_next_next_sentence;
	vector<int> array_flag;
	start = clock();

	

	vector<AST_change> changes = compare_AST(tree1, tree2);

	start = clock();
	change_places = evolution(petri, changes);

	//output_CPN(petri, "output");

	//RG rg1(petri); //定义可达图
	//create_RG(rg1);
	//print_RG(rg1);


	//************************变化影响分析部分

	string main_begin = find_P_name(petri, "main begin");
	change_places.push_back(main_begin);

	C_Petri petri1;
	petri1 = changeAnalyse(petri, change_places);
	
	output_CPN(petri1, "output");

	RG rg2(petri1); //定义可达图
	create_RG(rg2);
	print_RG(rg2, dirname + "changeAnalyse-" + filename);
	model_check(petri1, rg2);
	finish = clock();

	out << "变化影响分析时间：" << (finish - start) << endl;
	out << endl;
	out.close();
}



int main()
{
	//gtree *tree;
	//C_Petri petri;
	//RG rg;
	//tree = create_tree("simple//switch.txt", false);
	////DirectBuild(tree, "just_finish.txt", petri, rg);
	//intofile_tree(tree);
	//makeGraph("tree.dot", "tree.png");

	
	/*gtree *tree;
	C_Petri petri;
	tree = create_tree("simple.txt", true);
	intofile_tree(tree);
	makeGraph("tree.dot", "tree.png");
	create_CPN(petri, tree);
	output_CPN(petri, "output");
	RG rg;
	rg.init_RG(petri);
	create_RG(rg);
	print_RG(rg, "rg//simple.txt");*/

	vector<string> filelist;

	string filename, new_filename;
	get_names(origin_dirname + "*", filelist);
	//compare("goto.txt", "goto - new.txt");
	for (unsigned int i = 0; i < filelist.size(); i++)
	{
		new_filename = filelist[i];
		filename = new_filename;
		string_replace(filename, " - new", "");
		compare(filename, new_filename);
	}
	//get_names(origin_dirname + "*", filelist);
	//ifstream out;
	//for (unsigned int i = 0; i < filelist.size(); i++)
	//{
	//	create_tree(origin_dirname + filelist[i]);
	//	cout << filelist[i] << endl;
	//}



}

