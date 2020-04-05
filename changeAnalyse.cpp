#include"cpn.h"
#include"Lexer.h"
#include"AST_compare.h"
#include<algorithm>

bool exist_arc(C_Petri &petri, string source, string target, string V);
bool exist_in(vector<string> v, string s)//�ҵ�����true
{
	for (unsigned int i = 0; i < v.size(); i++)
	{
		if (v[i] == s)
			return true;
	}
	return false;
}

void find_previous_P(C_Petri &petri, string T, vector<string> &v)//, vector<string> &v1)//��Ǩ��ǰ����������ִ�л������⴦��
{
	//vector<string> v, v1;
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].sourceP == true && petri.arc[i].target == T )//&& petri.arc[i].V != "executed" && petri.arc[i].V != "executed#")
		{
			/*if (exist_arc(petri, petri.arc[i].target, petri.arc[i].source, petri.arc[i].V))
				v1.push_back(petri.arc[i].source);
			else*/
			if (petri.arc[i].type == 2 || petri.arc[i].type == 4)
				continue;
				v.push_back(petri.arc[i].source);
		}
		/*else if (petri.arc[i].sourceP == true && petri.arc[i].target == T && (petri.arc[i].V == "executed" || petri.arc[i].V == "executed#"))
		{
			for (int j = 0; j < petri.p_num; j++)
			{
				if (petri.place[j].name == petri.arc[i].source)
					if (petri.place[j].v_name == "executedP")
					{
						v.push_back(petri.arc[i].source);
						break;
					}
			}
		}*/

	}
	return ;
}

void find_afterward_P(C_Petri &petri, string T,vector<string> &v)//,vector<string> &v1)
{
	//vector<string> v, v1;
	for (int i = 0; i < petri.arcnum; i++)
	{
		
		if (petri.arc[i].sourceP == false && petri.arc[i].source == T && (execute_flag == false || (petri.arc[i].V != "executed" && petri.arc[i].V != "executed#")))
		{
			/*if (exist_arc(petri, petri.arc[i].target, petri.arc[i].source, petri.arc[i].V))
				v1.push_back(petri.arc[i].target);
			else*/
			if (petri.arc[i].type == 1 || petri.arc[i].type == 2)
				continue;
				v.push_back(petri.arc[i].target);
		}
	}
	return ;
}

bool exist_arc(C_Petri &petri, string source, string target, string V)
{
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].source == source && petri.arc[i].target == target && petri.arc[i].V == V)
			return true;
	}
	return false;
}

vector<string> forward_exist_T(C_Petri &petri, vector<string> change_T, string place)
{
	vector<string> T;
	stack<string> _P;
	vector<string> P_done;
	_P.push(place);

	while (!_P.empty())
	{
		string temp_P = _P.top();
		_P.pop();
		for (int i = 0; i < petri.arcnum; i++)
		{
			
			if (petri.arc[i].V != "executed#" && petri.arc[i].type != 6 &&
				petri.arc[i].V != "#"&&petri.arc[i].sourceP == false && petri.arc[i].target == temp_P)
			{
				if (exist_in(change_T, petri.arc[i].source))
				{
					if (!exist_in(T, petri.arc[i].source))
						T.push_back(petri.arc[i].source);
					//					break;
				}
				else
				{
					for (int j = 0; j < petri.arcnum; j++)
					{
						if (petri.arc[j].sourceP == true && petri.arc[j].target == petri.arc[i].source)
						{
							for (int k = 0; k < petri.p_num; k++)
							{
								if (petri.place[k].controlP == true && petri.place[k].name == petri.arc[j].source)
								{
									if (!exist_in(P_done, petri.arc[j].source))
									{
										vector<string> enter_P = petri.get_enter_P(petri.arc[j].source);
										for (unsigned int l = 0; l < enter_P.size(); l++)
											_P.push(enter_P[l]);
										P_done.push_back(petri.arc[j].source);
									}
									break;
								}
								else if (petri.place[k].controlP == false && petri.place[k].name == petri.arc[j].source)
									break;
							}

						}
					}
				}
			}
		}
	}
	return T;
}

void slice_pre_process(C_Petri petri, vector<string> &change_places);

void back_forward_slicing(C_Petri &petri, vector<string> place, vector<string> &final_P, vector<string>&final_T, vector<Arc> &final_Arc)//�仯ǰ��petri,Ԥ�����Ŀ�������place
{
	vector<string> P1, P2, P_done, T1, T2;// , P_read;
	P1 = place;
	for (unsigned int i = 0; i < P1.size(); i++)
	{
		if (!exist_in(P_done, P1[i]))
		{
			//���ж��Ƿ�Ϊȫ�ֱ���
			bool global = petri.get_global(P1[i]);
			if (global == true)
			{
				vector<string> tmp_change;
				tmp_change.push_back(P1[i]);
				slice_pre_process(petri, tmp_change);
				for (unsigned int j = 1; j < tmp_change.size(); j++)
					if (!exist_in(P1, tmp_change[j]))
						P1.push_back(tmp_change[j]);
			}

			for (int j = 0; j < petri.arcnum; j++)
			{
				//������token���ı�ı�Ǩ
				/*if (exist_arc(petri, petri.arc[j].target, petri.arc[j].source, petri.arc[j].V))
					continue;*/
				
				if (petri.arc[j].source == P1[i])
				{
					if (petri.arc[j].type == 1 || petri.arc[j].type == 2 || petri.arc[j].type == 5)
						continue;
					if (!exist_in(T1, petri.arc[j].target))//����T1��
					{

						if (execute_flag == false ||( petri.arc[j].V != "executed"&&petri.arc[j].V != "executed#"))//����ִ�л�
						{
							
							/*for (int k = 0; k < petri.p_num; k++)
							{
								if (petri.place[k].name == P1[i])
								{*/

									//if (petri.place[k].controlP == true)
									//{
									T1.push_back(petri.arc[j].target);
									vector<string> v;// , v1;
									find_previous_P(petri, petri.arc[j].target, v);
									for (unsigned int q = 0; q < v.size(); q++)
									{
										if (!exist_in(P1, v[q]))
											P1.push_back(v[q]);
									}
									/*for (unsigned int q = 0; q < v1.size(); q++)
										P_read.push_back(v1[q]);*/
									v.clear();
									//v1.clear();
									find_afterward_P(petri, petri.arc[j].target, v);
									for (unsigned int q = 0; q < v.size(); q++)
									{
										if (!exist_in(P1, v[q]))
											P1.push_back(v[q]);
									}
									/*for (unsigned int q = 0; q < v1.size(); q++)
										P_read.push_back(v1[q]);*/

									//}
							/*		break;
								}

							}*/
						}
					}
				}
				else if (petri.arc[j].target == P1[i])
				{
					if (petri.arc[j].type == 2 || petri.arc[j].type == 3)
						continue;
					if (!exist_in(T1, petri.arc[j].source))//����T1��
					{
						if (execute_flag == false ||( petri.arc[j].V != "executed"&&petri.arc[j].V != "executed#"))//����ִ�л�
						{
							//������token���ı�Ŀ���
							/*for (int k = 0; k < petri.p_num; k++)
							{
								if (petri.place[k].name == P1[i])
								{*/

									//if (petri.place[k].controlP == true)
									//{
									T1.push_back(petri.arc[j].source);
									vector<string> v, v1;
									find_previous_P(petri, petri.arc[j].source, v);
									for (unsigned int q = 0; q < v.size(); q++)
									{
										if (!exist_in(P1, v[q]))
											P1.push_back(v[q]);
									}
									/*for (unsigned int q = 0; q < v1.size(); q++)
										P_read.push_back(v1[q]);*/
									v.clear();
									v1.clear();
									find_afterward_P(petri, petri.arc[j].source, v);
									for (unsigned int q = 0; q < v.size(); q++)
									{
										if (!exist_in(P1, v[q]))
											P1.push_back(v[q]);
									}
									/*for (unsigned int q = 0; q < v1.size(); q++)
										P_read.push_back(v1[q]);*/
									//}
							/*		break;
								}

							}*/
						}
					}
				}
			}
			
			P_done.push_back(P1[i]);
		}

	}


	//P1.insert(P1.end(), P_read.begin(), P_read.end());//����P_read

	//sort(P1.begin(), P1.end());
	//P1.erase(unique(P1.begin(), P1.end()), P1.end());

	sort(T1.begin(), T1.end());
	T1.erase(unique(T1.begin(), T1.end()), T1.end());



	vector<Arc> arc;
	for (int i = 0; i < petri.arcnum; i++)
	{
		if (petri.arc[i].sourceP == true)
		{
			if (exist_in(P1, petri.arc[i].source) && exist_in(T1, petri.arc[i].target))
			{
				arc.push_back(petri.arc[i]);
				//				break;
			}
		}
		else
		{
			if (exist_in(P1, petri.arc[i].target) && exist_in(T1, petri.arc[i].source))
			{
				arc.push_back(petri.arc[i]);
				//				break;
			}
		}
	}
	final_P = P1;
	final_T = T1;
	final_Arc = arc;
}

void post_process(C_Petri &petri, vector<string> change_P, vector<string> change_T, vector<Arc> &change_Arc)
{
	string main_begin = find_P_name(petri, "main begin");
	for (unsigned i = 0; i < change_P.size(); i++)
	{
		bool flag = false;
		if (change_P[i] != main_begin)
		{
			for (unsigned j = 0; j < change_Arc.size(); j++)
			{
				if (change_Arc[j].sourceP == false && change_Arc[j].target == change_P[i] && change_Arc[j].type != 6)
				{
					flag = true;
					break;
				}
			}
			if (flag == false)
			{
				vector<string> temp_v = forward_exist_T(petri, change_T, change_P[i]);
				bool flag1 = false;
				for (unsigned int j = 0; j < temp_v.size(); j++)
				{
					Arc arc(temp_v[j], change_P[i], "executed", false);

					//��ֹ�ظ����
					for (int k = int(change_Arc.size() - 1); k >= 0; k--)
						if (change_Arc[k].source == arc.source && change_Arc[k].target == arc.target)
						{
							flag1 = true;
							break;
						}
					if (flag1 == false)
					{
						
						change_Arc.push_back(arc);
					}
				}
			}
		}
	}
}

void initial_changeAnalyse_cpn(C_Petri &petri1, C_Petri &petri, vector<string> change_P, vector<string> change_T, vector<Arc> &change_Arc)
{
	cout << endl;
	cout << "��ؿ�����" << endl;

	petri1.p_num = change_P.size();
	petri1.t_num = change_T.size();
	petri1.arcnum = change_Arc.size();

	for (unsigned int i = 0; i < change_P.size(); i++)
	{
		cout << change_P[i] << "    ";
		for (int j = 0; j < petri.p_num; j++)
		{
			if (petri.place[j].name == change_P[i])
				petri1.place.push_back(petri.place[j]);
		}
	}

	cout << endl;
	cout << "��ر�Ǩ��" << endl;
	for (unsigned int i = 0; i < change_T.size(); i++)
	{
		cout << change_T[i] << "    ";
		for (int j = 0; j < petri.t_num; j++)
		{
			if (petri.transition[j].name == change_T[i])
				petri1.transition.push_back(petri.transition[j]);
		}
	}
	cout << endl;
	cout << "��ػ���" << endl;
	for (unsigned int i = 0; i < change_Arc.size(); i++)
	{
		cout << change_Arc[i].source << "------->" << change_Arc[i].target << endl;
		petri1.arc.push_back(change_Arc[i]);
	}

	for (int i = 0; i < petri1.p_num; i++)
	{
		for (int j = 0; j < petri1.p_num - i - 1; j++)
		{
			if (petri1.place[j].id_num > petri1.place[j + 1].id_num)
			{
				Place tmp(petri1.place[j]);
				//tmp = petri.place[j];
				petri1.place[j] = petri1.place[j + 1];
				petri1.place[j + 1] = tmp;
			}
		}
	}
}

void sort_change(vector<string> &change_P, vector<string> &change_T)
{
	for (unsigned int i = 0; i < change_P.size(); i++)
	{
		for (unsigned int j = 0; j < change_P.size() - 1 - i; j++)
		{
			int num1 = atoi(change_P[j].substr(1).c_str());
			int num2 = atoi(change_P[j + 1].substr(1).c_str());
			if (num1 > num2)
			{
				string temp = change_P[j];
				change_P[j] = change_P[j + 1];
				change_P[j + 1] = temp;
			}
		}
	}

	for (unsigned int i = 0; i < change_T.size(); i++)
	{
		for (unsigned int j = 0; j < change_T.size() - 1 - i; j++)
		{
			int num1 = atoi(change_T[j].substr(1).c_str());
			int num2 = atoi(change_T[j + 1].substr(1).c_str());
			if (num1 > num2)
			{
				string temp = change_T[j];
				change_T[j] = change_T[j + 1];
				change_T[j + 1] = temp;
			}
		}
	}
}

string find_relateP(C_Petri petri, string transition)
{
	for (int i = 0; i < petri.arcnum; i++)
	{

		if (petri.arc[i].target == transition)
		{
			string V = petri.arc[i].V;
			if (V != "executed"&&V != "#"&&V != "executedP")
				return petri.arc[i].source;
		}
	}
	cout << "can't find relateP!" << endl;
	exit(1);
	return "";
}

void slice_pre_process(C_Petri petri, vector<string> &change_places)
{
	string fun_P;
	vector<string> call_P;
	vector<string> done;
	for (unsigned int i = 0; i < change_places.size(); i++)
	{
		fun_P = petri.get_fun_P(change_places[i]);
		if (fun_P == "")
			continue;
		if (exist_in(done, fun_P))
			continue;
		else
			done.push_back(fun_P);
		call_P = petri.get_call_P(fun_P);
		if (call_P.size() == 0)
			continue;
		else
			change_places.insert(change_places.end(), call_P.begin(), call_P.end());
	}
}

//���룺petri���Լ���ʼ�仯����
//���أ������仯Ӱ��������petri��
C_Petri changeAnalyse(C_Petri &petri, vector<string> change_places)
{
	vector<string> change_P, change_T, formula_P, formula_T, others;
	vector<Arc> change_Arc;

	//��ȡ��ʽ�еĿ�����Ǩ��������Ǩת��Ϊ����
	/*getPTinformula("formulaC.txt", formula_P, formula_T, others, true);
	getPTinformula("formulaF.txt", formula_P, formula_T, others, true);
	for (unsigned int i = 0; i < formula_T.size(); i++)
		formula_P.push_back(find_relateP(petri, formula_T[i]));
	change_places.insert(change_places.end(), formula_P.begin(), formula_P.end());

	sort(change_places.begin(), change_places.end());
	change_places.erase(unique(change_places.begin(), change_places.end()), change_places.end());*/

	//change_T.insert(change_T.end(), formula_T.begin(), formula_T.end());

	slice_pre_process(petri, change_places);
	back_forward_slicing(petri, change_places, change_P, change_T, change_Arc);

	post_process(petri, change_P, change_T, change_Arc);
	//sort_change(change_P, change_T);
	C_Petri petri1;
	initial_changeAnalyse_cpn(petri1, petri, change_P, change_T, change_Arc);
	return petri1;
}