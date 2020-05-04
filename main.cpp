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
int nextnum = 100;//���(��100��ʼ)
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
void splitExpression(string &s, vector<string>& v)//�����ַ����ͽ�����ϣ����ַ�������>,<�ȷָ��������
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
				//cout << "�����쳣!=��==" << endl;
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


int string_replace(string &s1, const string &s2, const string &s3)//��s1���ҵ�s2�ִ����滻��s3
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


int find_T_exist(vector<Transition> transition, string s)//�ҿ������Ƿ���v_name����s�ģ����ҷ����м���
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
		if (petri.arc[i].V != "#" && petri.arc[i].V != "executed#" && petri.arc[i].V != "executed")//��ʽ��
			out << "{" << petri.arc[i].source << "," << petri.arc[i].target << "}" << endl;
		else if (petri.arc[i].V == "executed" || petri.arc[i].V == "relation")
			out << "{" << petri.arc[i].source << "," << petri.arc[i].target << "[style=\"dashed\"]}" << endl;
	}
	out.close();
}


void readGraph(string input, string output) //.txt ת .dot
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

void makeGraph(string inputname, string outputname) //����pngͼƬ
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
	fout << "�ɴ�ͼ�ڵ㹲��" << rg.node_num << "��" << endl;
	fout << "������ʾ" << endl;
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
		//cout << "��̽ڵ�:";
		fout << "��̽ڵ�:";
		for (unsigned int j = 0; j < rg.rgnode[i].next.size(); j++)
		{
			//cout << rg.rgnode[i].next[j].num << "    ������Ǩ:" << rg.rgnode[i].next[j].T << "    ";
			fout << rg.rgnode[i].next[j].num << "    ������Ǩ:" << rg.rgnode[i].next[j].T << "    ";
		}
		//cout << endl;
		fout << endl;
	}

	fout.close();
}


//evolution���ֽ���
//********************//
//********************//

bool exist_in(vector<string> v, string s);

void initializing(C_Petri &petri)//��ʼ��petri����main_begin����token������main_begin��main_v
{
	///��ʼ��main��token��ֵ
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

void onlybuildCPN(gtree *tree,string new_filename, C_Petri &petri)
{
	reset_gen_cpn();
	Place::total_num = 0;
	Transition::total_num = 0;
	Arc::total_num = 0;
	clock_t start, finish;


	//************************����cpn
	create_CPN(petri, tree);

	//************************���cpn
	output_CPN(petri, "output");
}

void get_names(string dirname, vector<string> &filelist)
{
	struct _finddata_t fa;
	long fHandle;
	if ((fHandle = _findfirst(dirname.c_str(), &fa)) == -1L)//������Ըĳ���Ҫ��Ŀ¼ 
	{
		printf("��ǰĿ¼��û��txt�ļ�\n");
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

//void adjust_del(vector<AST_change> &changes)
//{
//	for (unsigned int i = 0; i < changes.size(); i++)
//	{
//		for (unsigned int j = 0; j < changes[i].move.size(); j++)
//		{
//
//		}
//		for (unsigned int j = 0; j < changes[i].add.size(); j++)
//		{
//			for (unsigned int k = 0; k < changes[i].del.size(); k++)
//			{
//				if (changes[i].del[k].first > changes[i].add[j].first)
//					changes[i].del[k].first++;
//			}
//		}
//		for (unsigned int j = 0; j < changes[i].del.size(); j++)
//		{
//			for (unsigned int k = 0; k < changes[i].del.size(); k++)
//			{
//				if (changes[i].del[k].first > changes[i].del[j].first)
//					changes[i].del[k].first--;
//			}
//		}
//		
//
//	}
//}

//���ܣ�����CPN�Լ����ɿɴ�ͼ
//���룺�﷨��tree
//�����ͨ�������βδ�����CPN�Ϳɴ�ͼ
void DirectBuild(gtree *tree, string new_filename, C_Petri &petri, RG &rg)
{
	reset_gen_cpn();
	Place::total_num = 0;
	Transition::total_num = 0;
	Arc::total_num = 0;
	clock_t start, finish;


	//************************����cpn
	create_CPN(petri, tree);

	//************************���cpn
	output_CPN(petri, "output");

	//************************���ɿɴ�ͼ
	vector<string> v;
	//RG rg(petri); //����ɴ�ͼ
	rg.init_RG(petri);
	create_RG(rg);
	//print_RG(rg, dirname + new_filename);
}

//���ܣ�����﷨��
//���룺Դ������
//��������﷨����pngͼƬ��ʽ����tree.png��
void out_tree(string filename)
{
	gtree *tree;
	C_Petri petri;
	RG rg;
	tree = create_tree(filename, false);
	intofile_tree(tree);
	makeGraph("tree.dot", "tree.png");
}

//���ܣ���Դ����ͱ仯���Դ������б仯Ӱ�������
//		����Դ���򹹽�CPN�Լ��ɴ�ͼ�������б仯��⡢ģ���޸ġ��仯Ӱ�����
//���룺Դ����ͱ仯�������
//�������ֱ�ӽ�ģ�Լ��仯Ӱ��������ɵĿɴ�ͼ����rg�ļ�����
//		����program_time.txt�ļ��м�¼����ʱ�䡢�ɴ�ͼ�ڵ�����CPN�п�����Ǩ����
void compare(string filename, string new_filename)
{
	ofstream out;
	out.open("program(time).txt", ios::out | ios::app);
	if (out.fail())
	{
		cout << "open program(time) failed!" << endl;
		exit(1);
	}
	out << "���ļ���" << filename << "\t" << "���ļ���" << new_filename << endl;
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
	
	//************************ģ�ͼ��
	model_check(petri_new, rg_new);
	finish = clock();
	out << "�ɴ�ͼ�ڵ������" << rg_new.rgnode.size() << endl;
	out << "��������:" << rg_new.petri.p_num << endl;
	out << "��Ǩ����:" << rg_new.petri.t_num << endl;
	out << "ֱ�ӹ���ʱ�䣺" << (finish - start) / 1000.0 << "��" << endl;
	rg_new.release();
	petri_new.release();
	//************************ֱ�ӹ�������
	execute_flag = false;
	DirectBuild(tree1, filename, petri, rg);


	C_Petri petri_afterevolution;
	
	//************************ת��LTL��ʽ
	//char originXMLfile[50] = "formulaC.xml";
	//char desXMLfile[50] = "NewFormulaC.xml";
	//xml_trans_C(petri, originXMLfile, desXMLfile);


	vector<string> change_places;
	vector<AST_change> changes;
	string main_begin;
	C_Petri petri1;
	//����ִ�л��Ľ��
	start = clock();

	changes = compare_AST(tree1, tree2);
	

	change_places = evolution(petri, changes);


	//************************�仯Ӱ���������

	main_begin = find_P_name(petri, "main begin");
	change_places.push_back(main_begin);

	temp = clock() - start;

	petri_afterevolution.arc = petri.arc;
	petri_afterevolution.arcnum = petri.arcnum;
	petri_afterevolution.place = petri.place;
	petri_afterevolution.p_num = petri.p_num;
	petri_afterevolution.transition = petri.transition;
	petri_afterevolution.t_num = petri.t_num;
	
	petri1 = changeAnalyse(petri, change_places);

	output_CPN(petri1, "output");

	RG rg_noexecute(petri1); //����ɴ�ͼ
	create_RG(rg_noexecute);
	print_RG(rg_noexecute, rg_dirname + "changeAnalyse-" + filename);
	model_check(petri1, rg_noexecute);
	finish = clock();
	out << "�ɴ�ͼ�ڵ������" << rg_noexecute.rgnode.size() << endl;
	out << "��������:" << rg_noexecute.petri.p_num << endl;
	out << "��Ǩ����:" << rg_noexecute.petri.t_num << endl;
	out << "����ִ�л��仯Ӱ�����ʱ�䣺" << (finish - start + temp) / 1000.0 << "��" << endl;

	//��ִ�л��ı仯Ӱ�����
	execute_flag = true;
	//************************ģ���ݻ�����
	
	//	vector<string> array_this_sentence, array_this_sentence_after, array_father_sentence, array_last_sentence, array_next_sentence, array_next_father_sentence, array_next_next_sentence;

	start = clock();

	//changes = compare_AST(tree1, tree2);


	//change_places = evolution(petri, changes);

	////output_CPN(petri, "output");

	////RG rg1(petri); //����ɴ�ͼ
	////create_RG(rg1);
	////print_RG(rg1);


	////************************�仯Ӱ���������

	//main_begin = find_P_name(petri, "main begin");
	//change_places.push_back(main_begin);

	petri1 = changeAnalyse(petri_afterevolution, change_places);
	
	output_CPN(petri1, "output");

	RG rg2(petri1); //����ɴ�ͼ
	create_RG(rg2);
	print_RG(rg2, rg_dirname + "changeAnalyse-" + filename);
	model_check(petri1, rg2);
	finish = clock();
	out << "�ɴ�ͼ�ڵ������" << rg2.rgnode.size() << endl;
	out << "��������:" << rg2.petri.p_num << endl;
	out << "��Ǩ����:" << rg2.petri.t_num << endl;
	out << "�仯Ӱ�����ʱ�䣺" << (finish - start + temp) / 1000.0 << "��" << endl;
	out << endl;


	out.close();
}

//���ܣ��Ա�Դ����ֱ�ӽ�ģ����ͨ��Ƭ�Լ�����ִ�л�����Ƭ�Ľ��
//		��Ƭ�ĳ�ʼ��������ͨ���仯�������ȡ�Ŀ���ȷ��
//���룺Դ����ͱ仯�������
//�������ֱ�ӽ�ģ�Լ��仯Ӱ��������ɵĿɴ�ͼ����rg_sliceOnly�ļ�����
//		����program_time.txt�ļ��м�¼����ʱ�䡢�ɴ�ͼ�ڵ�����CPN�п�����Ǩ����
void compare1(string filename, string new_filename)
{
	ofstream out;
	out.open("program(time).txt", ios::out | ios::app);
	if (out.fail())
	{
		cout << "open program(time) failed!" << endl;
		exit(1);
	}
	out << "���ļ���" << filename << "\t" << "���ļ���" << new_filename << endl;
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

	//************************ģ�ͼ��
	model_check(petri_new, rg_new);
	finish = clock();

	out << "�ɴ�ͼ�ڵ������" << rg_new.rgnode.size() << endl;
	out << "��������:" << rg_new.petri.p_num << endl;
	out << "��Ǩ����:" << rg_new.petri.t_num << endl;
	out << "ֱ�ӹ���ʱ�䣺" << (finish - start) / 1000.0 << "��" << endl;
	rg_new.release();
	//petri_new.release();
	//************************ֱ�ӹ�������
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
	

	//************************ת��LTL��ʽ
	//char originXMLfile[50] = "formulaC.xml";
	//char desXMLfile[50] = "NewFormulaC.xml";
	//xml_trans_C(petri, originXMLfile, desXMLfile);


	vector<string> change_places;
	vector<AST_change> changes;
	string main_begin;
	C_Petri petri1;
	//����ִ�л��Ľ��
	

	changes = compare_AST(tree2, tree1);
	//adjust_del(changes);

	change_places = evolution(petri_new, changes);


	main_begin = find_P_name(petri_new, "main begin");
	change_places.push_back(main_begin);

	//temp = clock() - start;

	

	start = clock();

	petri1 = changeAnalyse(petri_new1, change_places);

	output_CPN(petri1, "output");

	RG rg_noexecute(petri1); //����ɴ�ͼ
	create_RG(rg_noexecute);
	print_RG(rg_noexecute, rg_sliceOnly_dirname + "changeAnalyseNE-" + filename);
	model_check(petri1, rg_noexecute);
	finish = clock();
	out << "�ɴ�ͼ�ڵ������" << rg_noexecute.rgnode.size() << endl;
	out << "��������:" << rg_noexecute.petri.p_num << endl;
	out << "��Ǩ����:" << rg_noexecute.petri.t_num << endl;
	out << "����ִ�л��仯Ӱ�����ʱ�䣺" << (finish - start + build_time) / 1000.0 << "��" << endl;

	//��ִ�л��ı仯Ӱ�����
	execute_flag = true;

	start = clock();
	petri1 = changeAnalyse(petri_new2, change_places);

	output_CPN(petri1, "output");

	RG rg_executed(petri1); //����ɴ�ͼ
	create_RG(rg_executed);
	print_RG(rg_executed, rg_sliceOnly_dirname + "changeAnalyseE-" + filename);
	model_check(petri1, rg_executed);
	finish = clock();
	out << "�ɴ�ͼ�ڵ������" << rg_executed.rgnode.size() << endl;
	out << "��������:" << rg_executed.petri.p_num << endl;
	out << "��Ǩ����:" << rg_executed.petri.t_num << endl;
	out << "�仯Ӱ�����ʱ�䣺" << (finish - start + build_time) / 1000.0 << "��" << endl;
	out << endl;


	out.close();
}

int main()
{

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

