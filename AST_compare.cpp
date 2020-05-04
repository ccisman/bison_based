

#include"tree.h"
//#include"cpn.h"
#include"AST_compare.h"
#include"y.tab.h"

int cal_height(gtree *t)
{
	if (t == NULL)
		return 0;

	int child = cal_height(t->child);
	int next = cal_height(t->next);

	int height = child + 1 > next ? child + 1 : next;
	return height;
}

void HighIndexList::order()
{
	if (list.size() == 0)
		return;
	for (unsigned int i = 0; i < list.size() - 1; i++)
		for (unsigned int j = 0; j < list.size() - i - 1; j++)
		{
			if (list[j].height < list[j + 1].height)
			{
				gtree *temp_point;
				int temp_height;
				temp_point = list[j].point;
				temp_height = list[j].height;
				list[j].point = list[j + 1].point;
				list[j].height = list[j + 1].height;
				list[j + 1].point = temp_point;
				list[j + 1].height = temp_height;
			}
		}
}

void HighIndexList::push(gtree *t)
{
	tree_height t_h;
	t_h.point = t;
	t_h.height = cal_height(t->child);
	list.push_back(t_h);
	order();
}

int HighIndexList::peekMax()
{
	if (list.size() == 0)
	{
		cout << "当前list为空!" << endl;
		exit(1);
	}
	else
		return list[0].height;
}

vector<gtree *> HighIndexList::pop()
{
	int height = peekMax();
	vector<gtree *> return_list;
	for (vector<tree_height>::iterator it = list.begin(); it != list.end(); )
	{
		if (it->height == height)
		{
			return_list.push_back(it->point);
			it = list.erase(it);
		}
		else
			break;

	}
	return return_list;
}

void HighIndexList::open(gtree *t)
{
	gtree *p = t->child;
	while (p != NULL)
	{
		push(p);
		p = p->next;
	}
}

bool theSame(gtree *t1, gtree *t2);

bool Equal_Node(gtree *t1, gtree *t2)
{
	/*if (t1->place == "3" || t1->place == "2")
	{
		int a;
		a = 1;
	}*/
	if (t1->type != t2->type)
		return false;
	string p1, p2;
	for (unsigned int i = 0; i < t1->place.size(); i++)
	{
		if (t1->place[i] == '#')
			break;
		else
			p1.append(1, t1->place[i]);
	}
	for (unsigned int i = 0; i < t2->place.size(); i++)
	{
		if (t2->place[i] == '#')
			break;
		else
			p2.append(1, t2->place[i]);
	}
	if (p1 != p2)
		return false;
	return true;
}

bool Equal_Tree(gtree *t1, gtree *t2)
{
	if (!Equal_Node(t1, t2))
		return false;

	return theSame(t1->child, t2->child);
}

bool theSame(gtree *t1, gtree *t2)
{
	if (t1 == NULL && t2 != NULL)
		return false;
	else if (t1 != NULL && t2 == NULL)
		return false;
	else if (t1 == NULL && t2 == NULL)
		return true;
	else if (t1 != NULL && t2 != NULL)
	{
		if (!Equal_Node(t1,t2))
			return false;
		else
			return theSame(t1->child, t2->child) && theSame(t1->next, t2->next);
	}
}


/*
top-down算法
输入：两棵语法树
输出：两棵语法树的Map集
*/

void top_down(gtree *t1, gtree *t2, vector<Mapping> &M)
{
	HighIndexList L1, L2;
	L1.push(t1);
	L2.push(t2);
	
	while (L1.peekMax() > minHeight&&L2.peekMax() > minHeight)
	{

		while (L1.peekMax() != L2.peekMax())
		{
			if (L1.peekMax() > L2.peekMax())
			{
				vector<gtree *> child_list = L1.pop();
				for (unsigned int i = 0; i < child_list.size(); i++)
					L1.open(child_list[i]);
			}
			else
			{
				vector<gtree *> child_list = L2.pop();
				for (unsigned int i = 0; i < child_list.size(); i++)
					L2.open(child_list[i]);
			}
		}
		vector<gtree*> H1 = L1.pop();
		vector<gtree*> H2 = L2.pop();
		for (vector<gtree*>::iterator it1 = H1.begin(); it1 != H1.end(); )
		{
			bool flag = false;
			bool break_flag = false;
			for (vector<gtree*>::iterator it2 = H2.begin(); it2 != H2.end(); )
			{
				if (Equal_Tree(*it1, *it2))
				{
					Mapping m;
					m.map1 = *it1;
					m.map2 = *it2;
					m.num = m.map1->num;
					M.push_back(m);
					it1 = H1.erase(it1);

					it2 = H2.erase(it2);
					if (H1.size() == 0 || it1 == H1.end())
					{
						break_flag = true;
						break;
					}
					if (H2.size() == 0)
					{
						break_flag = true;
						break;
					}
					flag = true;
				}
				else
					it2++;
			}
			if (break_flag == true)
				break;
			if (flag == false)
				it1++;
		}

		for (unsigned int i = 0; i < H1.size(); i++)
			L1.open(H1[i]);
		for (unsigned int i = 0; i < H2.size(); i++)
			L2.open(H2[i]);
	}

}

bool exist_in_M(gtree *t, vector<Mapping> M)
{
	for (unsigned int i = 0; i < M.size(); i++)
	{
		if (M[i].map1 == t || M[i].map2 == t)
			return true;
	}
	return false;
}

bool exist_in_M(Mapping m, vector<Mapping> M)
{
	for (unsigned int i = 0; i < M.size(); i++)
	{
		if ((M[i].map1 == m.map1 && M[i].map2 == m.map2) || (M[i].map1 == m.map2 && M[i].map2 == m.map1))
			return true;
	}
	return false;
}

gtree *extract_theOther_in_M(gtree *t, vector<Mapping> M)
{
	for (unsigned int i = 0; i < M.size(); i++)
	{
		if (M[i].map1 == t)
			return M[i].map2;
		else if (M[i].map2 == t)
			return M[i].map1;
	}
	cout << "M中没有当前指针，请检查是否exist_in_M！" << endl;
	exit(1);
}

/*void find_Nomatch_Node(gtree *t, vector<Mapping> &M, int type, vector<gtree *> &find_result)
{
	if (t == NULL)
		return;
	bool flag = exist_in_M(t, M);
	if (flag == true)
	{
		find_Nomatch_Node(t->next, M, type, find_result);
	}
	else
	{
		if (t->type == type)
			find_result.push_back(t);
		find_Nomatch_Node(t->child, M, type, find_result);
		find_Nomatch_Node(t->next, M, type, find_result);
	}

}*/

vector<Mapping> find_container_mapping(vector<Mapping> &M)//找到所有包含相同项的非match语句列节点，即container mapping集
{
	vector<Mapping> result_list;
	for (unsigned int i = 0; i < M.size(); i++)
	{
		if (M[i].map1->type == STATEMENT || M[i].map1->type == STATEMENT_LIST)
		{
			//if (M[i].map1->parent->type == 语句列 && !exist_in_M(M[i].map1->parent, M))
			if (M[i].map1->parent->type != STATEMENT_LIST && M[i].map2->parent->type != STATEMENT_LIST)//
			{
				if (M[i].map1->parent->type == ITERATION_STATEMENT || M[i].map1->parent->type == SELECTION_STATEMENT)
					continue;
				//cout << "语句完全匹配，声明不匹配!等待您的处理!" << endl;
				//exit(1);
			}
			else
			{
				gtree *statement_list1 = M[i].map1;
				gtree *statement_list2 = M[i].map2;
				while (statement_list1->parent->type == STATEMENT_LIST)
					statement_list1 = statement_list1->parent;
				while (statement_list2->parent->type == STATEMENT_LIST)
					statement_list2 = statement_list2->parent;

				Mapping m;
				m.map1 = statement_list1;
				m.map2 = statement_list2;
				m.num = statement_list1->num;
				if (!exist_in_M(m, result_list))
					result_list.push_back(m);

				if (M[i].map1->type == STATEMENT_LIST)
				{
					gtree *temp_p1 = M[i].map1->child, *temp_p2 = M[i].map2->child;
					/*while (temp_p1->type != STATEMENT)
						temp_p1 = temp_p1->child;
					while (temp_p2->type == STATEMENT)
						temp_p2 = temp_p2->child;*/
					Mapping temp_m;
					while (temp_p1->type == STATEMENT_LIST)
					{
						
						temp_m.map1 = temp_p1->next;
						temp_m.map2 = temp_p2->next;
						if (!exist_in_M(temp_m, M))
							M.push_back(temp_m);
						temp_p1 = temp_p1->child;
						temp_p2 = temp_p2->child;
					}
					temp_m.map1 = temp_p1;
					temp_m.map2 = temp_p2;
					if (!exist_in_M(temp_m, M))
						M.push_back(temp_m);
				}

			}
		}
		else if (M[i].map1->type == DECLARATION || M[i].map1->type == DECLARATION_LIST)
		{
			if (M[i].map1->parent->type != DECLARATION_LIST && M[i].map2->parent->type != DECLARATION_LIST)//
				continue;
			//{
			//	if (M[i].map1->parent->type == ITERATION_STATEMENT || M[i].map1->parent->type == SELECTION_STATEMENT)
			//		continue;
			//	cout << "语句完全匹配，声明不匹配!等待您的处理!" << endl;
			//	exit(1);
			//}
			//else
			//{
			gtree *statement_list1 = M[i].map1;
			gtree *statement_list2 = M[i].map2;
			while (statement_list1->parent->type == DECLARATION_LIST)
				statement_list1 = statement_list1->parent;
			while (statement_list2->parent->type == DECLARATION_LIST)
				statement_list2 = statement_list2->parent;

			Mapping m;
			m.map1 = statement_list1;
			m.map2 = statement_list2;
			m.num = statement_list1->num;
			if (!exist_in_M(m, result_list))
				result_list.push_back(m);

			if (M[i].map1->type == DECLARATION_LIST)
			{
				gtree *temp_p1 = M[i].map1->child, *temp_p2 = M[i].map2->child;
				/*while (temp_p1->type != STATEMENT)
					temp_p1 = temp_p1->child;
				while (temp_p2->type == STATEMENT)
					temp_p2 = temp_p2->child;*/
				Mapping temp_m;
				while (temp_p1->type == DECLARATION_LIST)
				{

					temp_m.map1 = temp_p1->next;
					temp_m.map2 = temp_p2->next;
					if (!exist_in_M(temp_m, M))
						M.push_back(temp_m);
					temp_p1 = temp_p1->child;
					temp_p2 = temp_p2->child;
				}
				temp_m.map1 = temp_p1;
				temp_m.map2 = temp_p2;
				if (!exist_in_M(temp_m, M))
					M.push_back(temp_m);
			}
			//}
		}

	}
	return result_list;
}

bool total_match(gtree *t1, gtree *t2, vector<Mapping> &M)
{
	
	gtree *p1 = t1, *p2 = t2;
	/*if (t1->child->type == STATEMENT)
	{
		if (t1->child->child->type == SELECTION_STATEMENT || t1->child->child->type == ITERATION_STATEMENT)
		{
			gtree *statement1 = t1->child->child->child->next->next->next->next;
			gtree *statement2 = t2->child->child->child->next->next->next->next;
			bool f1, f2;
			if (statement1->child->type == COMPOUND_STATEMENT)
			{
				gtree *statement_list1 = statement1->child->child;
				while (statement_list1->type != STATEMENT_LIST)
					statement_list1 = statement_list1->next;
				gtree *statement_list2 = statement2->child->child;
				while (statement_list2->type != STATEMENT_LIST)
					statement_list2 = statement_list2->next;
				f1 = total_match(statement_list1, statement_list2, M);
				return f1;
			}
		}
	}*/
	
	bool total_match = true;
	while (p1->type != STATEMENT)
		p1 = p1->child;
	while (p2->type != STATEMENT)
		p2 = p2->child;
	while (1)
	{
		if (p1->type != STATEMENT && p2->type != STATEMENT)
			return true;
		else if (p1->type != STATEMENT && p2->type == STATEMENT)
			return false;
		else if (p1->type == STATEMENT && p2->type != STATEMENT)
			return false;
		else
		{
			Mapping m;
			m.map1 = p1;
			m.map2 = p2;
			if (!exist_in_M(m, M))
				return false;
		}
		p1 = p1->parent->next;
		p2 = p2->parent->next;
	}
}

int sum_intersect(int target_num, vector<pair<int, int>> list)//计算target_num位置位置对与其他位置对相交的数量
{
	pair<int, int> target = list[target_num];
	int sum_neg = 0;
	for (unsigned int i = 1; i < list.size(); i++)//这里从1开始考虑到(0,0)已经存在
	{
		if (i != target_num)
		{
			int first = list[i].first - target.first;
			int second = list[i].second - target.second;
			if (first*second < 0)
				sum_neg++;

		}
	}
	return sum_neg;
}

int find_first_intersect(int position, vector<pair<int, int>> list)
{
	pair<int, int> target = list[position];
	for (unsigned int i = 1; i < list.size(); i++)
	{
		int first = list[i].first - target.first;
		int second = list[i].second - target.second;
		if (first*second < 0)
			return i;
	}
}

vector<pair<int, int>> process_move(vector<pair<int, int>> &list)//对所有移动的节点进行处理，返回所有移动节点的position_pair
{
	vector<pair<int, int>> result;
	for (unsigned int i = 1; i < list.size(); )
	{
		int sum1 = sum_intersect(i, list);
		int sum2;
		if (sum1 != 0)
		{
			int next_num = find_first_intersect(i, list);
			sum2 = sum_intersect(next_num, list);
			if (sum1 >= sum2)
			{
				result.push_back(list[i]);
				list.erase(list.begin() + i);
			}
			else
			{
				result.push_back(list[next_num]);
				list.erase(list.begin() + next_num);
			}
		}
		else
			i++;

	}
	return result;
}

int find_last_inmoves(vector<pair<int, int>> moves, int value, bool isfirst)
{
	int max = 0;
	for (unsigned int i = 0; i < moves.size(); i++)
	{
		if (isfirst)
		{
			if (moves[i].first < value)
				max = max > moves[i].first ? max : moves[i].first;
		}
		else
		{
			if (moves[i].second < value)
				max = max > moves[i].second ? max : moves[i].second;
		}
	}
	return max;
}

bool exist_inmoves(vector<pair<int, int>> moves, int value, bool isfirst)
{
	int max = 0;
	for (unsigned int i = 0; i < moves.size(); i++)
	{
		if (isfirst)
		{
			if (moves[i].first == value)
				return true;
		}
		else
		{
			if (moves[i].second == value)
				return true;
		}
	}
	return false;
}

void add_change(vector<pair<int, int>> moves, int last_first, int last_second, int now_first, int now_second, vector<pair<int, int>> &modify, vector<pair<int, int>> &add, vector<pair<int, int>> &del)
{
	last_first++;
	last_second++;
	while (last_first != now_first && last_second != now_second)
	{
		if (exist_inmoves(moves, last_first, true))
		{
			last_first++;
		}
		else if (exist_inmoves(moves, last_second, false))
		{
			last_second++;
		}
		else
		{
			modify.push_back(make_pair(last_first, last_second));
			last_first++;
			last_second++;
		}
	}

	if (last_first == now_first)
	{
		while (last_second != now_second)
		{
			if (!exist_inmoves(moves, now_second - 1, false))
				add.push_back(make_pair(last_first - 1, last_second));
			last_second++;
		}
	}
	else
	{
		while (last_first != now_first)
		{
			if (!exist_inmoves(moves, last_first, true))
				del.push_back(make_pair(last_first, last_second - 1));
			last_first++;
		}
	}

}

void process_other(vector<pair<int, int>> moves, vector<pair<int, int>> list, vector<pair<int, int>> &modify, vector<pair<int, int>> &add, vector<pair<int, int>> &del, int count1, int count2)
{
	int last_first, last_second, now_first, now_second;

	for (unsigned int i = 1; i < list.size(); i++)
	{
		last_first = list[i - 1].first;
		last_second = list[i - 1].second;
		//int temp_first = find_last_inmoves(moves, list[i].first, true);
		//last_first = last_first > temp_first ? last_first : temp_first;
		//int temp_second = find_last_inmoves(moves, list[i].second, false);
		//last_second = last_second > temp_second ? last_second : temp_second;

		int del_first = list[i].first - last_first;
		int del_second = list[i].second - last_second;
		if (del_first == 1 && del_second == 1)
			;
		else
			add_change(moves, last_first, last_second, list[i].first, list[i].second, modify, add, del);
		if (i == list.size() - 1)
		{
			last_first = find_last_inmoves(moves, count1, true);
			last_second = find_last_inmoves(moves, count2, false);
			last_first = last_first > list[i].first ? last_first : list[i].first;
			last_second = last_second > list[i].second ? last_second : list[i].second;
			if (count1 - last_first == 1 && count2 - last_second == 1)
				;
			else
				add_change(moves, last_first, last_second, count1, count2, modify, add, del);
		}
	}
}

AST_change block_handle(gtree *t1, gtree *t2, vector<Mapping> &M)//t1,t2都是语句列指针
{
	AST_change changes;
	changes.m.map1 = t1;
	changes.m.map2 = t2;
	
	int state_or_declare;
	bool is_state;
	if (t1->type == STATEMENT_LIST)
	{
		state_or_declare = STATEMENT;
		is_state = true;
	}
	else
	{
		state_or_declare = DECLARATION;
		is_state = false;
	}
	

	//先判断是否所有子句都已match（若都match则已经不需要处理该语句块）
	if (is_state)
	{
		bool match = total_match(t1, t2, M);
		if (match == true)
		{
			if (t1->parent->parent->parent != NULL)
			{
				Mapping m;
				m.map1 = t1->parent->parent->parent->parent;
				m.map2 = t2->parent->parent->parent->parent;
				M.push_back(m);
			}
			changes.m.map1 = NULL;
			changes.m.map2 = NULL;
			return changes;
		}
	}
	//找到语句块的position_pair数列
	vector<pair<int, int>> position_pair_list;
	position_pair_list.push_back(make_pair(0, 0));//(0,0)作为阶梯起点

	gtree *p1 = t1, *p2 = t2;
	while (p1->type != state_or_declare)
		p1 = p1->child;
	while (p2->type != state_or_declare)
		p2 = p2->child;
	gtree *p2_start = p2;
	int position1 = 1, position2 = 1;
	while (p1->type == state_or_declare)
	{
		if (exist_in_M(p1, M))
		{
			gtree *theOther = extract_theOther_in_M(p1, M);
			position2 = 1;
			p2 = p2_start;
			while (p2->type == state_or_declare)
			{
				if (p2 == theOther)
				{
					position_pair_list.push_back(make_pair(position1, position2));
				}
				position2++;
				p2 = p2->parent->next;
			}
		}
		position1++;
		p1 = p1->parent->next;
	}

	//根据position_pair_list先找到移动的pair
	vector<pair<int, int>> moves = process_move(position_pair_list);

	vector<pair<int, int>> modify, add, del;
	process_other(moves, position_pair_list, modify, add, del, position1, position2);

	if (is_state)
	{
		gtree *parent_statement1 = t1, *parent_statement2 = t2;
		while (parent_statement1 != NULL && parent_statement1->type != state_or_declare)
			parent_statement1 = parent_statement1->parent;
		while (parent_statement2 != NULL && parent_statement2->type != state_or_declare)
			parent_statement2 = parent_statement2->parent;
		if (parent_statement1 != NULL && parent_statement2 != NULL)
		{
			if (parent_statement1->parent->type == ITERATION_STATEMENT || parent_statement1->parent->type == SELECTION_STATEMENT)
			{
				Mapping m;
				m.map1 = parent_statement1->parent->parent;
				m.map2 = parent_statement2->parent->parent;
				M.push_back(m);
			}
			else
			{
				Mapping m;
				m.map1 = parent_statement1;
				m.map2 = parent_statement2;
				M.push_back(m);
			}
		}
	}
	else
	{
		Mapping m;
		m.map1 = t1->next;
		m.map2 = t2->next;
		if (exist_in_M(m, M))
		{
			gtree *parent_statement1 = t1, *parent_statement2 = t2;
			while (parent_statement1 != NULL && parent_statement1->type != STATEMENT)
				parent_statement1 = parent_statement1->parent;
			while (parent_statement2 != NULL && parent_statement2->type != STATEMENT)
				parent_statement2 = parent_statement2->parent;
			if (parent_statement1 != NULL && parent_statement2 != NULL)
			{
				if (parent_statement1->parent->type == ITERATION_STATEMENT || parent_statement1->parent->type == SELECTION_STATEMENT)
				{
					Mapping m;
					m.map1 = parent_statement1->parent->parent;
					m.map2 = parent_statement2->parent->parent;
					M.push_back(m);
				}
				else
				{
					Mapping m;
					m.map1 = parent_statement1;
					m.map2 = parent_statement2;
					M.push_back(m);
				}
			}
		}
	}
	changes.move = moves;
	changes.add = add;
	changes.modify = modify;
	changes.del = del;
	return changes;
}

void sort_M(vector<Mapping> &M)//冒泡算法
{
	for (int i = 0; i < int(M.size()) - 1; i++)
		for (int j = 0; j < int(M.size()) - i - 1; j++)
		{
			if (M[j].num > M[j + 1].num)
			{
				gtree *map1;
				gtree *map2;
				int num;
				map1 = M[j].map1;
				map2 = M[j].map2;
				num = M[j].num;
				M[j].map1 = M[j + 1].map1;
				M[j].map2 = M[j + 1].map2;
				M[j].num = M[j + 1].num;
				M[j + 1].map1 = map1;
				M[j + 1].map2 = map2;
				M[j + 1].num = num;
			}
		}
}

/*
extract_change函数
输入：两棵语法树、Top-down算法的输出map集
输出：所有语句列内语句级别的改动

*/

vector<AST_change> extract_change(gtree *t1, gtree *t2, vector<Mapping> &M)
{
	vector<AST_change> changes;

	vector<Mapping> container_list = find_container_mapping(M);
	//container_list.erase(unique(container_list.begin(), container_list.end()), container_list.end());
	sort_M(container_list);
	if (container_list.size() != 0)
	{
		for (int i = int(container_list.size() - 1); i >= 0; i--)
		{
			AST_change change = block_handle(container_list[i].map1, container_list[i].map2, M);
			if (change.m.map1 != NULL)
				changes.push_back(change);
		}
		return changes;
	}
	else
	{
		cout << "找不到container!" << endl;
		exit(1);
	}
}

