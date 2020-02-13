#include"tree.h"

void gtree::init(int root)
{
	this->type = root;
	this->type_1 = 0;
	this->child = NULL;
	this->next = NULL;
}

void gtree::cnode(int syn, int syn_1, gtree *t)
{
	t->next = new gtree;
	t->next->type = syn;
	t->next->type_1 = syn_1;
	t->next->child = NULL;
	t->next->next = NULL;
	t->next->parent = t->parent;
}

void gtree::newlable(int a)
{
	char b[3] = "0";
	if (a >= 10)
	{
		a = a - 10 + 'a' - '0';
	}
	b[0] += a;
	string c = b;
	place = "T" + c;
}

string gtree::newlable1(int a, int flag)
{
	char b[3] = "0";
	b[0] += a;
	string c = b;
	return("L" + c);
}

void gtree::expand(int num, gtree *t, int sum)//sum��ʾһЩ����������ظ�����
{
	t->child = new gtree;
	gtree *t1 = t->child;
	t1->child = NULL;
	t1->next = NULL;
	t1->parent = t;

	if (num == ����)//sum=0�����޳������ޱ���˵����1�����޳������б�����2�����б������޳�����3������
	{
		//���⴦��
		t1->type = kong;
		t1->type_1 = 0;
	}
	else if (num == ����˵��)//sum����
	{
		t1->type = �ؼ���;
		t1->type_1 = 10;//10����const
		cnode(��������, 0, t1);
		cnode(���, 0, t1->next);
	}
	else if (num == ��������)//1����int��2����float��3����double��4����char
	{
		if (sum == 1)
		{
			t1->type = ���ͱ�ʶ��;
			t1->type_1 = 0;
			cnode(��ʶ��, 0, t1);
			cnode(��ֵ��, 0, t1->next);
			cnode(����, 0, t1->next->next);
		}
		else if (sum == 8)
		{
			t1->type = ���ͱ�ʶ��;
			t1->type_1 = 0;
			cnode(��ʶ��, 0, t1);
			cnode(��ֵ��, 0, t1->next);
			cnode(ʵ��, 0, t1->next->next);
		}
		else if (sum == 9)
		{
			t1->type = ���ͱ�ʶ��;
			t1->type_1 = 0;
			cnode(��ʶ��, 0, t1);
			cnode(��ֵ��, 0, t1->next);
			cnode(ʵ��, 0, t1->next->next);
		}
		else if (sum == 7)
		{
			t1->type = ���ͱ�ʶ��;
			t1->type_1 = 0;
			cnode(��ʶ��, 0, t1);
			cnode(��ֵ��, 0, t1->next);
			cnode(�ַ�, 0, t1->next->next);
		}
	}
	else if (num == ����)//sum����+ -�ŵ�ֵ5.6����û��+-��Ϊ0
	{
		if (sum == 0)
		{
			t1->type = �޷�������;
			t1->type_1 = 0;
		}
		else if (sum == 5 || sum == 6)
		{
			t1->type = ���;
			t1->type_1 = sum;
			cnode(�޷�������, 0, t1);
		}
	}
	else if (num == ʵ��)//sum����+ -�ŵ�ֵ5.6����û��+-��Ϊ0
	{
		if (sum == 0)
		{
			t1->type = �޷���ʵ��;
			t1->type_1 = 0;
		}
		else if (sum == 5 || sum == 6)
		{
			t1->type = ���;
			t1->type_1 = sum;
			cnode(�޷���ʵ��, 0, t1);
		}
	}
	else if (num == ����ͷ��)//1����int��2����void��3����double��4����char��5����void
	{

		if (sum == 2)
		{
			t1->type = �ؼ���;
			t1->type_1 = 2;//2����void
			cnode(��ʶ��, 0, t1);
		}
		else
		{
			t1->type = ���ͱ�ʶ��;
			t1->type_1 = 0;
			cnode(��ʶ��, 0, t1);
		}
	}
	else if (num == ����˵��)//sum����
	{
		t1->type = ��������;
		t1->type_1 = 0;
		cnode(���, 0, t1);
	}
	else if (num == ��������)//sum����
	{

		t1->type = ���ͱ�ʶ��;
		t1->type_1 = 0;
		cnode(��ʶ��, 0, t1);

	}
	else if (num == ��ö�ٳ���)//sum=1�������� sum=2�����ַ�
	{
		if (sum == 1)
		{
			t1->type = ����;
			t1->type_1 = 0;
		}
		else if (sum == 2)
		{
			t1->type = �ַ�;
			t1->type_1 = 0;
		}
	}
	else if (num == ���ͱ�ʶ��)//sum����
	{

		t1->type = kong;
		t1->type_1 = 0;

	}
	else if (num == ��������)//sum����
	{
		t1->type = ����ͷ��;
		t1->type_1 = 0;
		cnode(������, 0, t1);
		cnode(������, 0, t1->next);
		cnode(������, 0, t1->next->next);
		cnode(�������, 0, t1->next->next->next);
		cnode(�������, 0, t1->next->next->next->next);
		cnode(�Ҵ�����, 0, t1->next->next->next->next->next);
	}
	else if (num == �������)//sum=0�����޳������ޱ���˵����1�����޳������б�����2�����б������޳�����3������
	{
		//���⴦��
		t1->type = kong;
		t1->type_1 = 0;
	}
	else if (num == ������)//sum=0����գ�sum=1�����в���
	{
		if (sum == 0)
		{
			t1->type = kong;
			t1->type_1 = 0;
		}
		else if (sum == 1)
		{
			t1->type = ���ͱ�ʶ��;
			t1->type_1 = 0;
			cnode(��ʶ��, 0, t1);
		}
	}
	else if (num == ���ʽ)//sum����+ -�ŵ�ֵ5.6����û��+-��Ϊ0
	{
		if (sum == 0)
		{
			t1->type = ��;
			t1->type_1 = 0;
		}
		else if (sum == 5 || sum == 6)
		{
			t1->type = ���;
			t1->type_1 = sum;
			cnode(��, 0, t1);
		}
	}
	else if (num == ��)//sum����
	{
		t1->type = ����;
		t1->type_1 = 0;
	}
	else if (num == ����)//sum����
	{
		t1->type = ����;
		t1->type_1 = 0;
	}
	else if (num == ����)//sum=1�����ʶ����sum=2�����ʶ��+[����]��sum=3�������ʽ����sum=4�����޷���������sum=5�����޷���ʵ����sum=6�����ַ���sum=7�������������
	{
		if (sum == 1)
		{
			t1->type = ��ʶ��;
			t1->type_1 = 0;
		}
		else if (sum == 2)
		{
			t1->type = ��ʶ��;
			t1->type_1 = 0;
			cnode(��������, 0, t1);
			cnode(���ʽ, 0, t1->next);
			cnode(��������, 0, t1->next->next);
		}
		else if (sum == 3)
		{
			t1->type = ��������;
			t1->type_1 = 0;
			cnode(���ʽ, 0, t1);
			cnode(��������, 0, t1->next);
		}
		else if (sum == 4)
		{
			t1->type = ����;
			t1->type_1 = 0;
		}
		else if (sum == 5)
		{
			t1->type = ʵ��;
			t1->type_1 = 0;
		}
		else if (sum == 6)
		{
			t1->type = �ַ�;
			t1->type_1 = 0;
		}
		else if (sum == 7)
		{
			t1->type = �����������;
			t1->type_1 = 0;
		}
	}
	else if (num == ���)//sum=1����������䣬sum=2����ѭ����䣬sum=3����{�����}��sum=4������������䣬sum=5����ֵ��䣬sum=6�������䣬sum=7����д��䣬sum=8����գ�sum=9���������䣬sum=10��������䣬sum=11������ʽ
	{
		if (sum == 1)
		{
			t1->type = �������;
			t1->type_1 = 0;
		}
		else if (sum == 2)
		{
			t1->type = ѭ�����;
			t1->type_1 = 0;
		}
		else if (sum == 3)
		{
			t1->type = �������;
			t1->type_1 = 0;
			cnode(�����, 0, t1);
			cnode(�Ҵ�����, 0, t1->next);
		}
		else if (sum == 4)
		{
			t1->type = �����������;
			t1->type_1 = 0;
			cnode(���, 0, t1);
		}
		else if (sum == 5)
		{
			t1->type = ��ֵ���;
			t1->type_1 = 0;
			cnode(���, 0, t1);
		}
		else if (sum == 6)
		{
			t1->type = �����;
			t1->type_1 = 0;
			cnode(���, 0, t1);
		}
		else if (sum == 7)
		{
			t1->type = д���;
			t1->type_1 = 0;
			cnode(���, 0, t1);
		}
		else if (sum == 8)
		{
			t1->type = kong;
			t1->type_1 = 0;
		}
		else if (sum == 9)
		{
			t1->type = ������;
			t1->type_1 = 0;
		}
		else if (sum == 10)
		{
			t1->type = �������;
			t1->type_1 = 0;
			cnode(���, 0, t1);
		}
		else if (sum == 11)
		{
			t1->type = ���ʽ;
			t1->type_1 = 0;
			cnode(���, 0, t1);
		}
		else if (sum == 12)
		{
			t1->type = break���;
			t1->type_1 = 0;
			cnode(���, 0, t1);
		}
	}
	else if (num == ��ֵ���)//sum=1���������ֵ��sum=2��������Ԫ�ظ�ֵ
	{
		if (sum == 1)
		{
			t1->type = ��ʶ��;
			t1->type_1 = 0;
			cnode(��ֵ��, 0, t1);
			cnode(���ʽ, 0, t1->next);
		}
		else if (sum == 2)
		{
			t1->type = ��ʶ��;
			t1->type_1 = 0;
			cnode(��������, 0, t1);
			cnode(���ʽ, 0, t1->next);
			cnode(��������, 0, t1->next->next);
			cnode(��ֵ��, 0, t1->next->next->next);
			cnode(���ʽ, 0, t1->next->next->next->next);
		}
	}
	else if (num == �������)//sum����
	{
		t1->type = �ؼ���;
		t1->type_1 = 3;//3����if
		cnode(������, 0, t1);
		cnode(������, 0, t1->next);
		cnode(������, 0, t1->next->next);
		cnode(���, 0, t1->next->next->next);
	}
	else if (num == ������)//sum����
	{
		t1->type = kong;
		t1->type_1 = 0;
	}
	else if (num == ����)//sum����
	{
		t1->type = ���ʽ;
		t1->type_1 = 0;
	}
	else if (num == ѭ�����)//sum���ã�forתwhile����
	{

		t1->type = �ؼ���;
		t1->type_1 = 5;//5����while
		cnode(������, 0, t1);
		cnode(������, 0, t1->next);
		cnode(������, 0, t1->next->next);
		cnode(���, 0, t1->next->next->next);

	}
	else if (num == ������)//sum����
	{
		t1->type = �ؼ���;
		t1->type_1 = 11;//11����switch
		cnode(������, 0, t1);
		cnode(���ʽ, 0, t1->next);
		cnode(������, 0, t1->next->next);
		cnode(�������, 0, t1->next->next->next);
		cnode(�����, 0, t1->next->next->next->next);
		cnode(ȱʡ, 0, t1->next->next->next->next->next);
		cnode(�Ҵ�����, 0, t1->next->next->next->next->next->next);
	}
	else if (num == �����)//sum����
	{
		t1->type = ��������;
		t1->type_1 = 0;
	}
	else if (num == ��������)//sum����
	{
		t1->type = �ؼ���;
		t1->type_1 = 12;//12����case
		cnode(��ö�ٳ���, 0, t1);
		cnode(ð��, 0, t1->next);
		cnode(���, 0, t1->next->next);
	}
	else if (num == ȱʡ)//sum=0����գ�sum=1����ǿ�
	{
		if (sum == 0)
		{
			t1->type = kong;
			t1->type_1 = 0;
		}
		else if (sum == 1)
		{
			t1->type = �ؼ���;
			t1->type_1 = 15;//15����default
			cnode(ð��, 0, t1);
			cnode(���, 0, t1->next);
		}
	}
	else if (num == �����������)//sun����
	{
		t1->type = ��ʶ��;
		t1->type_1 = 0;
		cnode(������, 0, t1);
		cnode(ֵ������, 0, t1->next);
		cnode(������, 0, t1->next->next);
	}
	else if (num == ֵ������)//sum=0����գ�sum=1����ǿ�
	{
		if (sum == 0)
		{
			t1->type = kong;
			t1->type_1 = 0;
		}
		else if (sum == 1)
		{
			t1->type = ����;
			t1->type_1 = 0;
		}
	}
	else if (num == �����)//sum����
	{
		t1->type = kong;
		t1->type_1 = 0;
	}
	else if (num == �����)//�������Ҫ��������Ҵ�����
	{
		t1->type = �ؼ���;
		t1->type_1 = 13;//13����scanf
		cnode(������, 0, t1);
		cnode(��ʶ��, 0, t1->next);
	}
	else if (num == д���)//sum=2�����ַ����������Լӣ���sum=3������ʽ
	{
		t1->type = �ؼ���;
		t1->type_1 = 14;//14����printf
		cnode(������, 0, t1);

		if (sum == 2)
		{
			cnode(�ַ���, 0, t1->next);
		}
		else if (sum == 3)
		{
			cnode(���ʽ, 0, t1->next);
			cnode(������, 0, t1->next->next);
		}
	}
	else if (num == �������)//sum����
	{
		t1->type = �ؼ���;
		t1->type_1 = 6;//6����return
	}
	else if (num == break���)
	{
		t1->type = �ؼ���;
		t1->type_1 = 20;//break
	}
	else
	{
		exit(-1);
	}
}