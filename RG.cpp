#include"RG.h"
#include<fstream>

bool array_compare(int *a1, int *a2, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (a1[i] != a2[i])
			return false;
	}
	return true;
}

bool array_compare(double *a1, double *a2, int n)
{
	for (int i = 0; i < n; i++)
	{
		if (a1[i] != a2[i])
			return false;
	}
	return true;
}

void array_copy(int *a1, int *a2, int n)//a1数组内容拷贝到a2中
{
	for (int i = 0; i < n; i++)
		a2[i] = a1[i];
}

void array_copy(double *a1, double *a2, int n)//a1数组内容拷贝到a2中
{
	for (int i = 0; i < n; i++)
		a2[i] = a1[i];
}

RG::RG(C_Petri petri1)
{
	petri.p_num = petri1.p_num;
	petri.t_num = petri1.t_num;
	petri.arcnum = petri1.arcnum;
	for (int i = 0; i < petri1.p_num; i++)
	{
		petri.place.push_back(petri1.place[i]);
	}
	for (int i = 0; i < petri1.t_num; i++)
	{
		petri.transition.push_back(petri1.transition[i]);
	}
	for (int i = 0; i < petri1.arcnum; i++)
	{
		petri.arc.push_back(petri1.arc[i]);
	}

	create_node(petri, petri1.enable_T());
}

void RG::init_RG(C_Petri petri1)
{
	petri.p_num = petri1.p_num;
	petri.t_num = petri1.t_num;
	petri.arcnum = petri1.arcnum;
	for (int i = 0; i < petri1.p_num; i++)
	{
		petri.place.push_back(petri1.place[i]);
	}
	for (int i = 0; i < petri1.t_num; i++)
	{
		petri.transition.push_back(petri1.transition[i]);
	}
	for (int i = 0; i < petri1.arcnum; i++)
	{
		petri.arc.push_back(petri1.arc[i]);
	}
	petri.rebuildcp();
	create_node(petri, petri1.enable_T());
}

bool M::operator==(const M m1)
{
	if (control_P == m1.control_P&&token_num == m1.token_num&&color == m1.color&&s == m1.s&&n_n == m1.n_n&&n_dec == m1.n_dec)
	{
		if (array_compare(n, m1.n, n_n) && array_compare(dec, m1.dec, n_dec))
			return true;
		return false;
	}
	else
		return false;
}

bool equal_M(vector<M> m1, vector<M> m2)//要求m1.m2的size一样
{
	for (unsigned int i = 0; i < m1.size(); i++)
	{
		if (m1[i] == m2[i])
		{
		}
		else
			return false;
	}
	return true;
}

void load(vector<M> m, C_Petri &petri)
{
	for (int i = 0; i < petri.p_num; i++)
	{
		petri.place[i].controlP = m[i].control_P;
		petri.place[i].token_num = m[i].token_num;
		petri.place[i].colorset_tag = m[i].color;
		//petri.place[i].num = m[i].n;
		//petri.place[i].decimal = m[i].dec;
		int n_n = m[i].n_n;
		int n_dec = m[i].n_dec;
		petri.place[i].n_num = n_n;
		petri.place[i].n_decimal = n_dec;
		petri.place[i].num = new int[n_n];
		petri.place[i].decimal = new double[n_dec];
		array_copy(m[i].n, petri.place[i].num, n_n);
		array_copy(m[i].dec, petri.place[i].decimal, n_dec);
		petri.place[i].str = m[i].s;
	}

}

void occur_T(C_Petri &petri, string T)
{

	for (int i = 0; i < petri.t_num; i++)
	{
		if (petri.transition[i].name == T)
		{
			//producer
			for (unsigned int j = 0; j < petri.transition[i].producer.size(); j++)
			{
				if (petri.transition[i].producer[j]->controlP == true)
				{
					int token_num = petri.transition[i].producer[j]->token_num--;
					if (token_num == 0)
					{
						cout << "token_num < 0 !" << endl;
						exit(-1);
					}

				}
			}

			//consumer
			for (unsigned int j = 0; j < petri.transition[i].consumer.size(); j++)
			{

				if (petri.transition[i].consumer[j]->controlP == true)
					petri.transition[i].consumer[j]->token_num++;
				else
				{
					if (petri.transition[i].consumer_V[j] == "")
						continue;
					if (petri.transition[i].consumer[j]->ispoint == true)
					{
						petri.transition[i].consumer[j]->str = petri.transition[i].consumer_V[j];
					}
					else
					{
						string temp_s = petri.transition[i].consumer_V[j];
						int position = 0;
						for (unsigned int i = 0; i < temp_s.length(); i++)
						{
							if (temp_s[i] == '$')
							{
								position = i;
								break;
							}
						}

						int array_num = 0;
						if (position != 0)
						{
							int current = petri.get_current(T);
							array_num = int(change_and_cal(temp_s.substr(position + 1), petri.place, current));
							temp_s = temp_s.substr(0, position);
						}

						double value = change_and_cal(temp_s, petri.place, petri.get_current(T));
						string temp_tag = petri.transition[i].consumer[j]->colorset_tag;
						if (temp_tag == "")
							temp_tag = "0";
						if (temp_tag[temp_tag.size() - 1] == '1' || temp_tag[temp_tag.size() - 1] == '5' || temp_tag[temp_tag.size() - 1] == '6')
							petri.transition[i].consumer[j]->num[array_num] = int(value);
						else if (temp_tag[temp_tag.size() - 1] == '3' || temp_tag[temp_tag.size() - 1] == '4')
							petri.transition[i].consumer[j]->decimal[array_num] = value;
					}
				}
			}

			break;
		}
	}
}

int RG::create_node(C_Petri petri, vector<string> T)
{
	vector<M> temp_M;
	M temp_m;
	for (int i = 0; i < petri.p_num; i++)
	{
		temp_m.control_P = petri.place[i].controlP;
		temp_m.token_num = petri.place[i].token_num;
		temp_m.color = petri.place[i].colorset_tag;
		//temp_m.n = petri.place[i].num;
		//temp_m.dec = petri.place[i].decimal;
		int n_n = petri.place[i].n_num;
		int n_dec = petri.place[i].n_decimal;
		temp_m.n_n = n_n;
		temp_m.n_dec = n_dec;
		temp_m.n = new int[n_n];
		temp_m.dec = new double[n_dec];
		temp_m.s = petri.place[i].str;
		array_copy(petri.place[i].num, temp_m.n, n_n);
		array_copy(petri.place[i].decimal, temp_m.dec, n_dec);
		temp_M.push_back(temp_m);
	}

	RGNode node;
	node.num = node_num++;
	node.m = temp_M;

	rgnode.push_back(node);
	return node.num;


}

void RG::add_next(int node_id, stack<int> &newnode)
{
	RGNode rgnode1 = rgnode[node_id];
	load(rgnode1.m, petri);
	vector<string> T = petri.enable_T();
	for (unsigned int i = 0; i < T.size(); i++)
	{
		//		for (int j = 0; j < petri.t_num; j++)
		//		{
		//			if (T[i] == petri.transition[j].name)
		//			{
		//				rgnode[node_id].isfirable.push_back(j);
		//				break;
		//			}
		//		}
		string temp_s = T[i].substr(1);
		int temp_i = atoi(temp_s.c_str());
		rgnode[node_id].isfirable.push_back(temp_i);

	}
	rgnode[node_id].enableNum = T.size();
	if (T.size() == 0)
	{
		rgnode[node_id].end = true;
		return;
	}
	for (unsigned int i = 0; i < T.size(); i++)
	{
		int old_num = node_num;
		nextNode next;
		next.T = T[i];
		load(rgnode1.m, petri);
		occur_T(petri, T[i]);
		int num = create_node(petri, T);
		next.num = num;
		rgnode[node_id].next.push_back(next);
		//if (node_num > old_num)
			newnode.push(next.num);
	}
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

void RG::release()
{
	for (unsigned int i = 0; i < rgnode.size(); i++)
	{
		for (unsigned int j = 0; j < rgnode[i].m.size(); j++)
		{
			if (rgnode[i].m[j].n_n > 0)
				delete[] rgnode[i].m[j].n;

			if (rgnode[i].m[j].n_dec > 0)
				delete[] rgnode[i].m[j].dec;
		}
	}
	rgnode.clear();
	petri.release();
}