

//evolution部分
//********************//
//********************//

#include"tree.h"
#include"AST_compare.h"
#include"cpn.h"
#include"y.tab.h"

bool judge_while_P(C_Petri petri, string place)
{
	for (int i = 0; i < petri.p_num; i++)
	{
		if (petri.place[i].name == place)
		{
			if (petri.place[i].v_name.rfind("while", 1) != string::npos)
				return true;
			else
				return false;
		}
	}
	cout << "judge_while_P() can't find place!" << endl;
	exit(1);
}



void delete_last_executed(C_Petri &petri, string _P1)
{
	//	string _P1 = find_P_name(petri, v_name);
	vector<string> v1 = petri.get_exit(_P1);
	if (v1.size() == 0)
		return;
	bool flag = false;
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].source == v1[0] && petri.arc[i].V == "executed")
		{
			for (int j = 0; j < petri.p_num; j++)
			{
				if (petri.arc[i].target == petri.place[j].name)
				{
					if (petri.place[j].v_name == "executedP")
					{
						petri.delete_all_connected_arc(petri.place[j].name);
						flag = true;
						break;
					}

				}
			}
			//			petri.delete_all_connected_arc(petri.arc[i].source);
		}
	}
	if (flag == true)
		return;
	for (unsigned int j = 0; j < v1.size(); j++)
	{
		for (int i = 0; i < petri.arcnum; i++)
		{
			if (petri.arc[i].source == v1[j] && petri.arc[i].V == "executed")
			{
				petri.Delete_Arc(i);
			}
		}
	}
}

//内部语句前向弧构建，将P_last和P_next执行弧添加上，并返回执行库所名
string add_executed(C_Petri &petri, string P_last, string P_next)
{
	vector<string> v1 = petri.get_exit(P_last);
	if (v1.size() == 0)
		return "";
	
	string P1 = gen_P();
	petri.Add_Place(P1, "executedP", "", true, false, 0, 0, "", 0, false);
	string P2 = P_next;
	vector<string> v2 = petri.get_enter(P2);

	for (unsigned int j = 0; j < v1.size(); j++)
	{
		petri.Add_Arc(v1[j], P1, "executed", false);
	}

	for (unsigned int j = 0; j < v2.size(); j++)
	{
		petri.Add_Arc(P1, v2[j], "executed", true);
	}
	return P1;
}

//外部语句前向弧构建，将P_last和P_next执行弧添加上
void add_executed_1(C_Petri &petri, string P_last, string P_next)
{

	vector<string> v1 = petri.get_exit(P_last);
	if (v1.size() == 0)
		return;

	//	vector<string> v2 = petri.get_enter(P2);
	vector<string> enter_P = petri.get_enter_P(P_next);
	for (unsigned int j = 0; j < v1.size(); j++)
	{
		for (unsigned i = 0; i < enter_P.size(); i++)
			petri.Add_Arc(v1[j], enter_P[i], "executed", false);
	}


}

void while_executed(C_Petri petri, string sentence, string executed_P)
{
	//while与执行库所
	if (sentence.rfind("while", 1) != string::npos)
	{
		string _P = find_P_name(petri, sentence);
		vector<string> v1 = petri.get_exit(_P);
		for (unsigned int j = 0; j < v1.size() - 1; j++)
		{
			petri.Add_Arc(v1[j], executed_P, "", false);
		}
	}
}

string find_father_func(string father_sentence)
{
	string func;
	string temp_func = father_sentence;
	if (temp_func.rfind("int", 1) != string::npos)
	{
		temp_func = temp_func.substr(3);
	}
	else if (temp_func.rfind("void", 1) != string::npos)
	{
		temp_func = temp_func.substr(4);
	}
	else if (temp_func.rfind("double", 1) != string::npos)
	{
		temp_func = temp_func.substr(6);
	}
	else if (temp_func.rfind("float", 1) != string::npos)
	{
		temp_func = temp_func.substr(5);
	}
	else if (temp_func.rfind("char", 1) != string::npos)
	{
		temp_func = temp_func.substr(4);
	}
	else if (temp_func.rfind("short", 1) != string::npos)
	{
		temp_func = temp_func.substr(5);
	}
	else if (temp_func.rfind("long", 1) != string::npos)
	{
		temp_func = temp_func.substr(4);
	}
	for (unsigned int i = 0; i < temp_func.size(); i++)
	{
		if (temp_func[i] == '(')
			break;
		func = func + temp_func[i];
	}
	return func;
}

void travel_by_place(gtree *p, string place, gtree *&temp_t)
{
	if (p == NULL)
		return;
	if (p->place == place)
		temp_t = p;

	travel_by_place(p->child, place, temp_t);
	travel_by_place(p->next, place, temp_t);
}

gtree *replicate_tree(gtree *root)
{
	gtree *newRoot = NULL;
	gtree *newChild = NULL;
	gtree *newNext = NULL;

	if (!root)
		return NULL;
	else
	{
		newNext = replicate_tree(root->next);
		newChild = replicate_tree(root->child);
		newRoot = new gtree;
		newRoot->type = root->type;
		//newRoot->type_1 = root->type_1;
		newRoot->place = root->place;
		newRoot->child = newChild;
		newRoot->next = newNext;
		
		newRoot->contain_call = root->contain_call;
		newRoot->contain_call_flag = root->contain_call_flag;
	}
	return newRoot;
}

void parent_son(gtree *&t)//先序将所有parent赋值
{
	if (t == NULL)
		return;
	if (t->child != NULL)
	{
		t->child->parent = t;
		parent_son(t->child);
	}
	if (t->next != NULL)
	{
		t->next->parent = t->parent;
		parent_son(t->next);
	}
}

//void real_exit(vector<string> &v)
//{
//
//	string temp = v[v.size() - 1];
//	v.clear();
//	v.push_back(temp);
//}

void delete_all_arc()
{

}

void back_executed(C_Petri &petri, vector<string> last_T, vector<string> new_place_exit)//将last_T的出口变迁对应的所有执行弧用new_place_exit的出口变迁代替
{
	if (last_T.size() == 0)
		return;
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].V == "executed")
		{
			for (unsigned int j = 0; j < last_T.size(); j++)
			{
				if (petri.arc[i].source == last_T[j])
				{
					petri.arc[i].V = "executed#";
					for (unsigned int k = 0; k < new_place_exit.size(); k++)
						petri.Add_Arc(new_place_exit[k], petri.arc[i].target, "executed", false);
					break;
				}
			}

		}
	}
}

void pre_executed_1(C_Petri &petri, vector<string> next_T, vector<string> new_place_enter)//将指向new_place_enter语句入口的所有执行弧删除，并指向下一条语句入口
{
	if (new_place_enter.size() == 0)
		return;
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].V == "executed")
		{
			for (unsigned int j = 0; j < new_place_enter.size(); j++)
			{
				if (petri.arc[i].target == new_place_enter[j])
				{
					petri.arc[i].V = "executed#";
					for (unsigned int k = 0; k < next_T.size(); k++)
						petri.Add_Arc(petri.arc[i].source, next_T[k], "executed", true);
				}
			}

		}
	}
}

void pre_executed_1(C_Petri &petri, string P1, string P2)//将指向new_place_enter语句入口的所有执行弧删除，并指向下一条语句入口
{
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].V == "executed")
		{

			if (petri.arc[i].target == P1)
			{
				petri.arc[i].V = "executed#";
				petri.Add_Arc(petri.arc[i].source, P2, "executed", false);
			}
		}
	}
}

void change_exit(C_Petri &petri, string father_P, vector<string>last_T, vector<string> new_place_exit)//改变父语句的出口库所由last_T变为new_place_exit
{
	for (int i = 0; i < petri.p_num; i++)
	{
		if (petri.place[i].name == father_P)
		{
			for (vector<string>::iterator it = petri.place[i].exit.begin(); it != petri.place[i].exit.end(); )
			{
				if (exist_in(last_T, *it))
				{
					it = petri.place[i].exit.erase(it);
				}
				else
					it++;
			}

			petri.place[i].exit.insert(petri.place[i].exit.begin(), new_place_exit.begin(), new_place_exit.end());

			break;
		}
	}
}

void inside_add(C_Petri &petri, string father_place, gtree *father_tree, string new_place, gtree *newtree)//添加inside语句的特有弧
{
	vector<string> v = petri.get_enter(father_place);
	//int call_flag = petri.get_call_flag(new_place);
	gtree *compound = newtree;
	while (compound->type != COMPOUND_STATEMENT)
		compound = compound->parent;


	if (father_tree->type == SELECTION_STATEMENT)
	{
		gtree *find_else = father_tree->child;
		bool else_flag = false;
		while (find_else != compound->parent)
		{
			if (find_else->type == ELSE )//&& find_else->type_1 == 4)//else
			{
				else_flag = true;
				break;
			}
			find_else = find_else->next;
		}
		if (else_flag == false)
		{
			vector<string> temp = petri.get_enter_P(new_place);
			for (unsigned int i = 0; i < temp.size(); i++)
				petri.Add_Arc(v[0], temp[i], "", false);
		}
		else
		{
			vector<string> temp = petri.get_enter_P(new_place);
			for (unsigned int i = 0; i < temp.size(); i++)
				petri.Add_Arc(v[1], temp[i], "", false);
		}
	}
	else
	{

		vector<string> temp = petri.get_enter_P(new_place);
		for (unsigned int i = 0; i < temp.size(); i++)
			petri.Add_Arc(v[0], temp[i], "", false);
	}
}

void inside_delete(C_Petri &petri, string father_place, string delete_place)
{
	vector<string> v = petri.get_control_T(father_place);
	for (unsigned int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < petri.arcnum; j++)
		{
			if (petri.arc[j].source == v[i] && petri.arc[j].target == delete_place)
			{
				petri.arc[j].V += "#";
				break;
			}
		}
	}
}

void delete_node(gtree *&tree)
{
	if (tree->next)
		delete_node(tree->next);
	if (tree->child)
		delete_node(tree->child);
	delete tree;
}

void change_call_enter(C_Petri &petri, vector<string> source, vector<string> target, string place)//将source与place之间弧删去，添加target到place的弧
{
	for (unsigned int i = 0; i < source.size(); i++)
		petri.Delete_Arc(source[i], place);
	for (unsigned int i = 0; i < target.size(); i++)
		petri.Add_Arc(target[i], place, "", false);
}

string operate_add(C_Petri &petri, Mapping m, pair<int, int> add)
{

	bool isfirst, islast, inside;
	int position1 = add.first;
	int position2 = add.second;
	gtree *t1, *t2;
	gtree *last_tree = NULL, *next_tree = NULL;
	gtree *father_tree = m.map1;//father_tree只能指向具体语句或函数定义
	while (father_tree->type != FUNCTION_DEFINITION && father_tree->type != ITERATION_STATEMENT && father_tree->type != SELECTION_STATEMENT)
		father_tree = father_tree->parent;
	t1 = m.map1;
	while (t1->type != STATEMENT)
		t1 = t1->child;
	//gtree *first_statement1 = t1;
	for (int i = 0; i < position1 - 1; i++)
		t1 = t1->parent->next;
	t2 = m.map2;
	while (t2->type != STATEMENT)
		t2 = t2->child;
	//gtree *first_statement2 = t2;
	for (int i = 0; i < position2 - 1; i++)
		t2 = t2->parent->next;

	string current_P = find_P_name(petri, t1->place);
	vector<string> current_T = petri.get_control_T(current_P);
	int current = petri.get_current_P_num(current_T[0]);

	if (position1 == 0)
	{
		isfirst = true;
		islast = false;
	}
	else
	{
		isfirst = false;
		last_tree = t1;
		//for (int i = 0; i < position1 - 1; i++)
		//	last_tree = last_tree->next;
		if (t1->parent->next->type != STATEMENT)
			islast = true;
		else
			islast = false;
	}
	if (father_tree->type == FUNCTION_DEFINITION)
		inside = false;
	else
		inside = true;

	//处理树的改变
	gtree *newtree = replicate_tree(t2);
	parent_son(newtree);

	gtree *new_statement_list = newNode("statement_list", STATEMENT_LIST);
	if (isfirst == true)
	{
		next_tree = t1;
		new_statement_list->parent = next_tree->parent;
		new_statement_list->next = next_tree;
		next_tree->parent->child = new_statement_list;
		new_statement_list->child = newtree;
		newtree->parent = new_statement_list;
	}
	else// if (islast == true)
	{
		if (islast == true)
			next_tree = NULL;
		else
			next_tree = t1->parent->next;
		if (t1->parent->child == t1)
		{
			new_statement_list->parent = t1->parent;
			new_statement_list->child = t1;
			t1->parent->child = new_statement_list;
			t1->parent = new_statement_list;
			new_statement_list->next = newtree;
			newtree->parent = new_statement_list->parent;
		}
		else
		{
			new_statement_list->parent = t1->parent;
			new_statement_list->child = t1->parent->child;
			t1->parent->child->parent = new_statement_list;
			t1->parent->child = new_statement_list;
			t1->parent = new_statement_list;
			new_statement_list->next = newtree;
			newtree->parent = new_statement_list->parent;
		}
	}



	//处理新语句的cpn构建
	string new_place = get_gen_P();
	//string new_T = get_gen_T();

	

	ast_to_cpn(petri, newtree, current);//直接对statement进行建模

	//按id_num冒泡排序
	for (int i = 0; i < petri.p_num; i++)
	{
		for (int j = 0; j < petri.p_num - i - 1; j++)
		{
			if (petri.place[j].id_num > petri.place[j + 1].id_num)
			{
				Place tmp(petri.place[j]);
				//tmp = petri.place[j];
				petri.place[j] = petri.place[j + 1];
				petri.place[j + 1] = tmp;
			}
		}
	}

	//这里处理函数调用语句新建的第一个库所不是语句的控制库所的问题
	vector<string> temp_enter = petri.get_enter_P(new_place);
	if (temp_enter.size() == 0)
	{
		int p_num = atoi(new_place.substr(1).c_str());
		if (inside == true)
			p_num++;
		else
			p_num += 2;
		new_place = "P" + to_string(p_num);
	}


	//修改执行弧
	string father_place;
	
	if (inside == true)
	{
		if (father_tree->type == FUNCTION_DEFINITION)
			father_place = find_P_name(petri, father_tree->place + " begin");
		else
			father_place = find_P_name(petri, father_tree->parent->place);//此处parent是避免重名，因为重名只改statement的place
	}


	if (isfirst == true)
	{
		string next_P = find_P_name(petri, next_tree->place);
		//前向弧
		//if (pre_executed == false)
		//{
		if (inside == true)
		{
			inside_add(petri, father_place, father_tree, new_place, newtree);
		}
		else
		{
			string func = father_tree->place;
			string func_T = find_T_name(petri, func + " begin");
			vector<string> new_place_enter_P = petri.get_enter_P(new_place);
			vector<string> next_P_enter_P = petri.get_enter_P(next_P);
			for (unsigned int i = 0; i < new_place_enter_P.size(); i++)
				petri.Add_Arc(func_T, new_place_enter_P[i], "", false);
			for (unsigned int i = 0; i < next_P_enter_P.size(); i++)
				petri.Delete_Arc(func_T, next_P_enter_P[i]);
		}
		/*}
		else if (inside == true)
			inside_add(petri, father_place, father_tree, new_place, newtree);*/
		//后向弧

		//bool whileorif = false;

		if (inside == true)
		{
			add_executed(petri, new_place, next_P);
		}
		else
		{
			add_executed_1(petri, new_place, next_P);
		}
	}
	else
	{
		string last_P = find_P_name(petri, last_tree->place);
		//后向弧
		vector<string> last_T = petri.get_exit(last_P);
		vector<string> new_place_exit = petri.get_exit(new_place);


		back_executed(petri, last_T, new_place_exit);

		//前向弧

		//if (pre_executed == false)
		//{

			//bool whileorif = false;
			if (inside == true)
			{
				inside_add(petri, father_place, father_tree, new_place, newtree);
				/*if (last_tree->child->type == 循环语句)
					whileorif = true;*/
				string executed_P = add_executed(petri, last_P, new_place);

				//新增while语句要注意前向执行库所
				if (newtree->child->type == ITERATION_STATEMENT && executed_P != "")
				{
					vector<string> new_exit = petri.get_exit(new_place);
					for (unsigned int i = 0; i < new_exit.size() - 1; i++)
						petri.Add_Arc(new_exit[i], executed_P, "", false);
				}
			}
			else
			{
				/*if (last_tree->child->type == 循环语句)
					whileorif = true;*/
				//if (pre_executed == false)
				add_executed_1(petri, last_P, new_place);
			}

		//修改出口
		if (islast == true && inside == true)
			change_exit(petri, father_place, last_T, new_place_exit);
	}

	return new_place;
}

string operate_del(C_Petri &petri, Mapping m, pair<int, int> del)
{
	bool isfirst, islast, inside;
	int position1 = del.first;
	int position2 = del.second;
	gtree *t1, *t2;
	gtree *last_tree = NULL, *next_tree = NULL;
	gtree *father_tree = m.map1;//father_tree只能指向具体语句或函数定义
	while (father_tree->type != FUNCTION_DEFINITION && father_tree->type != ITERATION_STATEMENT && father_tree->type != SELECTION_STATEMENT)
		father_tree = father_tree->parent;
	t1 = m.map1;
	while (t1->type != STATEMENT)
		t1 = t1->child;
	gtree *first_statement1 = t1;
	for (int i = 0; i < position1 - 1; i++)
		t1 = t1->parent->next;
	t2 = m.map2;
	while (t2->type != STATEMENT)
		t2 = t2->child;
	//gtree *first_statement2 = t2;
	for (int i = 0; i < position2 - 1; i++)
		t2 = t2->parent->next;

	if (position1 == 1)
	{
		isfirst = true;
		islast = false;
	}
	else
	{
		isfirst = false;
		last_tree = first_statement1;
		for (int i = 0; i < position1 - 2; i++)
			last_tree = last_tree->parent->next;
		if (t1->parent->next->type != STATEMENT)
			islast = true;
		else
			islast = false;
	}
	if (father_tree->type == FUNCTION_DEFINITION)
		inside = false;
	else
		inside = true;

	
	string delete_place = find_P_name(petri, t1->place);
	


	//处理语法树
	if (isfirst == true)
	{
		t1->parent->parent->child = t1->parent->next;
		delete_node(t1->child);
		delete t1;
	}
	else
	{
		t1->parent->child->child->parent = t1->parent;
		if (t1->parent->child->child->next != NULL)
			t1->parent->child->child->next->parent = t1->parent;
		t1->parent->child = t1->parent->child->child;
		delete_node(t1->child);
		delete t1;
	}

	//修改执行弧


	if (islast == false)
	{
		if (isfirst == true)
		{
			gtree *temp = m.map1;
			while (temp->type != STATEMENT)
				temp = temp->child;
			next_tree = temp;
		}
		else
			next_tree = last_tree->parent->next;
	}
	if (next_tree != NULL && judge_expression_statement(next_tree))
	{
		next_tree = next_tree->parent->next;
	}

	string father_place;
	if (father_tree->type == FUNCTION_DEFINITION)
		father_place = find_P_name(petri, father_tree->place + " begin");
	else
		father_place = find_P_name(petri, father_tree->parent->place);

	if (isfirst == true)
	{
		string next_P = find_P_name(petri, next_tree->place);
		if (inside == true)
		{
			
			//前向弧
			inside_delete(petri, father_place, delete_place);
			//后向弧
			delete_last_executed(petri, delete_place);
		}
		else
		{
			string func = father_tree->place;
			string func_T = find_T_name(petri, func + " begin");
			//前向弧
			vector<string> enter_P = petri.get_enter_P(delete_place);
			vector<string> next_enter_P = petri.get_enter_P(next_P);
			for (int i = 0; i < petri.arcnum; i++)
			{
				if (petri.arc[i].source == func_T)
				{
					if (enter_P[0] == petri.arc[i].target)
					{
						petri.arc[i].V += "#";
						for (unsigned int j = 0; j < next_enter_P.size(); j++)
							petri.Add_Arc(petri.arc[i].source, next_enter_P[j], "", false);
					}
					else if (enter_P.size() == 2 && enter_P[1] == petri.arc[i].target)
					{
						petri.arc[i].V += "#";
						for (unsigned int j = 0; j < next_enter_P.size(); j++)
							petri.Add_Arc(petri.arc[i].source, next_enter_P[j], "", false);
					}
				}
			}
			//后向弧
			delete_last_executed(petri, delete_place);
		}

	}
	else
	{
		string last_P = find_P_name(petri, last_tree->place);
		//vector<string> last_T = petri.get_exit(last_P);
		vector<string> delete_place_enter = petri.get_enter(delete_place);
		vector<string> delete_place_exit = petri.get_exit(delete_place);
		vector<string> last_exit = petri.get_exit(last_P);
		/*if (iswhile == true)
			real_exit(delete_place_exit);
		if (last_tree->child->type == 循环语句)
			real_exit(last_exit);*/

		//gtree *next_tree = NULL;
		vector<string> next_enter;
		string next_P;
		
		if (islast == false)
		{
			//next_tree = last_tree->next;
			next_P = find_P_name(petri, next_tree->place);
			next_enter = petri.get_enter(next_P);
		}
		if (inside == true)
		{
			//string father_place = find_P_name(petri, father_tree->place);
			//前向弧
			inside_delete(petri, father_place, delete_place);
			//后向弧
			if (islast == false)
			{
				pre_executed_1(petri, next_enter, delete_place_enter);
				delete_last_executed(petri, delete_place);

			}
			else
			{
				change_exit(petri, father_place, delete_place_exit, last_exit);
				back_executed(petri, delete_place_exit, last_exit);
			}
		}
		else
		{
			string func = father_tree->place;
			string func_end_P = find_P_name(petri, func + " end");
			//前向弧
			//此处不用处理
			//后向弧
			if (islast == false)
			{
				//pre_executed_1(petri, delete_place, next_P);
				//delete_last_executed(petri, delete_place);
				//change_exit(petri, father_P, last_T, delete_place_exit);

				for (unsigned int i = 0; i < last_exit.size(); i++)
				{
					//if (next_call == false)
					vector<string> next_P_enter_P = petri.get_enter_P(next_P);
					vector<string> delete_place_enter_P = petri.get_enter_P(delete_place);
					for (unsigned int j = 0; j < next_P_enter_P.size(); j++)
						petri.Add_Arc(last_exit[i], next_P_enter_P[j], "executed", false);
					for (unsigned int j = 0; j < delete_place_enter_P.size(); j++)
						petri.Delete_Arc(last_exit[i], delete_place_enter_P[j]);
				}
			}
			else
			{
				for (unsigned int i = 0; i < last_exit.size(); i++)
				{
					vector<string> delete_place_enter_P = petri.get_enter_P(delete_place);
					petri.Add_Arc(last_exit[i], func_end_P, "executed", false);
					for (unsigned int j = 0; j < delete_place_enter_P.size(); j++)
						petri.Delete_Arc(last_exit[i], delete_place_enter_P[j]);
				}

			}
		}

	}

	//return "";
	return delete_place;
}

vector<string> operate_modify(C_Petri &petri, Mapping m, pair<int, int> modify)
{
	vector<string> change_P;
	pair<int, int> add, del;
	del.first = modify.first + 1;
	del.second = modify.second - 1;
	add.first = modify.first - 1;
	add.second = modify.second;
	string add_P = operate_add(petri, m, add);
	string del_P = operate_del(petri, m, del);
	if (del_P != "")
		change_P.push_back(del_P);
	if (add_P != "")
		change_P.push_back(add_P);
	return change_P;
}

vector<string> operate_move(C_Petri &petri, Mapping m, pair<int, int> move)
{
	vector<string> change_P;
	pair<int, int> add, del;
	del.first = move.first;
	del.second = move.first - 1;
	add.first = move.second - 1;
	add.second = move.second;
	string del_P = operate_del(petri, m, del);
	string add_P = operate_add(petri, m, add);
	if (del_P != "")
		change_P.push_back(del_P);
	if (add_P != "")
		change_P.push_back(add_P);
	return change_P;
}

string operate_add_declare(C_Petri &petri, Mapping m, pair<int, int> add)
{
	//int position1 = add.first;
	int position2 = add.second;
	gtree *t2;
	int current;
	//t1 = m.map1;
	//while (t1->type != DECLARATION)
	//	t1 = t1->child;
	////gtree *first_statement1 = t1;
	//for (int i = 0; i < position1 - 1; i++)
	//	t1 = t1->parent->next;
	t2 = m.map2;
	while (t2->type != DECLARATION)
		t2 = t2->child;
	current = m.map1->record_P_num;
	//gtree *first_statement2 = t2;
	for (int i = 0; i < position2 - 1; i++)
		t2 = t2->parent->next;
	string new_place = get_gen_P();
	ast_to_cpn(petri, t2, current);

	//按id_num冒泡
	for (int i = 0; i < petri.p_num; i++)
	{
		for (int j = 0; j < petri.p_num - i-1; j++)
		{
			if (petri.place[j].id_num > petri.place[j + 1].id_num)
			{
				Place tmp(petri.place[j]);
				//tmp = petri.place[j];
				petri.place[j] = petri.place[j + 1];
				petri.place[j + 1] = tmp;
			}
		}
	}
	return new_place;
}

void copy_P(C_Petri &petri, string P1, string P2)
{
	for (int i = 0; i < petri.p_num; i++)
	{
		if(petri.place[i].name==P1)
			for(int j=0;j<petri.p_num;j++)
				if (petri.place[j].name == P2)
				{
					petri.place[j].v_name = petri.place[i].v_name;
					petri.place[j].num = petri.place[i].num;
					petri.place[j].n_num = petri.place[i].n_num;
					petri.place[j].decimal = petri.place[i].decimal;
					petri.place[j].n_decimal = petri.place[i].n_decimal;
					petri.place[j].str = petri.place[i].str;
					return;
				}
	}
}

void delete_P(C_Petri &petri, string P)
{
	for (int i = 0; i < petri.p_num; i++)
		if (petri.place[i].name == P)
			petri.place[i].v_name += "#";
}

string operate_modify_declare(C_Petri &petri, Mapping m, pair<int, int> add)
{
	int position1 = add.first;
	int position2 = add.second;
	gtree *t1, *t2;

	t1 = m.map1;
	while (t1->type != DECLARATION)
		t1 = t1->child;
	//gtree *first_statement1 = t1;
	for (int i = 0; i < position1 - 1; i++)
		t1 = t1->parent->next;
	t2 = m.map2;
	while (t2->type != DECLARATION)
		t2 = t2->child;
	//gtree *first_statement2 = t2;
	for (int i = 0; i < position2 - 1; i++)
		t2 = t2->parent->next;
	/*string new_place = get_gen_P();
	ast_to_cpn(petri, t2);*/
	vector<string> P_list, P_name_list1, P_name_list2;
	gtree *init_dec_list = t1->child->next;
	gtree *init_dec = init_dec_list;
	while (init_dec->type != INIT_DECLARATOR)
		init_dec = init_dec->child;
	while (init_dec != NULL)
	{
		string dec_name = init_dec->place;
		P_name_list1.push_back(dec_name);
		string _P = find_P_name(petri, dec_name);
		P_list.push_back(_P);
		init_dec = init_dec->parent->next->next;
	}

	
	init_dec = t2->child->next;
	while (init_dec->type != INIT_DECLARATOR)
		init_dec = init_dec->child;
	while (init_dec != NULL)
	{
		string dec_name = init_dec->place;
		P_name_list2.push_back(dec_name);
		//string _P = find_P_name(petri, dec_name);
		//P_list.push_back(_P);
		init_dec = init_dec->parent->next->next;
	}
	int match = -1;//暂定为单参数
	for (unsigned int i = 0; i < P_name_list2.size(); i++)
	{
		for (unsigned int j = 0; j < P_name_list1.size();j++)
			if (P_name_list2[i] == P_name_list1[j])
			{
				match = j;
				break;
			}
	}
	//init_dec = init_dec->parent->next->next;
	string new_place = operate_add_declare(petri, m, add);
	/*if (match != -1)
	{*/
		string origin_P = P_list[0];//这里0是暂时的
		copy_P(petri, new_place, origin_P);
		delete_P(petri, new_place);
		return origin_P;
	/*}
	else
		return new_place;*/
}

enum { ADD, DEL };

void adjust_changes(AST_change &change, int flag, int position)
{
	if (flag == ADD)
	{
		for (unsigned int i = 0; i < change.del.size(); i++)
		{
			if (change.del[i].first > position)
				change.del[i].first++;
		}
		for (unsigned int i = 0; i < change.move.size(); i++)
		{
			if (change.move[i].first > position)
				change.move[i].first++;
		}
	}
	else
	{
		for (unsigned int i = 0; i < change.move.size(); i++)
		{
			if (change.move[i].first > position)
				change.move[i].first--;
		}
	}
}

vector<string> evolution(C_Petri &petri, vector<AST_change> &changes)//t1,t2代表两棵语法树根节点
{
	vector<string> change_P;
	vector<string> temp_P;
	for (unsigned int i = 0; i < changes.size(); i++)
	{
		if (changes[i].m.map1->type == DECLARATION_LIST)
		{
			string temp_P1;
			for (unsigned int j = 0; j < changes[i].modify.size(); j++)
			{

				temp_P1 = operate_modify_declare(petri, changes[i].m, changes[i].modify[j]);
				//change_P.insert(change_P.end(), temp_P.begin(), temp_P.end());
				change_P.push_back(temp_P1);
			}
			for (unsigned int j = 0; j < changes[i].add.size(); j++)
			{
				temp_P1 = operate_add_declare(petri, changes[i].m, changes[i].add[j]);
				//change_P.insert(change_P.end(), temp_P.begin(), temp_P.end());
				change_P.push_back(temp_P1);

			}
		}
	}
	
	
	for (unsigned int i = 0; i < changes.size(); i++)
	{
		//vector<int> add_num,del_num;
		if (changes[i].m.map1->type == STATEMENT_LIST)
		{
			for (int j = int(changes[i].modify.size() - 1); j >= 0; j--)
			{
				temp_P = operate_modify(petri, changes[i].m, changes[i].modify[j]);
				change_P.insert(change_P.end(), temp_P.begin(), temp_P.end());
			}
			for (int j = int(changes[i].add.size() - 1); j >= 0; j--)
			//for (unsigned int j = 0; j < changes[i].add.size(); j++)
			{
				string add = operate_add(petri, changes[i].m, changes[i].add[j]);
				adjust_changes(changes[i], ADD, changes[i].add[j].first);

				if (add != "")
					change_P.push_back(add);
			}
			for (int j = int(changes[i].del.size() - 1); j >= 0; j--)
			{
				string del = operate_del(petri, changes[i].m, changes[i].del[j]);
				adjust_changes(changes[i], DEL, changes[i].del[j].first);

				if (del != "")
					change_P.push_back(del);
			}
			for (int j = int(changes[i].move.size() - 1); j >= 0; j--)
			{
				temp_P = operate_move(petri, changes[i].m, changes[i].move[j]);
				change_P.insert(change_P.end(), temp_P.begin(), temp_P.end());
			}
		}
	}
	return change_P;
}