#define _CRT_SECURE_NO_WARNINGS
#include"cpn.h"
#include"y.tab.h"
#include"v_table.h"
#include<fstream>

string colorset[] = { "int","char","float","double","string" };

bool execute_flag = false;

int Arc::total_num = 0;
int Place::total_num = 0;
int Transition::total_num = 0;

vector<V_Table *> v_tables;//variable tables

Arc::Arc(string s, string t, string v, bool s_P)
{
	source = s;
	target = t;
	V = v;
	sourceP = s_P;
	id_num = total_num++;

}

Place::Place(string n, string v_n, string colorset_t, bool c_P)
{
	name = n;
	v_name = v_n;
	colorset_tag = colorset_t;
	controlP = c_P;
	id_num = total_num++;
	global = false;
	ispoint = false;
}

void Place::set_Place_value(int n)
{
	num[0] = n;
	token_num = 1;
}

void Place::set_Place_value(double dec)
{
	decimal[0] = dec;
	token_num = 1;
}

void Place::set_Place_value(string s)
{
	str = s;
	token_num = 1;
}

Transition::Transition(string n, bool c_T, int current)
{
	name = n;
	controlT = c_T;
	current_P_num = current;
	id_num = total_num++;
}

void Transition::set_C_Transition_value(string str)
{
	booleanExpression = str;
}

void Transition::set_V_Transition_value(string str)
{
	v_Expression = str;
}

C_Petri::C_Petri()
{

}

void C_Petri::Add_Arc(Arc a)
{
	arc.push_back(a);
	arcnum++;
}

void C_Petri::Add_Arc(string source, string target, string V, bool sourceP)
{
	int sourceid, targetid;
	Arc a(source, target, V, sourceP);
	for (vector<Arc>::iterator it = arc.begin(); it != arc.end(); it++)
	{
		if (it->source == source && it->target == target)
		{

			if (it->V == V)
			{
				//cout << "�ظ���ӱ�" << source << "-->" << target << endl;
				//arcnum--;
				return;
			}
			else
			{
				//cout << "���Ǳ�" << source << "-->" << target << endl;
				arc.erase(it);
				arcnum--;
				break;
			}
		}
	}

	if (sourceP)
	{
		int i, j;
		for (i = 0; i < p_num; i++)
		{
			if (place[i].name == source)
			{
				sourceid = place[i].id_num;
				break;
			}
		}
		if (i == p_num)
		{
			cout << "can't find source:" << source << endl;
			//exit(1);
		}
		for (j = 0; j < t_num; j++)
		{
			if (transition[j].name == target)
			{
				targetid = transition[j].id_num;
				break;
			}
		}
		if (j == t_num)
		{
			cout << "can't find target:" << target << endl;
			exit(1);
		}
	}
	else
	{
		int i, j;
		for (i = 0; i < p_num; i++)
		{
			if (place[i].name == target)
			{
				targetid = place[i].id_num;
				break;
			}
		}
		if (i == p_num)
		{
			cout << "can't find target!" << endl;
			exit(1);
		}
		for (j = 0; j < t_num; j++)
		{
			if (transition[j].name == source)
			{
				sourceid = transition[j].id_num;
				break;
			}
		}
		if (j == t_num)
		{
			cout << "can't find source!" << endl;
			//exit(1);
		}
	}
	a.sourceNum = sourceid;
	a.targetNum = targetid;
	Add_Arc(a);
}

void C_Petri::Add_Place(Place p)
{
	place.push_back(p);
	p_num++;
}

void C_Petri::Add_Place(string name, string v_name, string colorset_tag, bool controlP, bool t, int n, double dec, string s, int array_size,bool ispoint)
{
	//	int judge = find_exist(place, v_name);
	//	if (judge != 0)
	//	{
	//		char temp[5];
	//		itoa(judge, temp, 10);
	//		v_name = v_name + "@" + temp;
	//	}
	Place place(name, v_name, colorset_tag, controlP);
	place.ispoint = ispoint;
	if (controlP == false && v_name.find("@") == string::npos)
	{
		if (v_name.size() > 2 && v_name[v_name.size() - 1] == 'v'&&v_name[v_name.size() - 2] == '_')
			;
		else
			place.global = true;
	}
	if (!t)
	{
		//		place.token_num = 0;//��ʾδ����ֵ
		if (colorset_tag == "")
		{

		}
		else
		{
			switch (colorset_tag[colorset_tag.size() - 1])
			{
			case '1':
			case '5':
			case '6':
				place.num = new int[array_size];
				place.n_num = array_size;
				break;
			case '3':
			case '4':
				place.decimal = new double[array_size];
				place.n_decimal = array_size;
				break;
			default:
				break;
			}
		}
		if (controlP == true)
			place.token_num = 0;
		else
			place.token_num = 1;
		Add_Place(place);
	}
	else
	{
		if (controlP == true)
			place.token_num = 0;
		else
			place.token_num = 1;

		switch (colorset_tag[colorset_tag.size() - 1])
		{
		case '1':
		case '5':
		case '6':
			place.num = new int[array_size];
			place.n_num = array_size;
			place.set_Place_value(n);
			break;
		case '3':
		case '4':
			place.decimal = new double[array_size];
			place.n_decimal = array_size;
			place.set_Place_value(dec);
			break;
		default:
			place.set_Place_value(s);
			break;
		}
		Add_Place(place);
	}
}

void C_Petri::Add_Transition(Transition t)
{
	transition.push_back(t);
	t_num++;
}

void C_Petri::Add_Transition(string name, bool controlT, string s, int current)
{
	Transition t(name, controlT, current);
	if (controlT)
		t.set_C_Transition_value(s);
	else
		t.set_V_Transition_value(s);
	Add_Transition(t);


}

string C_Petri::find_place(string t_name, string V)
{
	for (int i = 0; i < arcnum; i++)
	{
		if ((arc[i].sourceP == true) && (arc[i].target == t_name) && arc[i].V == V)
		{
			return arc[i].source;
		}
	}
	cout << "can't find place" << endl;
	return "";
}

bool C_Petri::Add_Place_enter(string name, string s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].enter.push_back(s);
			return true;
		}
	}
	return false;
}

bool C_Petri::Add_Place_enter(string name, vector<string> s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].enter.insert(place[i].enter.end(), s.begin(), s.end());
			return true;
		}
	}
	return false;
}

bool C_Petri::Add_Place_exit(string name, string s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].exit.push_back(s);
			return true;
		}
	}
	return false;
}

bool C_Petri::Add_Place_exit(string name, vector<string> s)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			place[i].exit.insert(place[i].exit.end(), s.begin(), s.end());
			return true;
		}
	}
	return false;
}

vector<string> C_Petri::get_enter(string name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			return place[i].enter;
		}
	}
	cout << "error in get_enter()" << endl;
	//exit(1);
}

vector<string> C_Petri::get_exit(string name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (name == place[i].name)
		{
			return place[i].exit;
		}
	}
	cout << "error in get_exit()" << endl;
	//exit(1);
}

void C_Petri::set_global(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (p_name == place[i].name)
		{
			place[i].global = true;
			break;
		}
	}
}

bool C_Petri::get_global(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (p_name == place[i].name)
		{
			return place[i].global;
			//break;
		}
	}
}


bool C_Petri::is_enable(string t_name)
{
	vector<string> all_P;
	all_P = find_all_place(t_name);
	for (unsigned int i = 0; i < all_P.size(); i++)
	{
		for (int j = 0; j < p_num; j++)
		{
			if (place[j].name == all_P[i])
			{
				if (place[j].token_num < 1)
				{
					return false;
				}
			}
		}
	}

	string guard_Expression;
	for (int i = 0; i < t_num; i++)
	{
		if (transition[i].name == t_name)
		{
			guard_Expression = transition[i].booleanExpression;
			break;
		}
	}
	if (guard_Expression != "")
	{
		bool flag1 = guard(guard_Expression, t_name);
		if (flag1 == true)
			return true;
		else
			return false;
	}
	else
	{
			return true;
	}

}

vector<string> C_Petri::enable_T()
{
	vector<string> T;
	for (int i = 0; i < t_num; i++)
	{
		if (is_enable(transition[i].name) == true)
		{
			T.push_back(transition[i].name);
		}
	}

	return T;
}

bool judge_logic_operator(string s)
{
	if (s == "&&" || s == "||")
		return true;
	return false;
}

bool judge_relation_operator(string s)
{
	if (s == ">" || s == "<" || s == ">=" || s == "<=" || s == "==" || s == "!=")
		return true;
	return false;
}

int priority(string s)
{
	if (s == "#")
		return -1;
	else
	{
		if (s == "(")
			return 0;
		else if (judge_logic_operator(s))
			return 1;
		else if (judge_relation_operator(s))
			return 2;
		else if (s == "+" || s == "-")
			return 3;
		else if (s == "*" || s == "/" || s == "%")
			return 4;
		else if (s == "&" || s == "|")
			return 5;
		else if (s == ")" || s == "!" || s == "@" || s == "^")
			return 6;

	}
	cout << "priority error!" << endl;
	return -2;
}

bool is_operator(string s)
{
	if (s == "+" || s == "-" || s == "*" 
		|| s == "/" || s == "(" || s == ")" 
		|| s == "#" || s == "&" || s == "|" 
		|| s == "^" || s == "%" || s == "!"
		||judge_logic_operator(s)||judge_relation_operator(s))
	{
		return true;
	}
	return false;
}

int is_operator(string s, int pos)//�ж��Ƿ�Ϊ�����������1��ʾ���ַ������������2��ʾ˫�ַ������
{
	if (s[pos] == '#')
		return 1;
	string temp_s;
	temp_s = s[pos];
	temp_s.append(1,s[pos + 1]);
	if (is_operator(temp_s))
		return 2;
	temp_s = s[pos];
	if (is_operator(temp_s))
		return 1;
	return 0;
}

double process_get_value(vector<Place> place, int pos, int current,int array_num)
{
	Place *temp_p1 = &place[pos];
	while (temp_p1->ispoint == true)
	{
		if (temp_p1->str == "")
		{
			cout << "point δ��ֵ!" << endl;
			exit(1);
		}
		//string P_num = "P" + to_string(current);
		for (unsigned int i = 0 ; i <place.size(); i++)
			if (place[i].name == temp_p1->str)
			{
				temp_p1 = &place[i];
				break;
			}
	}
	if (temp_p1->colorset_tag[temp_p1->colorset_tag.size() - 1] == '1' || temp_p1->colorset_tag[temp_p1->colorset_tag.size() - 1] == '5' || temp_p1->colorset_tag[temp_p1->colorset_tag.size() - 1] == '6')
		return temp_p1->num[array_num];
	else if (temp_p1->colorset_tag[temp_p1->colorset_tag.size() - 1] == '3' || temp_p1->colorset_tag[temp_p1->colorset_tag.size() - 1] == '4')
		return temp_p1->decimal[array_num];
	else
	{
		cout << "����colortag����!" << endl;
		return -1;
	}
}

double get_value(string s, vector<Place> place, int current)//ͨ��������Ѱ�ұ�������value��ֵ��δ��������
{
	int position = 0;
	int array_num = 0;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == '[')
		{
			position = i;
			string temp_s;
			for (unsigned int j = i + 1; j < s.length(); j++)
			{
				if (s[j] == ']')
					break;
				temp_s += s[j];
			}
			array_num = int(change_and_cal(temp_s, place, current));
			break;
		}
	}
	if (position != 0)
		s = s.substr(0, position);

	if (s[0] >= '0'&&s[0] <= '9' || (s[0] == '-'&&s[1] >= '0'&&s[1] <= '9'))
	{
		stringstream ss;
		ss << s;
		double temp;
		ss >> temp;
		return temp;
	}
	int current_1 = place.size() - 1;
	/*for (int i = 0; i<int(place.size()); i++)
	{
		int p_num = atoi(place[i].name.substr(1).c_str());
		if (p_num > current)
		{
			current_1 = i - 1;
			break;
		}
	}*/
	for (int i = current_1; i > 0; i--)
	{
		if (place[i].id_num <= current)
		{
			current_1 = i;
			break;
		}
	}
	for (int i = current_1; i >= 0; i--)
	{
		/*string name = place[i].name;
		name = name.substr(1);
		int name_num = atoi(name.c_str());*/
		/*if (name_num < current)
		{*/
			vector<string> v;
			SplitString(place[i].v_name, v, "@");
			if (s == v[v.size() - 1])
			{
				double value = process_get_value(place, i, current, array_num);
				return value;
			}
		//}
	}

	//cout << "afterward get_value" << endl;

	//for (int i = place.size() - 1; i >= current_1; i--)
	//{
	//	//string name = place[i].name;
	//	//name = name.substr(1);
	//	//int name_num = atoi(name.c_str());

	//	vector<string> v;
	//	SplitString(place[i].v_name, v, "@");
	//	if (s == v[v.size() - 1])
	//	{
	//		double value = process_get_value(place, i, current, array_num);
	//		return value;
	//	}

	//}

	cout << "get_value error!" << endl;
	exit(1);
}

double cal(double top_1, double top_2, string s)
{
	if (s == "+")
		return top_1 + top_2;
	else if (s == "-")
		return top_1 - top_2;
	else if (s == "*")
		return top_1 * top_2;
	else if (s == "/")
		return top_1 / top_2;
	else if (s == "&")
		return int(top_1) & int(top_2);
	else if (s == "|")
		return int(top_1) | int(top_2);
	else if (s == "^")
		return int(top_1) ^ int(top_2);
	else if (s == "%")
		return int(top_1) % int(top_2);
	else if (s == "!")
	{
		if (top_1 - 0 < 1e-6)
			return 1;
		else
			return 0;
	}
	else if (s == "@")
		return -top_1;
	else if (s == "&&")
		return top_1 && top_2;
	else if (s == "||")
		return top_1 || top_2;
	else if (s == ">=")
		return top_1 >= top_2;
	else if (s == "<=")
		return top_1 <= top_2;
	else if (s == ">")
		return top_1 > top_2;
	else if (s == "<")
		return top_1 < top_2;
	else if (s == "==")
		return top_1 == top_2;
	else if (s == "!=")
		return top_1 != top_2;
	else
		return -1;
}

bool is_unary(string s)
{
	if (s == "!" || s == "@" )
		return true;
	else
		return false;
}

void process_stack(stack<double> &Stack, stack<string> &Stack_c)
{
	string s = Stack_c.top();
	Stack_c.pop();
	if (!is_unary(s))
	{
		double top_1 = Stack.top();
		Stack.pop();
		double top_2 = Stack.top();
		Stack.pop();
		double res = cal(top_2, top_1, s);
		Stack.push(res);
	}
	else
	{
		double top_1 = Stack.top();
		Stack.pop();
		double res = cal(top_1, top_1, s);
		Stack.push(res);
	}
}

double change_and_cal(string s, vector<Place> place, int current)//����׺ʽsת���ɺ�׺ʽ���ҽ��������д���
{
	if (s == "" || s == "executed")
		return 0;
	string temp_s;
	string translator;

	stack<double> Stack;
	stack<string> Stack_c;
	Stack_c.push("#");
	s = s + "#";
	bool ignore = false;
	for (unsigned int i = 0; i < s.length(); i++)
	{
		if (s[i] == ' ')
			continue;
		if (s[i] == '[')
			ignore = true;
		if (s[i] == ']')
			ignore = false;
		int count = is_operator(s, i);
		if (count != 0 && ignore == false)
		{
			if (temp_s != "")
			{
				double value = get_value(temp_s, place, current);
				temp_s.clear();
				Stack.push(value);
			}
			else if (s[i] == '-')
			{
				Stack_c.push("@");
				continue;
			}
			if (s[i] == '(')
			{
				translator = s[i];
				Stack_c.push(translator);
			}
			else if (s[i] == ')')
			{
				while (Stack_c.top() != "(")
				{
					process_stack(Stack, Stack_c);
				}
				Stack_c.pop();
			}
			else
			{
				if (count == 1)
					translator = s[i];
				else
				{
					translator = s[i];
					translator.append(1, s[i + 1]);
				}
				while (priority(translator) <= priority(Stack_c.top()))
				{
					if (s[i] == '#' && Stack_c.top() == "#")
					{
						double result = Stack.top();
						Stack.pop();
						return result;
					}
					process_stack(Stack, Stack_c);

				}
				Stack_c.push(translator);
			}
		}
		else
			temp_s.append(1, s[i]);
		if (count == 2)
			i++;
	}
	cout << "change error!" << endl;
	return -1;
}

bool C_Petri::guard(string booleanExpression, string T)
{
	return change_and_cal(booleanExpression, place, get_current_P_num(T));
}//a&&b||c&&(d||e)

vector<string> C_Petri::find_all_place(string t_name)
{
	vector<string> v;

	for (int i = 0; i < arcnum; i++)
	{
		if (arc[i].sourceP == true && arc[i].V != "#" && arc[i].V != "executed#"&&arc[i].V != "relation")//��ʽ�����ϵ��
			if (arc[i].target == t_name)
				v.push_back(arc[i].source);
	}
	return v;
}

string C_Petri::Delete_Arc(int num)
{
	int i = 0;
	for (vector<Arc>::iterator it = arc.begin(); it != arc.end(); it++, i++)
	{
		if (i == num)
		{
			//			arc.erase(it);
			//			arcnum--;
			string temp = it->V;
			if (it->V == "executed")
				it->V = "executed#";
			else
				it->V = "#";
			return temp;
		}
	}
	return "";
}

string C_Petri::Delete_Arc(string source, string target)
{
	for (vector<Arc>::iterator it = arc.begin(); it != arc.end(); it++)
	{
		if (it->source == source && it->target == target)
		{
			//			arc.erase(it);
			//			arcnum--;
			string temp = it->V;
			if (it->V == "executed")
				it->V = "executed#";
			else
				it->V = "#";
			return temp;
		}
	}
	return "";
}

void C_Petri::Change_Arc(int num, string source, string target, string V, bool sourceP)
{
	for (int i = 0; i < arcnum; i++)
	{
		if (i == num)
		{
			arc[i].source = source;
			arc[i].target = target;
			arc[i].V = V;
			arc[i].sourceP = sourceP;
		}
	}
}

void C_Petri::delete_all_connected_arc(string place)
{
	for (int i = 0; i < arcnum; i++)
	{
		if (arc[i].source == place || arc[i].target == place)
		{
			Delete_Arc(i);
		}
	}
}

int C_Petri::get_current_P_num(string T)
{
	for (int i = 0; i < t_num; i++)
	{
		if (transition[i].name == T)
			return transition[i].current_P_num;
	}
	cout << "get_current_P_num error" << endl;
	exit(1);
}

void C_Petri::set_point_flag(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			place[i].ispoint = true;
			return;
		}
	}
}

bool C_Petri::get_point_flag(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			return place[i].ispoint;
		}
	}
	cout << "get_point_flag error!" << endl;
	exit(1);
}

void C_Petri::set_false_exit(string p_name,vector<string> false_exit)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			place[i].false_exit = false_exit;
			return;
		}
	}
}

vector<string> C_Petri::get_false_exit(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			return place[i].false_exit;
		}
	}
	cout << "get_false_exit error!" << endl;
	exit(1);
}

void C_Petri::set_enter_P(string p_name, vector<string> enter_P)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			place[i].enter_P = enter_P;
			return;
		}
	}
}

vector<string> C_Petri::get_enter_P(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			return place[i].enter_P;
		}
	}
	cout << "get_enter_P error!" << endl;
	exit(1);
}

void C_Petri::set_control_T(string p_name, vector<string> control_T)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			place[i].control_T = control_T;
			return;
		}
	}
}

vector<string> C_Petri::get_control_T(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			return place[i].control_T;
		}
	}
	cout << "get_control_T error!" << endl;
	exit(1);
}

void C_Petri::add_call_P(string p_name, string call_P)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			place[i].call_P.push_back(call_P);
			return;
		}
	}
}

vector<string> C_Petri::get_call_P(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			return place[i].call_P;
		}
	}
	cout << "get_call_P error!" << endl;
	exit(1);
}

void C_Petri::set_fun_P(string p_name, string fun_P)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			place[i].fun_P = fun_P;
			return;
		}
	}
}

string C_Petri::get_fun_P(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			return place[i].fun_P;
		}
	}
	cout << "get_fun_P error!" << endl;
	//exit(1);
}

void C_Petri::set_pre_executed_P(string p_name, string executed_P)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			place[i].pre_executed_P = executed_P;
			return;
		}
	}
}

string C_Petri::get_pre_executed_P(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			return place[i].pre_executed_P;
		}
	}
	cout << "get_pre_executed_P error!" << endl;
	exit(1);
}

void C_Petri::set_arc_type(string source, string target, int type)
{
	for (int i = arcnum - 1; i >= 0; i--)
	{
		if (arc[i].source == source && arc[i].target == target)
		{
			arc[i].type = type;
			return;
		}
	}
}

int C_Petri::get_arc_type(string source, string target)
{
	for (int i = 0; i < arcnum; i++)
	{
		if (arc[i].source == source && arc[i].target == target)
		{
			
			return arc[i].type;
		}
	}
}

void C_Petri::add_label_P(string p_name, string label_P)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			place[i].label_after_P.push_back(label_P);
			return;
		}
	}
}

vector<string> C_Petri::get_label_P(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			return place[i].label_after_P;
		}
	}
	cout << "get_label_P error!" << endl;
	exit(1);
}

void C_Petri::clear_enter(string p_name)
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].name == p_name)
		{
			place[i].enter.clear();
			return ;
		}
	}
	cout << "clear_enter error!" << endl;
	exit(1);
}

int gen_P_num = 0, gen_T_num = 0;//���ɿ����ͱ�Ǩ�����±�
bool control_P = true;
bool control_T = true;
bool t = false;
bool sourceP = true;
bool ispoint = false;
int n1 = 0;
int array_size;
double d = 0.0;
string tag = "";
string V_name = "";
string s = "";
string P1 = "";
string T = "";
string V = "";


string gen_P()
{
	string temp = "";
	return temp + 'P' + to_string(gen_P_num++);

}
string gen_T()
{
	string temp = "";
	return temp + 'T' + to_string(gen_T_num++);
}
string get_gen_P()
{
	string temp = "";
	return temp + 'P' + to_string(gen_P_num);
}
string get_gen_T()
{
	string temp = "";
	return temp + 'T' + to_string(gen_T_num);
}
void reset_gen_cpn()
{
	gen_P_num = 0;
	gen_T_num = 0;
	for (unsigned int i = 0; i < v_tables.size(); i++)
	{
		delete v_tables[i];
	}
	v_tables.clear();
}
int find_P_exist(vector<Place> place, string s)//�ҿ������Ƿ���v_name����s�ģ����ҷ����м���
{
	vector<string> v;
	for (int i = place.size() - 1; i >= 0; i--)
	{
		v.clear();
		SplitString(place[i].v_name, v, "#");
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

string find_executedP(C_Petri petri, string T)//������Ǩ����Ѱ��ָ������ִ�п�����û�з���""
{
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].V == "executed" && petri.arc[i].sourceP == true && petri.arc[i].target == T && petri.arc[i].V != "#")
		{
			for (int j = 0; j < petri.p_num; j++)
			{
				if (petri.place[j].name == petri.arc[i].source)
					if (petri.place[j].v_name == "executedP")
						return petri.arc[i].source;
			}
		}
	}
	return "";
}
string find_P_name(C_Petri petri, string v_name)//ͨ��������v_name�ҿ�����name
{

	for (int i = petri.p_num - 1; i >= 0; i--)
	{
		string s = petri.place[i].v_name;
		vector<string> v;
		SplitString(s, v, "@");

		int pos = (v.size() - 1);
		if (pos >= 0 && v[pos] == v_name)
			return petri.place[i].name;
	}
	cout << "can't find P" << endl;
	return "";
}
string find_P_name_1(C_Petri petri, string v_name, int current)//ͨ��������v_name�ҿ�����name
{
	/*int current_1 = petri.place.size() - 1;
	for (int i = 0; i<int(petri.place.size()); i++)
	{
		int p_num = atoi(petri.place[i].name.substr(1).c_str());
		if (p_num > current)
		{
			current_1 = i - 1;
			break;
		}
	}*/
	int current_1 = petri.place.size() - 1;
	for (int i = petri.p_num - 1; i > 0; i--)
	{
		if (petri.place[i].id_num <= current)
		{
			current_1 = i;
			break;
		}
	}
	for (int i = current_1; i >= 0; i--)
	{
		string s = petri.place[i].v_name;
		vector<string> v;
		SplitString(s, v, "@");

		int pos = v.size() - 1;
		if (pos >= 0 && v[pos] == v_name)
			return petri.place[i].name;
	}

	/*cout << "afterward find_P_name1" << endl;

	for (unsigned int i = petri.place.size() - 1; i >=current_1 ; i--)
	{
		string s = petri.place[i].v_name;
		vector<string> v;
		SplitString(s, v, "@");

		int pos = v.size() - 1;
		if (pos >= 0 && v[pos] == v_name)
			return petri.place[i].name;
	}*/

	cout << "can't find P" << endl;
	return "";
}
string find_T_name(C_Petri petri, string v_name)//ͨ�����ʽ��v_name�ұ�Ǩ��name
{
	for (int i = petri.t_num - 1; i >= 0; i--)
	{
		string s = petri.transition[i].v_Expression;
		string s1 = petri.transition[i].booleanExpression;
		if (s == v_name || s1 == v_name)
			return petri.transition[i].name;
	}
	cout << "can't find T" << endl;
	return "";
}
bool create_connect(C_Petri &petri, string T, string express, int current)//������Ǩ�ͱ��ʽ�������ʽ�����п������Ǩ����ϵ��������к���ȫ�ֱ����򷵻�true
{
	string P2;
	vector<string> v;
	string V;
	bool sourceP;
	bool global = false;
	splitExpression(express, v);
	for (unsigned int i = 0; i < v.size(); i++)
	{
		if (v[i][0] == '_' || (v[i][0] >= 'a'&&v[i][0] <= 'z') || (v[i][0] >= 'A'&&v[i][0] <= 'Z'))
		{
			int position = 0;
			for (unsigned int j = 0; j < v[i].length(); j++)
			{
				if (v[i][j] == '[')
				{
					position = j;
					break;
				}
			}
			if (position != 0)
				v[i] = v[i].substr(0, position);
			string temp = v[i], temp_s = "";
			if (temp.size() >= 2)
			{
				char temp_c = temp[temp.size() - 2];
				temp_s.append(1, temp_c);
				temp_c = temp[temp.size() - 1];
				temp_s.append(1, temp_c);
			}
			P2 = find_P_name_1(petri, v[i], current);
			bool is_point = petri.get_point_flag(P2);
			if (is_point)
				V = "";
			else
				V = v[i];
			
			
			sourceP = true;
			petri.Add_Arc(P2, T, V, sourceP);//_v����
			sourceP = false;
			//			V = "";
			petri.Add_Arc(T, P2, V, sourceP);
			if (petri.get_global(P2))
				global = true;
			
			if (temp_s == "_v")
			{
				petri.set_arc_type(P2, T, 2);
				petri.set_arc_type(T, P2, 2);
			}

			
		}
	}
	return global;
}
void inside_block(C_Petri &petri, gtree *tree1, string T)//compound_statement��ģ������ڲ���ģ����Ҫ�����Ƿ���ڹ��̵���
{
	gtree *tr;
	int sum = 0;
	//bool flag = false;
	if (tree1->child->next->type == STATEMENT_LIST)//ֻ�����
	{
		tr = tree1->child->next;
	}
	else if (tree1->child->next->next->type == STATEMENT_LIST)//����������ж���
	{
		tr = tree1->child->next->next;
	}
	else//ֻ�ж���
	{
		cout << "�ݲ�����ֻ�ж�������!" << endl;
		exit(1);
	}
	while (tr->type != STATEMENT)
		tr = tr->child;

	vector<string> now;
	vector<string> last;
	while (tr)
	{
		now.clear();
		/*if (tr->child->type == SELECTION_STATEMENT || tr->child->type == ITERATION_STATEMENT
			|| judge_assign_statement(tr) || judge_call_statement(tr) || judge_return_statement(tr))*/
		/*if(tr->type==STATEMENT)
		{*/

			bool control_P, t;
			int n1 = 0;
			double d = 0.0;
			string tag;
			string V = "", V_name = "";
			string s = "";
			string _P = find_P_name(petri, tr->place);
			vector<string> enter_P = petri.get_enter_P(_P);
			bool sourceP = false;

			if (enter_P.size() == 0)
			{
				if (tr->parent->next->type == STATEMENT)
					tr = tr->parent->next;
				else
					break;
				continue;
			}
			for (unsigned int i = 0; i < enter_P.size(); i++)
				petri.Add_Arc(T, enter_P[i], V, sourceP);
			if (enter_P[0] != _P)
				petri.Add_Arc(T, _P, V, sourceP);
			//int flag = petri.get_call_flag(_P);

			now = petri.get_enter(_P);
			if (sum == 0)
			{
				sum++;
			}
			else
			{
				string newP = gen_P();
				V_name = "executedP";
				tag = "";
				control_P = true;
				t = false;
				ispoint = false;
				petri.Add_Place(newP, V_name, tag, control_P, t, n1, d, s, 0, ispoint);

				
				petri.set_pre_executed_P(_P, newP);

				V = "executed";

				sourceP = false;
				for (unsigned int i = 0; i < last.size(); i++)
					petri.Add_Arc(last[i], newP, V, sourceP);

				//break��䲻������
				//if (tr->child->type == break���)
				//	break;


				sourceP = true;
				for (unsigned int i = 0; i < now.size(); i++)
					petri.Add_Arc(newP, now[i], V, sourceP);
				if (tr->child->type == ITERATION_STATEMENT)//ѭ�����ǰ��ִ�п���������
				{
					vector<string> false_exit = petri.get_false_exit(_P);
					for (unsigned int i = 0; i < false_exit.size(); i++)
					{
						sourceP = false;
						petri.Add_Arc(false_exit[i], newP, V, sourceP);
					}
				}

			}

			last = petri.get_exit(_P);
			
		//}
		if (tr->parent->next->type == STATEMENT)
			tr = tr->parent->next;
		else
			break;
		/*if (flag == true)
			break;*/
	}
}
void inside_block1(C_Petri &petri, gtree *tree1, string T)//compound_statement��ģ������ⲿ��ģ��Ҫ�����Ƿ���ڹ��̵���
{
	gtree  *tr;
	int sum = 0;//��������begin�Ƿ����ӵ����ƿ���

	if (tree1->child->next->type == STATEMENT_LIST)//ֻ�����
	{
		tr = tree1->child->next;
	}
	else if (tree1->child->next->next->type == STATEMENT_LIST)//����������ж���
	{
		tr = tree1->child->next->next;
	}
	else//ֻ�ж���
	{
		cout << "�ݲ�����ֻ�ж�������!" << endl;
		exit(1);
	}
	while (tr->type != STATEMENT)
		tr = tr->child;
	vector<string> now;
	vector<string> last;
	while (tr)
	{


		/*if (tr->child->type == SELECTION_STATEMENT || tr->child->type == ITERATION_STATEMENT
			|| judge_assign_statement(tr) || judge_call_statement(tr) || judge_return_statement(tr))*/
		if (tr->type == STATEMENT)
		{
			

			int n1 = 0;
			double d = 0.0;
			string tag;
			string V = "", V_name = "";
			string s = "";
			string _P = find_P_name(petri,tr->place);
			vector<string> enter_P = petri.get_enter_P(_P);
			bool sourceP = false;


			if (sum == 0)//��һ�������ƻ�
			{
				/*if (flag_call == false)
				{*/
				V = "executed";

				//_P = find_P_name(petri, tr->place);
				for (unsigned int i = 0; i < enter_P.size(); i++)
					petri.Add_Arc(T, enter_P[i], V, sourceP);
				//}
				sum++;
			}

			now = enter_P;

			if (last.size() != 0)// && flag_call == false)
			{
				for (unsigned int i = 0; i < last.size(); i++)
				{
					V = "executed";

					sourceP = false;
					for (unsigned int j = 0; j < now.size(); j++)
						petri.Add_Arc(last[i], now[j], V, sourceP);
				}

			}
			last = petri.get_exit(_P);
		}
		if (tr->parent->next!=NULL && tr->parent->next->type == STATEMENT)
			tr = tr->parent->next;
		else
			break;

	}
}

bool judge_bool(string s)
{
	bool ignore = false;
	for (unsigned int i = 0; i < s.length() - 1; i++)
	{
		if (s[i] == '(')
			ignore = true;
		else if (s[i] == ')')
			ignore = false;
		if (ignore == false && (s[i] == '>' || s[i] == '<'))
			return true;
		if (ignore == false && (s[i] == '=' || s[i] == '!'))
			if (s[i + 1] == '=')
				return true;
	}
	return false;
}

string opposite(string s)
{
	if (s.find(">=") != string::npos)
		string_replace(s, ">=", "<");
	else if (s.find("<=") != string::npos)
		string_replace(s, "<=", ">");
	else if (s.find("<") != string::npos)
		string_replace(s, "<", ">=");
	else if (s.find(">") != string::npos)
		string_replace(s, ">", "<=");
	else if (s.find("==") != string::npos)
		string_replace(s, "==", "!=");
	else if (s.find("!=") != string::npos)
		string_replace(s, "!=", "==");
	else//�����ȽϷ�
	{

	}
	if (s.find("&&") != string::npos)
		string_replace(s, "&&", "||");
	else if (s.find("||") != string::npos)
		string_replace(s, "||", "&&");

	return s;

}
string opposite_all(string s)
{
	if (!judge_bool(s))
		return "!(" + s + ")";
	vector<string> v;
	vector<string>v1;
	string temp_s;
	string result;
	for (unsigned int i = 0; i < s.size(); i++)
	{
		if (s[i] == '&')
		{
			v1.push_back("||");
			v.push_back(temp_s);
			temp_s.clear();
			i += 2;
		}
		else if (s[i] == '|')
		{
			v1.push_back("&&");
			v.push_back(temp_s);
			temp_s.clear();
			i += 2;
		}
		temp_s += s[i];
	}
	v.push_back(temp_s);
	for (unsigned int i = 0; i < v.size(); i++)
	{
		v[i] = opposite(v[i]);
	}
	result = v[0];
	for (unsigned int i = 0; i < v1.size(); i++)
	{
		result += v1[i] + v[i + 1];
	}
	return result;
}

gtree* find_last_sentence(gtree *node, bool &flag, bool &flag1)//nodeָ��statement�ڵ㣬�����ҵ�����һ��statement�ڵ��ָ��,��һ��Ϊ�������flagΪtrue,flag1Ϊtrue����else������
{
	flag = false;
	flag1 = false;
	gtree *p = node;
	gtree *test_else;
	if (p->parent->type == SELECTION_STATEMENT)
	{
		test_else = p->parent->child;
		while (test_else->next != p)
			test_else = test_else->next;
		if (test_else->type == ELSE)//else
			flag1 = true;
		else
			flag1 = false;
		flag = true;
		return p->parent->parent;
	}
	else if (p->parent->type == ITERATION_STATEMENT)
	{
		flag = true;
		flag1 = false;
		return p->parent->parent;
	}
	else//��statement_list�Ƴ�
	{
		gtree *p1 = p->parent->child;
		
		if (p1 == p)//��һ��
		{
			gtree *temp_compound_expression = p;
			while (temp_compound_expression->type != COMPOUND_STATEMENT)
				temp_compound_expression = temp_compound_expression->parent;
			if (temp_compound_expression->parent->type == FUNCTION_DEFINITION)
				return NULL;
			else
			{
				test_else = temp_compound_expression->parent->parent->child;
				while (test_else->next != temp_compound_expression->parent)
					test_else = test_else->next;
				if (test_else->type == ELSE)//else
					flag1 = true;
				else
					flag1 = false;
				flag = true;
				return temp_compound_expression->parent->parent->parent;
			}
		}
		else//�ǵ�һ�䣬����ʵflag��flag1������
		{
			if (p1->child->type == STATEMENT)
				p1 = p1->child;
			else
				p1 = p1->child->next;

			//gtree *last_point = p1;
			//while (1)
			//{
			//	if (p1->next->type != ��� || p1->next->child->type == ���ʽ)
			//		p1 = p1->next->next;
			//	else
			//	{
			//		last_point = p1;
			//		p1 = p1->next;
			//	}
			//	if (p1 == p)
			//		break;
			//}
			return p1;
		}
	}

}

bool judge_inside(gtree *temp_statement)
{
	if (temp_statement->parent->type == SELECTION_STATEMENT || temp_statement->parent->type == ITERATION_STATEMENT)
		return true;
	else
	{
		gtree *temp_compound = temp_statement;
		while (temp_compound->type != COMPOUND_STATEMENT)
			temp_compound = temp_compound->parent;
		if (temp_compound->parent->type == FUNCTION_DEFINITION)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
}


void process_declarator(gtree *declarator, C_Petri &petri, string tag, string base, bool para, int current)//����declarator��para�����β��е�declarator
{
	bool call_declare_flag = false;
	gtree *identifier;
	if (declarator->child->next == NULL)
		identifier = declarator->child;
	else
		identifier = declarator->child->next;
	while (identifier->type != IDENTIFIER)
		identifier = identifier->child;
	if (base != "")
		V_name = base + '@' + identifier->place;
	else
		V_name = identifier->place;
	control_P = false;

	if (declarator->next == NULL || declarator->next->type != REMAIN || declarator->next->place != "=")//�޳�ʼ��
		t = false;
	else//�г�ʼ��,�ݲ�֧�������ʼ��
	{
		t = true;
		gtree *init = declarator->next->next;
		string temp_s;
		if (init->child->type == ASSIGNMENT_EXPRESSION)
			temp_s = init->child->place;
		else
		{
			cout << "�ݲ�֧�������ʼ��!" << endl;
			exit(1);
		}

		t = true;
		switch (tag[tag.size() - 1])
		{
		case '1':
		case '5':
		case '6':
			n1 = int(change_and_cal(temp_s, petri.place, gen_P_num));
			break;
		case '3':
		case '4':
			d = change_and_cal(temp_s, petri.place, gen_P_num);
			break;
		case '2':
			s = temp_s;
			break;
		default:
			cout << "declarate type error!" << endl;
			exit(1);
			break;
		}
	}
	if (declarator->child->type == POINTER)
		ispoint = true;
	else
		ispoint = false;
	bool array_flag = false;
	if (identifier->parent->next != NULL && identifier->parent->next->type == REMAIN && identifier->parent->next->place == "[")//����
	{
		array_flag = true;
		if (identifier->parent->next->next->type == CONSTANT_EXPRESSION && para == false)
			array_size = atoi(identifier->parent->next->next->child->place.c_str());
		else if(identifier->parent->next->next->type == CONSTANT_EXPRESSION && para == true)
			array_size = 1;
		else
		{
			cout << "�ݲ�֧�������С��ָ����" << endl;
			exit(1);
		}
	}
	else if (identifier->parent->next != NULL && identifier->parent->next->type == REMAIN && identifier->parent->next->place == "(")//��������
		call_declare_flag = true;
	else//������Ǻ�������
	{
		array_size = 1;
	}
	if (!call_declare_flag)
	{
		if (array_flag == true && para == true)
			ispoint = true;

		string _P = gen_P();
		petri.Add_Place(_P, V_name, tag, control_P, t, n1, d, s, array_size, ispoint);

		gtree *compound = declarator;
		string table_name;
		while (compound != NULL && compound->type != COMPOUND_STATEMENT)
			compound = compound->parent;
		if (compound == NULL)
			table_name = "global";
		else
			table_name = compound->place;
		for (unsigned int i = 0; i < v_tables.size(); i++)
		{
			if (table_name == v_tables[i]->name)
			{
				v_tables[i]->insert(V_name, _P);
				break;
			}
		}

		if (current != 0)
		{
			for (unsigned int i = 0; i < petri.p_num; i++)
				if (petri.place[i].name == _P)
					petri.place[i].id_num = current;
		}
	}

}

void process_declaration(gtree *declaration, C_Petri &petri, string base,int current)
{
	/*if (declaration->child->next->type != INIT_DECLARATOR_LIST)
	{
		cout << "�ն��壬ֻ������û�б�����!" << endl;
		exit(1);
	}*/
	gtree *init_declarator = declaration->child->next;
	if (declaration->child->type == STORAGE_CLASS_SPECIFIER)
	{

	}
	else
	{
		tag = declaration->child->place;
		while (init_declarator->type != INIT_DECLARATOR)
			init_declarator = init_declarator->child;

		process_declarator(init_declarator->child, petri, tag, base, false, current);
		gtree *temp;
		while (init_declarator->parent->next != NULL && init_declarator->parent->next->type == REMAIN && init_declarator->parent->next->place == ",")
		{
			init_declarator = init_declarator->parent;
			temp = init_declarator->next->next;
			process_declarator(temp->child, petri, tag, base, false, current);
		}
	}
}

void process_para_type_list(gtree *para_type_list, C_Petri &petri, string base_Vname)
{
	gtree *para_list;
	if (para_type_list->child->next != NULL)//ELLIPSIS�ݲ�����
	{
		cout << "ELLIPSIS�ݲ����ǣ�" << endl;
		exit(1);
	}
	else
		para_list = para_type_list->child;

	gtree *para_declaration = para_list->child;
	while (para_declaration->type != PARAMETER_DECLARATION)
		para_declaration = para_declaration->child;
	
	tag = para_declaration->child->place;
	if (para_declaration->child->next == NULL || para_declaration->child->next->type == ABSTRACT_DECLARATOR)
	{
		if (para_declaration->child->place == "7")
			return;
		cout << "���������ݲ�����!" << endl;
		exit(1);
	}
	process_declarator(para_declaration->child->next, petri, tag, base_Vname, true, 0);

	gtree *temp;
	while (para_declaration->parent->next != NULL && para_declaration->parent->next->type == REMAIN && para_declaration->parent->next->place == ",")
	{
		para_declaration = para_declaration->parent;
		temp = para_declaration->next->next;
		tag = temp->child->place;
		if (temp->child->next == NULL || temp->child->next->type == ABSTRACT_DECLARATOR)
		{
			if (temp->child->place == "7")
				return;
			cout << "���������ݲ�����!" << endl;
			exit(1);
		}
		process_declarator(temp->child->next, petri, tag, base_Vname, true, 0);
	}
}



vector<string> get_statement_exit(gtree *statement1, C_Petri &petri)
{
	string statement_P;
	vector<string> temp_v1;
	gtree *last_statement;
	if (statement1->child->type == COMPOUND_STATEMENT)
	{
		if (statement1->child->child->next->type == STATEMENT_LIST)//ֻ�����û�ж���
		{
			if (statement1->child->child->next->child->type == STATEMENT)
			{
				last_statement = statement1->child->child->next->child;
				//last_statement = get_real_laststatement(last_statement);
			}
			else
			{
				last_statement = statement1->child->child->next->child->next;
				//last_statement = get_real_laststatement(last_statement);
				//temp_v1 = petri.get_exit(statement_P);
			}
		}
		else if (statement1->child->child->next->type == DECLARATION_LIST && statement1->child->child->next->next->type == STATEMENT_LIST)//������ж���
		{
			if (statement1->child->child->next->next->child->type == STATEMENT)
			{
				last_statement = statement1->child->child->next->next->child;
				//last_statement = get_real_laststatement(last_statement);
				//temp_v1 = petri.get_exit(statement_P);
			}
			else
			{
				last_statement = statement1->child->child->next->next->child->next;
				//last_statement = get_real_laststatement(last_statement);
				//temp_v1 = petri.get_exit(statement_P);
			}
		}
		else//ֻ�ж���
		{
			cout << "ֻ�ж���û������!" << endl;
			exit(1);
		}
		if (last_statement != NULL)
		{
			statement_P = find_P_name(petri, last_statement->place);
			temp_v1 = petri.get_exit(statement_P);
		}
		else
		{
			statement_P = find_P_name(petri, statement1->place);
			temp_v1 = petri.get_enter(statement_P);
		}
	}
	else if (statement1->child->type == ITERATION_STATEMENT || statement1->child->type == SELECTION_STATEMENT
		|| statement1->child->type == JUMP_STATEMENT || statement1->child->type == LABELED_STATEMENT)//�������
	{
		statement_P = find_P_name(petri, statement1->child->place);
		temp_v1 = petri.get_exit(statement_P);
	}
	else if (judge_assign_statement(statement1))
	{
		statement_P = find_P_name(petri, statement1->child->child->child->place);
		temp_v1 = petri.get_exit(statement_P);
	}
	return temp_v1;
}

void process_label(C_Petri &petri)
{
	for (int i = 0; i < petri.p_num; i++)
	{
		if (petri.place[i].is_goto == true)
		{
			string goto_T = find_T_name(petri, petri.place[i].v_name);
			string label = petri.place[i].goto_label;
			if (label == "default1")
				label = "default";
			label = " " + label;
			for (int j = 0; j < petri.p_num; j++)
				if (petri.place[j].v_name == label)
				{
					//gotoָ��label���
					for (unsigned int k = 0; k < petri.place[j].enter_P.size(); k++)
						petri.Add_Arc(goto_T, petri.place[j].enter_P[k], "", false);
					if (petri.place[j].pre_executed_P != "")
						petri.Add_Arc(goto_T, petri.place[j].pre_executed_P, "", false);
					//gotoָ��label������
					for (unsigned int k = 0; k < petri.place[j].label_after_P.size(); k++)
						petri.Add_Arc(goto_T, petri.place[j].label_after_P[k], "", false);
				}
		}
	}
}

void ast_to_cpn(C_Petri &petri, gtree *p, int addition)//additionΪ0��ʾֱ�ӹ����������ʾ����仯ʱ����
{
	if (p == NULL) return;
	tag = "";
	control_P = true;
	control_T = true;
	t = false;
	sourceP = true;
	n1 = 0;
	d = 0.0;
	V_name = "";
	s = "";
	P1 = "";
	T = "";
	V = "";
	p->record_P_num = gen_P_num;
	/*if (p->type == COMPOUND_STATEMENT)
	{
		p->record_P_num = gen_P_num;
	}
	else */if (p->type == DECLARATION)
	{
		control_P = false;
		//V_name = p->place;
		//cout << p->place << endl;
		//getchar();
		gtree *p1 = p->parent;
		string func = "";
		while (p1 != NULL && p1->type != COMPOUND_STATEMENT)
			p1 = p1->parent;
		if (p1 != NULL)
			func = p1->place;
		process_declaration(p, petri, func, addition);
	}
	else if (p->type == FUNCTION_DEFINITION)
	{
		int current;
		if (addition == 0)
			current = gen_P_num;
		else
			current = addition;
		string ret_tag;
		if (p->child->type == DECLARATION_SPECIFIERS)
		{
			ret_tag = p->child->place;
		}
		else
		{
			cout << "�ݲ�֧�ֶ��庯��δ���巵������!" << endl;
			exit(1);
		}

		//����begin�Ŀ���
		control_P = true;
		control_T = false;
		tag = "";
		t = false;
		sourceP = true;

		P1 = gen_P();
		string begin_P = P1;
		T = gen_T();
		string func;
		if (p->child->next->child->type == POINTER)
			func = p->child->next->child->next->child->child->place;
		else
			func = p->child->next->child->child->child->place;
		V_name = func + " begin";
		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, 0, false);
		s = func + " begin";

		//transition���ں��棬Ϊ���ܹ��ں�����ʹ���β�


		if (p->child->next->child->type == POINTER)
		{
			//����ָ�������
		}
		else
		{
			gtree *direct_declarator = p->child->next->child;
			if (direct_declarator->child->next->type == REMAIN && direct_declarator->child->next->place == "(")//������������βο���
			{
				if (direct_declarator->child->next->next->type == PARAMETER_TYPE_LIST)
				{
					gtree *para_type_list = direct_declarator->child->next->next;//������

					process_para_type_list(para_type_list, petri, func);
				}
				else//�޲κ���
				{

				}
			}
			else
			{
				cout << "��������ȱ������!" << endl;
				exit(1);
			}
		}

		string P2 = gen_P();
		V_name = func + " end";
		tag = ret_tag;
		control_P = true;
		t = false;
		petri.Add_Place(P2, V_name, tag, control_P, t, n1, d, s, 0, false);
		string P3;
		if (tag[tag.size() - 1] != '7')//void��������
		{
			P3 = gen_P();
			V_name = func + "_v";//func_v���ڴ�ŷ���ֵ
			control_P = false;
			t = false;
			petri.Add_Place(P3, V_name, tag, control_P, t, n1, d, s, 1, false);//������ʱ��֧��ָ��
			//petri.set_call_flag(P3, 2);//_v����
		}

		petri.Add_Transition(T, control_T, s, current);//beginǰ�пո�
		petri.Add_Arc(begin_P, T, V, sourceP);

	}
	else if ((p->type == ASSIGNMENT_EXPRESSION && p->child->next!=NULL) 
	|| p->type == SELECTION_STATEMENT || p->type == ITERATION_STATEMENT 
	|| judge_return_statement(p) || judge_break_statement(p) || judge_goto_statement(p)||judge_label_statement(p))//��ֵ������������ѭ�����
	{
		
		P1 = gen_P();
		control_P = true;
		t = false;
		V_name = p->place;
		int judge = find_P_exist(petri.place, V_name);
		if (judge != 0)
		{
			char temp[5];
			_itoa(judge, temp, 10);
			V_name = V_name + "#" + temp;
			p->place = V_name;
			gtree *temp_p = p;
			while (temp_p->type != STATEMENT)
				temp_p = temp_p->parent;

			temp_p->place = V_name;

		}
		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, 0, false);

		vector<string> temp_v;
		temp_v.push_back(P1);
		petri.set_enter_P(P1, temp_v);
		
		int current;
		if (addition == 0)
			current = gen_P_num;
		else
			current = addition;

		if (p->type == SELECTION_STATEMENT || p->type == ITERATION_STATEMENT)
		{
			string T1 = gen_T();
			string T2 = gen_T();
			vector<string> v;
			v.push_back(T1);
			v.push_back(T2);
			control_T = true;

			petri.Add_Transition(T1, control_T, s, current);
			petri.Add_Transition(T2, control_T, s, current);

			petri.Add_Place_enter(P1, v);
			petri.set_control_T(P1, v);//���ÿ��Ʊ�Ǩ
			if (p->type == ITERATION_STATEMENT)//ѭ������end����
			{
				string P2 = gen_P();
				V_name = p->place + " end";
				tag = "";
				petri.Add_Place(P2, V_name, tag, true, false, n1, d, s, 0, false);
				string T3 = gen_T();
				petri.Add_Transition(T3, false, V_name, current);
				petri.Add_Arc(P2, T3, "", true);
				petri.Add_Arc(T2, P2, "", false);
				vector<string> temp_v;
				temp_v.push_back(P2);
				petri.set_enter_P(P2, temp_v);
			}
		}
		else if (p->type == ASSIGNMENT_EXPRESSION && p->child->next != NULL)
		{
			string T1 = gen_T();
			vector<string> v;
			v.push_back(T1);

			control_T = false;
			s = V_name;
			petri.Add_Transition(T1, control_T, s, current);
			petri.Add_Place_enter(P1, v);
			petri.set_control_T(P1, v);//���ÿ��Ʊ�Ǩ
			petri.Add_Place_exit(P1, v);
		}
		else if (judge_return_statement(p))//return
		{
			string T1 = gen_T();
			vector<string> v;
			v.push_back(T1);

			control_T = false;
			s = V_name;
			petri.Add_Transition(T1, control_T, s, current);
			petri.Add_Arc(P1, T1, "", true);
			petri.Add_Place_enter(P1, v);
			petri.set_control_T(P1, v);//���ÿ��Ʊ�Ǩ
			//petri.Add_Place_exit(P1, v);
		}
		else if (judge_break_statement(p))//break
		{
			string T1 = gen_T();
			vector<string> v;
			v.push_back(T1);

			control_T = false;
			s = V_name;
			petri.Add_Transition(T1, control_T, s, current);
			petri.Add_Arc(P1, T1, "", true);
			petri.Add_Place_enter(P1, v);
			petri.set_control_T(P1, v);//���ÿ��Ʊ�Ǩ
			//petri.Add_Place_exit(P1, v);
			gtree *iter = p;
			while (iter->type != ITERATION_STATEMENT)
				iter = iter->parent;
			string end_P = find_P_name(petri, iter->place + " end");
			petri.Add_Arc(T1, end_P, "", false);
		}
		else if (judge_goto_statement(p))
		{
			string T1 = gen_T();
			vector<string> v;
			v.push_back(T1);

			control_T = false;
			s = V_name;
			petri.Add_Transition(T1, control_T, s, current);
			petri.Add_Arc(P1, T1, "", true);
			petri.Add_Place_enter(P1, v);
			petri.set_control_T(P1, v);//���ÿ��Ʊ�Ǩ
			for (int i = 0; i < petri.p_num; i++)
				if (petri.place[i].name == P1)
				{
					petri.place[i].is_goto = true;
					petri.place[i].goto_label = p->child->child->next->place;
				}
			//petri.Add_Place_exit(P1, v);
		}
		else if (judge_label_statement(p))
		{
			string T1 = gen_T();
			vector<string> v;
			v.push_back(T1);

			control_T = false;
			s = V_name;
			petri.Add_Transition(T1, control_T, s, current);
			petri.Add_Arc(P1, T1, "", true);
			petri.Add_Place_enter(P1, v);
			petri.set_control_T(P1, v);//���ÿ��Ʊ�Ǩ
			//petri.Add_Place_exit(P1, v);
		}

		//�����ں���begin�����������fun_P��
		gtree *fun = p;
		while (fun->type != FUNCTION_DEFINITION)
			fun = fun->parent;

		string last_fun = find_P_name(petri, fun->place + " begin");
		petri.set_fun_P(P1, last_fun);

	}
	else if (p->type == COMPOUND_STATEMENT)
	{
		V_Table *table = new V_Table(p->place);
		v_tables.push_back(table);
	}

	ast_to_cpn(petri, p->child, addition);
	
	if (p->type == ASSIGNMENT_EXPRESSION && p->parent->type == EXPRESSION && p->child->type != CONDITIONAL_EXPRESSION)
	{

		//			int array_num = 0;
		//			if (p->child->next->type == ��������)
		//				array_num = int(change_and_cal(p->child->next->next->place, petri.place, gen_P_num));
		P1 = find_P_name(petri, p->place);


		vector<string> temp_v = petri.get_control_T(P1);
		T = temp_v[0];
		control_T = false;



		sourceP = true;
		V = "";
		petri.Add_Arc(P1, T, V, sourceP);


		bool global = create_connect(petri, T, p->place, petri.get_current_P_num(T));
		if (global == true)
			petri.set_global(P1);
		gtree *identifier = p;
		while (identifier->type != IDENTIFIER)
			identifier = identifier->child;
		string P2 = find_P_name(petri, identifier->place);
		//cout << "P2=" << (P2=="P0") << endl;

		if (p->child->child->child->type == PRIMARY_EXPRESSION)//�Ⱥ����ֻ��һ�������ʽ
			V = p->child->next->next->place + "$" + "0";
		else if (p->child->child->child->next->type == REMAIN && p->child->child->child->next->place == "[")
			V = p->child->next->next->place + "$" + identifier->parent->parent->next->next->place;
		bool is_point = petri.get_point_flag(P2);
		if (is_point)
			V = find_P_name_1(petri, p->child->next->next->place, petri.get_current_P_num(T));
		sourceP = false;
		petri.Add_Arc(T, P2, V, sourceP);
		
		//������м���enter��exit
//			petri.Add_Place_enter(P1, T);
//			petri.Add_Place_exit(P1, T);

	}
	else if (p->type == SELECTION_STATEMENT)
	{
		if (p->child->type == SWITCH)//switch����ݲ�����
		{
			cout << "switch����ݲ�����!" << endl;
			exit(1);
		}
		else
		{
			//Ĭ�ϳ�����ȷ
			P1 = find_P_name(petri, p->place);


			vector<string> temp_v = petri.get_control_T(P1);
			T = temp_v[0];
			control_T = true;
			s = p->child->next->next->place;
			//֮ǰ���ñ�Ǩ������bool���ʽ��ֵ
			for (int i = 0; i < petri.t_num; i++)
			{
				if (petri.transition[i].name == T)
				{
					petri.transition[i].booleanExpression = s;
					break;
				}
			}
			//		petri.Add_Transition(T, control_T, s);
			string T1 = temp_v[1];
			bool global = create_connect(petri, T, s, petri.get_current_P_num(T));
			if (global == true)
				petri.set_global(P1);
			s = opposite_all(s);
			for (int i = 0; i < petri.t_num; i++)
			{
				if (petri.transition[i].name == T1)
				{
					petri.transition[i].booleanExpression = s;
					break;
				}
			}
			//		petri.Add_Transition(T1, control_T, s);
			create_connect(petri, T1, s, petri.get_current_P_num(T));
			string T2 = "";
			string T3 = "";
			//string p2 = find_first_yuju(p->child->next->next->next->next);//��if������ĵ�һ�����ʽ
			//string P2 = find_P_name(petri,p2);
			gtree *statement1 = p->child->next->next->next->next;//ָ��if����statement
			string statement_P;
			vector<string> temp_v1, temp_v2;//�����������

			temp_v1 = get_statement_exit(statement1, petri);

			

			if (statement1->child->type == COMPOUND_STATEMENT)
				inside_block(petri, statement1->child, T);
			else
			{
				statement_P = find_P_name(petri, statement1->place);
				vector<string> enter_P = petri.get_enter_P(statement_P);
				for (unsigned int i = 0; i < enter_P.size(); i++)
					petri.Add_Arc(T, enter_P[i], "", false);
				if (enter_P[0] != statement_P)
					petri.Add_Arc(T, statement_P, "", false);
			}

			string t3;

			if (p->child->next->next->next->next->next)
			{
				statement1 = p->child->next->next->next->next->next->next;
				temp_v2 = get_statement_exit(statement1, petri);
				if (statement1->child->type == COMPOUND_STATEMENT)
					inside_block(petri, statement1->child, T1);
				else
				{
					statement_P = find_P_name(petri, statement1->place);
					vector<string> enter_P = petri.get_enter_P(statement_P);
					for (unsigned int i = 0; i < enter_P.size(); i++)
						petri.Add_Arc(T1, enter_P[i], "", false);
					if (enter_P[0] != statement_P)
						petri.Add_Arc(T1, statement_P, "", false);
				}
			}
			sourceP = true;
			V = "";
			petri.Add_Arc(P1, T, V, sourceP);
			petri.Add_Arc(P1, T1, V, sourceP);

			//������м���enter��exit
	//		petri.Add_Place_enter(P1, T);
	//		petri.Add_Place_enter(P1, T1);
			petri.Add_Place_exit(P1, temp_v1);
			if (p->child->next->next->next->next->next)
				petri.Add_Place_exit(P1, temp_v2);
			else
				petri.Add_Place_exit(P1, T1);
		}
	}
	else if (p->type == ITERATION_STATEMENT)
	{
		if (p->child->type == DO)//switch����ݲ�����
		{
			cout << "do while ����ݲ�����!" << endl;
			exit(1);
		}
		else
		{
			P1 = find_P_name(petri, p->place);


			vector<string> temp_v = petri.get_control_T(P1);
			T = temp_v[0];
			control_T = true;
			string s = p->child->next->next->place;

			//petri.Add_Transition(T, control_T, s);
			//֮ǰ���ñ�Ǩ������bool���ʽ��ֵ
			for (int i = 0; i < petri.t_num; i++)
			{
				if (petri.transition[i].name == T)
				{
					petri.transition[i].booleanExpression = s;
					break;
				}
			}
			string T1 = temp_v[1];
			bool global = create_connect(petri, T, s, petri.get_current_P_num(T));
			if (global == true)
				petri.set_global(P1);
			s = opposite_all(s);

			//petri.Add_Transition(T1, control_T, s);
			//֮ǰ���ñ�Ǩ������bool���ʽ��ֵ
			for (int i = 0; i < petri.t_num; i++)
			{
				if (petri.transition[i].name == T1)
				{
					petri.transition[i].booleanExpression = s;
					break;
				}
			}
			create_connect(petri, T1, s, petri.get_current_P_num(T));
			string T2 = "";
			string T3 = "";
			//string p2 = find_first_yuju(p->child->next->next->next->next);//��if������ĵ�һ�����ʽ
			//string P2 = find_P_name(petri,p2);
			gtree *statement1 = p->child->next->next->next->next;//ָ��while����statement
			string statement_P;
			vector<string> temp_v1, temp_v2;//�����������

			temp_v1 = get_statement_exit(statement1, petri);

			///����while���Ƿ��й��̵���
			//int while_flag = petri.get_call_flag(P1);
			//vector<string> while_information = petri.get_information(P1);
			vector<string> enter_P = petri.get_enter_P(P1);
			for (unsigned int i = 0; i < temp_v1.size(); i++)
			{
				V = "";
				sourceP = false;
				//if (while_flag == 1)//while�����д��ڹ��̵���
				//{
				//	for (unsigned int j = 0; j < while_information.size(); j++)
				//		petri.Add_Arc(temp_v1[i], while_information[j], V, sourceP);
				//	if (!judge_inside(p->parent))
				//		;
				//	else
				for (unsigned int j = 0; j < enter_P.size(); j++)
				{
					petri.Add_Arc(temp_v1[i], enter_P[j], V, sourceP);
					petri.set_arc_type(temp_v1[i], enter_P[j], 6);
				}
				if (enter_P.size() == 2)
					;
				else
				{
					petri.Add_Arc(temp_v1[i], P1, V, sourceP);
					petri.set_arc_type(temp_v1[i], P1, 6);
				}
				/*}
				else
					petri.Add_Arc(temp_v1[i], P1, V, sourceP);*/
				

			}
			if (p->child->next->next->next->next->child->type == COMPOUND_STATEMENT)
				inside_block(petri, p->child->next->next->next->next->child, T);
			else
			{
				statement_P = find_P_name(petri, statement1->place);
				vector<string> enter_P = petri.get_enter_P(statement_P);
				for (unsigned int i = 0; i < enter_P.size(); i++)
					petri.Add_Arc(T, enter_P[i], "", false);
				if (enter_P[0] != statement_P)
					petri.Add_Arc(T, statement_P, "", false);
			}

			sourceP = true;
			V = "";
			petri.Add_Arc(P1, T, V, sourceP);
			petri.Add_Arc(P1, T1, V, sourceP);

			//������м���enter��exit
		//	petri.Add_Place_enter(P1, T);
		//	petri.Add_Place_enter(P1, T1);
			petri.set_false_exit(P1, temp_v1);//����temp_v1��ʵ������ʵ���ڣ�ֻ��T1��while���ĳ���

			string end_T = find_T_name(petri, p->place + " end");
			petri.Add_Place_exit(P1, end_T);
		}
	}
	else if (p->type == FUNCTION_DEFINITION)//��������Ĭ�Ϸ��ص����
	{
		string identifier, func;
		identifier = p->place;

		func = identifier + " begin";
		T = find_T_name(petri, func);

		gtree *compound_statement = p->child;
		while (compound_statement->type != COMPOUND_STATEMENT)
			compound_statement = compound_statement->next;
		inside_block1(petri, compound_statement, T);//�����inside_block���в�ͬ


		vector<string> v;
		string begin_place = find_P_name(petri, identifier + " begin");
		//v = petri.get_exit(begin_place);
		//����ʵ����ʽreturn
		//�ҵ�һ������������һ����䣬��������ڼ��뺯������

		gtree *statement_list = compound_statement->child;
		while (statement_list != NULL && statement_list->type != STATEMENT_LIST)
			statement_list = statement_list->next;
		if (statement_list == NULL)
		{
			cout << "���ж��������!" << endl;
			exit(1);
		}
		gtree *statement = statement_list;
		while (statement->type != STATEMENT)
			statement = statement->child;

		bool already_return = false;

		while (statement->parent != statement_list)
		{
			if (judge_return_statement(statement))
			{
				already_return = true;
				break;
			}
			statement = statement->parent->next;
		}
		if (already_return == false)
		{
			if (judge_return_statement(statement))
				already_return = true;
			else
			{
				vector<string> v1 = petri.get_exit(find_P_name(petri, statement->place));
				v.insert(v.end(), v1.begin(), v1.end());
			}
		}

		if (already_return == false)
		{
			string func_end = find_P_name(petri, identifier + " end");
			string func_v = "";
			if (p->child->type == DECLARATION_SPECIFIERS && p->child->child->type == TYPE_SPECIFIER && p->child->child->place == "7")
				;
			else
				func_v = find_P_name(petri, identifier + "_v");
			
			
			for (unsigned int i = 0; i < v.size(); i++)
			{

				V = "executed";
				petri.Add_Arc(v[i], func_end, V, false);
				if (func_v != "")
				{
					petri.Add_Arc(v[i], func_v, "0", false);
					petri.Add_Arc(func_v, v[i], "write", true);//_v������ȫ��Ϊд��
				}
			}
			
		}

	}
	else if (judge_call_postfix_expression(p))//��������ǰ׺���ʽ
	{
		int current;
		if (addition == 0)
			current = gen_P_num;
		else
			current = addition;
		bool noreturn = false;
		//string last_sentence;
		string last_func;
		gtree *find_func = p;
		string this_func_name;
		string new_place;
		while (find_func->type != FUNCTION_DEFINITION)
			find_func = find_func->parent;

		this_func_name = find_func->place;
		last_func = find_P_name(petri, this_func_name + " begin");

		//�ж��Ƿ���������
		string inside = "";
		gtree *temp_statement = p, *temp_expression = NULL;
		bool flag_condition = false;

		while (temp_statement->type != STATEMENT)
		{
			if (temp_statement->type == EXPRESSION)
				temp_expression = temp_statement;
			temp_statement = temp_statement->parent;
		}
		string statement_vname = temp_statement->place;
		if (judge_inside(temp_statement))
			inside = "in";
		else
			inside = "";

		//
		gtree *temp_primary_expression = p;
		while (temp_primary_expression->type != PRIMARY_EXPRESSION)
			temp_primary_expression = temp_primary_expression->child;

		string temp_identifier = temp_primary_expression->child->place;

		
		if (temp_expression->parent->type == SELECTION_STATEMENT || temp_expression->parent->type == ITERATION_STATEMENT)
			flag_condition = true;
		//bool last_father_flag, flag_else;



		//����()��()_c����
		P1 = gen_P();
		control_P = true;
		t = false;
		V_name = temp_identifier + "()";

		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, 0, false);
		
		petri.set_fun_P(P1, last_func);//()����ҲҪ��fun_P

		T = gen_T();
		control_T = false;
		s = temp_identifier + "()";
		petri.Add_Transition(T, control_T, s, current);


		V = "";
		sourceP = true;
		petri.Add_Arc(P1, T, V, sourceP);
		string P3;
		if (inside == "")
		{
			//������ƿ���
			P3 = gen_P();
			control_P = true;
			t = false;
			V_name = temp_identifier + "()_c";

			petri.Add_Place(P3, V_name, tag, control_P, t, n1, d, s, 0, false);
		}

		//���������ýṹ��������뱻���ú���begin��
		string call_fun_begin = find_P_name(petri, temp_identifier + " begin");
		petri.add_call_P(call_fun_begin, P1);
		if (inside == "")
			petri.add_call_P(call_fun_begin, P3);

		//while��佫()��()_c������ӽ�information����ѭ��
		//if (temp_expression->parent->type == ITERATION_STATEMENT && flag_condition == true)
		//{

		//	for (int i = 0; i < petri.p_num; i++)
		//	{
		//		if (petri.place[i].v_name == temp_expression->parent->place)
		//		{
		//			//petri.place[i].call_flag = 1;//1����while����������к�������
		//			petri.place[i].information.push_back(P1);
		//			if (inside == "")
		//				petri.place[i].information.push_back(P3);
		//			break;
		//		}
		//	}
		//}

		////��ʾ�����������
		if (flag_condition == false && temp_expression->child->type == ASSIGNMENT_EXPRESSION && temp_expression->child->child->type == CONDITIONAL_EXPRESSION && temp_statement->child->type != JUMP_STATEMENT)
		{
			noreturn = true;
			new_place = gen_P();
			control_P = true;
			t = false;
			V_name = p->place;
			int judge = find_P_exist(petri.place, V_name);
			if (judge != 0)
			{
				char temp[5];
				_itoa(judge, temp, 10);
				V_name = V_name + "#" + temp;
				p->place = V_name;
				temp_statement->place = V_name;
			}
			if (temp_statement->child->type == JUMP_STATEMENT && temp_statement->child->child->type == RETURN)
			{
				temp_statement->contain_call = V_name;
				temp_statement->contain_call_flag = true;
			}
			petri.Add_Place(new_place, V_name, tag, control_P, t, n1, d, s, 0, false);

			petri.set_fun_P(new_place, last_func);
			string T1 = gen_T();
			//vector<string> v;
			//v.push_back(T);//������ڿ�����Ϊ ()����
			if (inside == "")
			{
				vector<string> temp_v;
				temp_v.push_back(P1);
				temp_v.push_back(P3);
				petri.set_enter_P(new_place, temp_v);
			}
			else
			{
				vector<string> temp_v;
				temp_v.push_back(P1);
				petri.set_enter_P(new_place, temp_v);
			}
			control_T = false;
			petri.Add_Transition(T1, control_T, V_name, current);
			petri.Add_Arc(new_place, T1, "", true);
			//petri.clear_enter(new_place);
			vector<string> temp;
			temp.push_back(T1);
			petri.set_control_T(new_place, temp);
			petri.Add_Place_enter(new_place, T);//
			petri.Add_Place_exit(new_place, T1);
			//���ӹ�ϵ��
			//petri.Add_Arc(T1, P1, "relation", false);
			petri.Add_Arc(P1, T1, "relation", true);
			if (inside == "")
			{
				//petri.Add_Arc(T1, P3, "relation", false);
				petri.Add_Arc(P3, T1, "relation", true);
			}
		}
		else
		{
			if (temp_statement->child->type == JUMP_STATEMENT && temp_statement->child->child->type == RETURN)
			{
				temp_statement->contain_call = temp_expression->child->place;
				temp_statement->contain_call_flag = true;
			}
			string statement_P = find_P_name(petri, statement_vname);
			if (inside == "")
			{
				vector<string> temp_v;
				temp_v.push_back(P1);
				temp_v.push_back(P3);
				petri.set_enter_P(statement_P, temp_v);
				petri.clear_enter(statement_P);
				petri.Add_Place_enter(statement_P, T);
			}
			else
			{
				vector<string> temp_v;
				temp_v.push_back(P1);
				petri.set_enter_P(statement_P, temp_v);
				petri.clear_enter(statement_P);
				petri.Add_Place_enter(statement_P, T);
			}
			//���ӹ�ϵ��
			vector<string> control_T = petri.get_control_T(statement_P);
			for (unsigned int i = 0; i < control_T.size(); i++)
			{
				//petri.Add_Arc(control_T[i], P1, "relation", false);
				petri.Add_Arc(P1, control_T[i], "relation", true);
			}
			if (inside == "")
			{
				for (unsigned int i = 0; i < control_T.size(); i++)
				{
					//petri.Add_Arc(control_T[i], P3, "relation", false);
					petri.Add_Arc(P3, control_T[i], "relation", true);
				}
			}
			//temp_statement->contain_call = temp_expression->child->place;
		}


		//�������������õı������
		string temp_statement_P = find_P_name(petri, temp_statement->place);
		//petri.set_call_flag(temp_statement_P, 1);

		//���췵�ر�Ǩ
		string T2 = "";
		if (inside == "")
		{
			T2 = gen_T();
			control_T = false;
			s = temp_identifier + " end";
			petri.Add_Transition(T2, control_T, s, current);
			string temp_P = find_P_name(petri, s);

			V = "";
			sourceP = true;
			petri.Add_Arc(temp_P, T2, V, sourceP);
			petri.Add_Arc(P3, T2, V, sourceP);
			petri.set_arc_type(P3, T2, 4);
			petri.set_arc_type(temp_P, T2, 5);
		}

		string temp_s = temp_identifier + " begin";
		string P4 = find_P_name(petri, temp_s);
		V = "";
		sourceP = false;
		petri.Add_Arc(T, P4, V, sourceP);
		petri.set_arc_type(T, P4, 3);


		//�������ݴ����ݲ���������
		gtree *temp_tree = p->child->next->next;//ֵ������->child
		if (temp_tree->type == ARGUMENT_EXPRESSION_LIST)
		{
			vector<string> v;
			gtree *temp_assignment_expression = temp_tree;
			while (temp_assignment_expression->type != ASSIGNMENT_EXPRESSION)
				temp_assignment_expression = temp_assignment_expression->child;

			while (1)
			{
				string value = temp_assignment_expression->place;
				v.push_back(value);
				if (temp_assignment_expression->parent->next->next != NULL && temp_assignment_expression->parent->next->next->type == ASSIGNMENT_EXPRESSION)
					temp_assignment_expression = temp_assignment_expression->parent->next->next;
				else
					break;
			}
			int sum = 0;
			for (int i = 0; i < petri.p_num; i++)
			{
				string temp_s = petri.place[i].v_name;
				vector<string> v1;
				SplitString(temp_s, v1, "@");
				if (v1[0] == temp_identifier)
				{
					V = v[sum++];
					if (petri.place[i].ispoint)
					{
						string p_name = find_P_name_1(petri, V, petri.get_current_P_num(T));
						V = p_name;
					}
					
					sourceP = false;
					petri.Add_Arc(T, petri.place[i].name, V, sourceP);
					sourceP = true;
					petri.Add_Arc(petri.place[i].name, T, V, sourceP);
					petri.set_arc_type(T, petri.place[i].name, 3);
					petri.set_arc_type(petri.place[i].name, T, 2);
					//����
					bool global = false;
					if (petri.place[i].ispoint)
						global = create_connect(petri, T, v[sum - 1], petri.get_current_P_num(T));
					else
						global = create_connect(petri, T, V, petri.get_current_P_num(T));
					if (global == true)
						petri.set_global(P1);
				}
			}
		}

		///��ת���ش���

				//���ز���
		string temp_P = temp_identifier + " end";
		//last_call = find_T_name(petri, temp_P);

		if (inside == "")
		{
			string T1 = find_T_name(petri, temp_P);

			string  P2;
			//temp_place = temp_p->child->place;
			P2 = find_P_name(petri, temp_statement->place);
			V = "";//��ʱ����Ϊִ�л�
			sourceP = false;
			petri.Add_Arc(T1, P2, V, sourceP);
			petri.set_arc_type(T1, P2, 1);//�������÷��ػ�
			string_replace(temp_P, " end", " begin");

		}
		else
		{
			//����end����������control_T



			string P2 = find_P_name(petri, temp_statement->place);
			string P3 = find_P_name(petri, temp_P);
			vector<string> v1 = petri.get_control_T(P2);
			for (unsigned k = 0; k < v1.size(); k++)
			{
				V = "";//��ʱ����Ϊִ�л�
				sourceP = true;
				petri.Add_Arc(P3, v1[k], V, sourceP);
				petri.set_arc_type(P3, v1[k], 1);
			}


		}

	}
	else if (judge_return_statement(p))//
	{
		//string last_sentence;
		string last_func;
		gtree *find_func = p->child;

		while (find_func->type != FUNCTION_DEFINITION)
			find_func = find_func->parent;

		string identifier = find_func->place;
		last_func = find_P_name(petri, identifier + " begin");
		string expression;
		if (p->child->child->next->type == EXPRESSION)
			expression = p->child->child->next->place;
		else
			expression = "";

		T = find_T_name(petri, p->place);
		string last_func_end = find_P_name(petri, identifier + " end");
		petri.Add_Arc(T, last_func_end, "", false);

		if (expression != "")
		{
			string last_func_v= find_P_name(petri, identifier + "_v");
			petri.Add_Arc(T, last_func_v, expression, false);
			petri.Add_Arc(last_func_v, T, "write", true);//_v������ȫ��Ϊд��
			bool global = create_connect(petri, T, expression, petri.get_current_P_num(T));
			P1 = find_P_name(petri, p->place);
			if (global == true)
				petri.set_global(P1);
		}

	}
	else if (judge_label_statement(p))
	{
		gtree *child_statement = p->child->child;
		while (child_statement->type != STATEMENT)
			child_statement = child_statement->next;
		string child_P = find_P_name(petri, child_statement->place);
		vector<string> child_enter_P = petri.get_enter_P(child_P);

		string T1 = find_T_name(petri, p->place);

		//����label���ı�Ǩ��������enter_P
		for (unsigned int i = 0; i < child_enter_P.size(); i++)
			petri.Add_Arc(T1, child_enter_P[i], "", false);
		
		//����label���ĳ���
		vector<string> child_exit = petri.get_exit(child_P);
		string label_P = find_P_name(petri, p->place);
		petri.Add_Place_exit(label_P, child_exit);

		//�������ı���
		gtree *context = p;
		
		while (1)
		{
			if (judge_inside_compound_statement(context))
			{
				context = context->parent;
				while (context->type != COMPOUND_STATEMENT)
				{
					if (context->type == STATEMENT_LIST && context->next
						&& context->next->type == STATEMENT)
					{
						context->next->label_P = label_P;
					}
					context = context->parent;
				}
				context = context->parent;
			}
			else
				break;
		}
	}
	else if (p->type == COMPOUND_STATEMENT && p->parent->type == STATEMENT && p->parent->parent->type == STATEMENT_LIST)
	{
		p->parent->place = p->place;
		P1 = gen_P();
		control_P = true;
		t = false;
		V_name = p->place;
		tag = "";
		petri.Add_Place(P1, V_name, tag, control_P, t, n1, d, s, 0, false);

		int current;
		if (addition == 0)
			current = gen_P_num;
		else
			current = addition;
		string T1 = gen_T();
		control_T = false;
		s = V_name;
		petri.Add_Transition(T1, control_T, s, current);

		petri.Add_Arc(P1, T1, "", true);
		inside_block1(petri, p, T1);
		petri.Add_Place_enter(P1, T1);
		vector<string> v;
		v.push_back(T1);
		petri.set_control_T(P1, v);//���ÿ��Ʊ�Ǩ
		v = get_statement_exit(p->parent, petri);
		petri.Add_Place_exit(P1, v);
		vector<string> temp_v;
		temp_v.push_back(P1);
		petri.set_enter_P(P1, temp_v);
		
	}
	//else if (judge_break_statement(p))
	//{
	//	//�̳д�����
	//}
	//else if (judge_goto_statement(p))
	//{
	//	//�̳д�����
	//}

	//�����ǩ
	if (p->type == STATEMENT && p->label_P != "")
	{
		string this_P = find_P_name(petri, p->place);
		vector<string> enter_P = petri.get_enter_P(this_P);
		for (unsigned int i = 0; i < enter_P.size(); i++)
			petri.add_label_P(p->label_P, enter_P[i]);

	}

	ast_to_cpn(petri, p->next, addition);
}

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

void create_CPN(C_Petri &petri, gtree *tree)
{
	V_Table *table = new V_Table("global");
	v_tables.push_back(table);
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

void C_Petri::release()
{
	for (int i = 0; i < p_num; i++)
	{
		if (place[i].n_num > 0)
			delete[] place[i].num;
		if (place[i].n_decimal > 0)
			delete[] place[i].decimal;
	}
}