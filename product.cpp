#include"product.h"
#define FILE_BUFFER_LENGTH 30000
#include <fstream>

/******************************Global_fucntions*********************************/
bool judgeF(string s)//�ж���F����C���� 
{
	int pos = s.find("<=");
	if (pos == string::npos)
	{
		return true;            //��F���� 
	}
	else return false;          //c���� 
}

/*
int convert(RG rg, C_Petri ptnet, int i, int j)//rgnode[i].m[j]
{
	int x = 0;
	int sum1 = 0;
	int sum2 = 0;

	for (int ii = 0; ii < ptnet.m; ii++)
	{
		sum1 += ptnet.place[j].solution[ii] * rg.rgnode[0].m[ii];
		//cout << "ptnet.place["<<j<<"].solution["<<ii<<"]: " << ptnet.place[j].solution[ii]<<endl;
		//cout << "rg.rgnode[0].m[" << ii << "]: " << rg.rgnode[0].m[ii] << endl;
	}
	//cout << "sum1:" << sum1 << endl;
	for (int jj = 0; jj < ptnet.m; jj++)
	{
		if (jj == j)
			continue;
		else
			sum2 += ptnet.place[j].solution[jj] * rg.rgnode[i].m[jj];
	}
	//cout << "sum2:" << sum2 << endl;
	x = (sum1 - sum2) / ptnet.place[j].solution[j];

	return x;
}*/



int sumtoken(C_Petri ptnet, RG rg, string s, int statenum)//���������token�ĺ� ��������s��  p1,p2,p3,p4,  ����ʽ   
{
	int sum = 0;
	while (1)
	{
		int pos = s.find_first_of(",");
		if (pos == string::npos)
			break;//��û���ҵ� ��˵��û���� 
		string subs = s.substr(0, pos);     //ȡ��һ��p1 
		//	cout<<"   "<<subs<<" ";
		for (int i = 0; i < ptnet.p_num; i++)      //ȥ�����ĵ�������Ѱ�Ҷ�Ӧ���ֵĿ��� �õ��±� 
		{
			if (subs == ptnet.place[i].name)//name ���� vname?????
			{
				int idex = ptnet.place[i].id_num;                 //�õ������ı��
				/*Ŀǰ����int����� ֱ��ʹ��m�µ�token_num
				 *��������string��char��double���͵�
				 */
				sum += rg.rgnode[statenum].m[idex].token_num;
				break;
			}
		}
		s = s.substr(pos + 1, s.length() - pos);          //��ǰ����ù���P1ȥ�� ��p2��ʼ��Ϊ�µ�s�� 
	}
	return sum;
}
/*
int localBA(SBA ba, string val)//����data  �����Զ��� �ڽӱ���±�
{
	for (int i = 0; i < ba.svex_num; i++)
	{
		if (itos(ba.svertics[i].data) == val)
			return i;
	}
	return -1;//�Ҳ����򷵻�-1
}
*/

/*
int localRG(RG rg, string val)//����data  ���ؿɴ�ͼ �ڽӱ���±�
{
	for (int i = 0; i < rg.node; i++)
	{
		if (val == rg.rgnode[i].name)
			return i;
		//��int����ר��string �ٽ��бȽ�
		/*string str="";
		for(int k=0;k<5;k++)//��int����ר��string
		{
		str+=to_String(  rg.rgnode[i].m[k]   );
		}

		if(str==val)
		return i;
	}
	return -1;
}
*/

bool handleFLTLF(C_Petri ptnet, RG rg, string s, int statenum)//����F����  
{
	int i, j;//whileѭ������ 
	if (s[0] == '!')//��ԭ����Ļ����� ��ȡ��
	{

		s = s.substr(2, s.length() - 1);//ȥ�� !, { 
		bool flag = true;             //��ʼ��Ϊtrue  
		while (1)
		{
			int pos = s.find_first_of(",");
			if (pos < 0)
				break;
			string subs = s.substr(0, pos);//ȡ��һ��t1 ȥ�� ��Ǩ����Ƚ� �õ���� ��ȥisfirable������ ����Ƿ�ȫ������	
			for (i = 0; i < ptnet.t_num; i++)
			{
				if (ptnet.transition[i].name == subs)
				{
					int idex = ptnet.transition[i].id_num;//�õ��˱�Ǩ�ı��
					//��ȥisfirabe�������Ƿ���������
					for (j = 0; j < rg.rgnode[statenum].enableNum; j++)//30��isfirable����Ĵ�С 
					{
						if (idex == rg.rgnode[statenum].isfirable[j])
						{
							break;
						}
					}
					if (j >= rg.rgnode[statenum].enableNum) {
						flag = false;//isfirable������û�ҵ� ����Ϊfalse ������ѭ�� 
					}
					break; //���ֶ�Ӧ�Ŀ϶�ֻ��һ�� ������  
				}
			}
			if (flag == false)
			{
				//��t1�Ҳ�����Ӧ��  ��һ��t2Ҳ���ÿ��� ֱ������while 
				break;
			}

			//cout<<subs<<endl;
			s = s.substr(pos + 1, s.length() - pos);
		}
		return flag;
		//ȫ�ҵ�Ϊtrue  ��֮Ϊfalse
	}
	else
	{
		s = s.substr(1, s.length() - 1);//ȥ��{ 
		bool flag = false;//��ʼ״̬Ϊfalse  t1,t2,t3, ֻҪ����һ��t�Ҳ��� ��Ϊtrue ������(����ѭ��) 
		while (1)
		{
			int pos = s.find_first_of(",");
			if (pos < 0)break;
			string subs = s.substr(0, pos);//ȡ��һ��t1 ȥ�� ��Ǩ����Ƚ� �õ���� ��ȥisfirable������ ����Ƿ�ȫ������
			//bool flag=true;//���t1�Ƿ����ҵ� 

			for (i = 0; i < ptnet.t_num; i++)
			{
				if (ptnet.transition[i].name == subs)
				{
					int idex = ptnet.transition[i].id_num;//�õ��˱�Ǩ�ı��
					//��ȥisfirabe�������Ƿ���������
					for (j = 0; j < rg.rgnode[statenum].enableNum; j++)
					{
						if (idex == rg.rgnode[i].isfirable[j])
						{
							break;
						}
					}
					if (j >= rg.rgnode[statenum].enableNum) {
						flag = true;//isfirable������û�ҵ� 
					}
					break; //���ֶ�Ӧ�Ŀ϶�ֻ��һ�� ������  
				}
			}
			if (flag == true)
			{
				//��t1�Ҳ�����Ӧ��  ��һ��t2Ҳ���ÿ��� ֱ������while 
				break;
			}
			//cout<<subs<<endl;
			s = s.substr(pos + 1, s.length() - pos);
		}
		return flag;
		//ȫ�Ҳ���Ϊfalse  
	}
}
void handleLTLCstep(C_Petri ptnet, RG rg, int &front_sum, int &latter_sum, string s, int statenum)
{
	if (s[0] == 't')//��ǰ�벿��Ϊtoken-count��ʽ  
	{
		int pos = s.find_first_of("<=");//��λ��<=,ȡ��ǰ�벿��  
		string s_tokon = s.substr(12, pos - 13);//ȥ�� "token-count(" ")"  ֻʣp1,p2,    
		//cout<<" "<<s_tokon<<" ";
		front_sum = sumtoken(ptnet, rg, s_tokon, statenum);//����token�ĺ� 

		//�����벿��  
		s = s.substr(pos + 2, s.length() - pos - 2); //ȡ�ú�벿��  ���ǳ��� �������һ���ೣ������һ��'}'

		if (s[0] == 't')//����벿�ֻ���token-count 
		{
			string s_tokon = s.substr(12, pos - 13);//ȥ�� token-count(
		//	cout<<" "<<s_tokon<<" ";
			latter_sum = sumtoken(ptnet, rg, s_tokon, statenum);//��ͬ���� 

		}
		else//��벿���ǳ��� 
		{
			s = s.substr(0, s.length() - 1);//ȥ��}  ֻʣһ������ 
			latter_sum = atoi(s.c_str());
			//	 cout<<latter_sum;
		}

	}
	else//ǰ�벿���ǳ��� �Ǻ���һ����token-count��ʽ 
	{
		//����ǰ�벿��  ���еĴ� �Ǵӳ�����ʼ�� 
		int pos = s.find_first_of("<=");//��λ��<=,ȡ��ǰ�벿��
		string num = s.substr(0, pos);//ȡ������ 
		front_sum = atoi(num.c_str());
		//	 cout<< " "<<front_sum<<" ";

			 //�����벿��
		s = s.substr(pos + 14, s.length() - pos - 15);
		//	 cout<<" "<<s<<" ";
		latter_sum = sumtoken(ptnet, rg, s, statenum);
	}
}
bool handleLTLC(C_Petri ptnet, RG rg, string s, int statenum, ofstream &ofe)//����C����  
{
	int front_sum, latter_sum;//ǰ�벿�ֺ� ��벿�ֵĺ� 
	if (s[0] == '!')//����Ϊ�� ��ȡ��
	{
		//	cout<<" �� ";
		s = s.substr(2, s.length() - 2);//ȥ�� "!{" 
	//	cout<<s;
		handleLTLCstep(ptnet, rg, front_sum, latter_sum, s, statenum);
		ofe << "ǰ�벿�ֺͣ�" << front_sum << " ��벿�ֺͣ�" << latter_sum << endl;
		if (front_sum <= latter_sum)
			return true;
		else
			return false;

	}
	else
	{
		//	cout<<" �� ";
		s = s.substr(1, s.length() - 1);//ȥ�� "{"
	//	cout<<" "<<s;
		handleLTLCstep(ptnet, rg, front_sum, latter_sum, s, statenum);
		ofe << "ǰ�벿�ֺͣ�" << front_sum << " ��벿�ֺͣ�" << latter_sum << endl;
		if (front_sum <= latter_sum)
			return false;
		else
			return true;
	}
}
/****************************Global_fucntions End*******************************/
int cnt, tot, stack_id;
string to_String(int n)//������intת����string 
{
	int m = n;
	char s[max_to_string];
	char ss[max_to_string];
	int i = 0, j = 0;
	if (n < 0)// ������
	{
		m = 0 - m;
		j = 1;
		ss[0] = '-';
	}
	while (m > 0)
	{
		s[i++] = m % 10 + '0';
		m /= 10;
	}
	s[i] = '\0';
	i = i - 1;
	while (i >= 0)
	{
		ss[j++] = s[i--];
	}
	ss[j] = '\0';
	return ss;
}

void Product_Automata::getProduct(C_Petri ptnet, RG rg, SBA ba)//����ɴ�ͼ���Զ��� 
{
	int cont;                                  //���Զ��������
	ofstream outfile("getproduct.txt", ios::out);
	ofstream ofe("islabel.txt", ios::out);
	outfile << endl;
	outfile << "���BA�ڵ��ϵ�label��\n";
	for (int i = 0; i < ba.svex_num; i++)
	{
		outfile << "�ڵ�" << i << "��label:" << ba.svertics[i].label << endl;
		outfile << "�ڵ�" << i << "��isaccept:" << ba.svertics[i].isAccept << endl;
		outfile << "�ڵ�" << i << "��isinitial:" << ba.svertics[i].isInitial << endl;


	}
	outfile << endl;
	cont = 1;
	for (int i = 0; i < rg.node_num; i++)//�����ɴ�ͼÿһ���ڵ�
	{
		for (int j = 0; j < ba.svex_num; j++)//�����Զ���ÿһ���ڵ�
		{
			if (isLabel(ptnet, rg, ba, i, j, ofe))                                  //���⽻���ǿ�   
			{

				Product N;                                   //����һ��״̬�ڵ�N 
				//N.BAname = itos(ba.svertics[j].data);       //�Զ��� 
				//N.RGname = rg.rgnode[i].name;    //�ɴ�ͼ
				N.BAname_id = j;
				N.RGname_id = i;
				N.pro_id = cont++;                              //���Զ��������+1 
				addstatus(N);                               //���ӽ��Զ�����״̬ 
				outfile << "�����ӵ�Ǩ�ƹ�ϵ��\n";
				addtransitionF(rg, ba, i, j, N, outfile);                  //��������Ǩ��״̬ 
				addtransitionR(rg, ba, i, j, N, outfile);                  //��������Ǩ��״̬ 
				addinitial_status(ba, i, j, N);                  //���ӳ�ʼ״̬ 
				addisaccept(ba, j, N);                         //���ӿɽ���״̬ 
			}
		}
	}
}

void Product_Automata::addstatus(Product n)//���ӽ��Զ�����״̬ 
{
	status.push_back(n);//����״̬�ڵ� ����״̬������ 
}

void Product_Automata::addtransitionF(RG rg, SBA ba, int i, int j, Product n, ofstream &outfile)    //���ӽ��Զ�����Ǩ��״̬ 
{
	outfile << "��������Ǩ�ƹ�ϵ����" << endl;
	vector<Product>::iterator it_P;       //����ǰ������� 
	SArcNode *pba = new SArcNode;        //ָ��Ba�Զ������ڽӱ�Ľڵ� (��ͷ�ڵ�) 

	bool flag_v, flag_s;    //����Ƿ��ҵ���Ӧ�ı� 

	outfile << "N��RGname:" << n.RGname << " N��BAname:" << n.BAname << endl;
	//�����еĳ˻��� ����Ѱ�ҷ���Ҫ���ĳ���ڵ� ʹ����n���,�����Ǩ���� 
	for (it_P = status.begin(); it_P != status.end(); it_P++)
	{
		outfile << "�����õ���RGname:" << (*it_P).RGname << " BAname:" << (*it_P).BAname << endl;
		//���ݴ���ı�������λ(�����±�)����ʱ����idex���淵�ص��±�
		int idex = it_P->RGname_id;

		//��idexΪ���ȥ���� 	 
		RGNode temprgnode = rg.rgnode[idex];
		//ȥ����temprgnode��next����
		flag_v = false;//Ĭ��Ϊfalse �ҵ����Ϊtrue
		for (unsigned int k = 0; k < temprgnode.next.size(); k++)
		{
			if (temprgnode.next[k].num == i)
			{
				flag_v = true;
				break;
			}

		}

		/*
		prg = rg.rgnode[idex].firstEdge;//prgָ��idex����ȥ�ĵ�һ���ڵ�

		while (prg != NULL)
		{
			if (prg->target == i) //�ڿɴ�ͼ���ҵ�
			{
				flag_v = true;
				break;
			}
			else
				prg = prg->nextedge;
		}
		if (prg == NULL)
			flag_v = false;//δ�ҵ� */


			//�ô�������j  �Լ� status���ϱ����õ���BAname  �ж����Ƿ����Զ�����Ǩ�ƣ������ڽӱ����ж� 
		idex = it_P->BAname_id;//��ʱ����idex���淵�ص��±�

		//��idexΪ���  ȥѰ���Ƿ����t 
		pba = ba.svertics[idex].firstarc;
		//cout << "ba.vertics[idex]:"<< ba.vertics[idex].data  << endl;
		while (pba != NULL)
		{
			if (pba->adjvex == j) //���Զ����ڽӱ����ҵ� 
			{
				flag_s = true;
				break;
			}
			pba = pba->nextarc;
		}
		if (pba == NULL)
			flag_s = false;//δ�ҵ� 


		if (flag_v && flag_s) //�������� ���Զ�����Ǩ�ƹ�ϵ���� ���� 
		{
			T t;//��ʱ���� 
			t.s = (*it_P);
			t.e = n;
			transition.push_back(t);//����Ǩ�ƹ�ϵ 
			outfile << " ������������,��ӵ�T������,����id:" << t.s.pro_id << " �յ��id:" << t.e.pro_id << endl;
		}
	}
	outfile << "\n\n\n";
}

//i�ǿɴ�ͼ�� j���Զ�����
void Product_Automata::addtransitionR(RG rg, SBA ba, int i, int j, Product n, ofstream &outfile)    //���ӽ��Զ�����Ǩ��״̬ 
{
	outfile << "��ӷ����Ǩ�ƹ�ϵ����" << endl;
	vector<Product>::iterator it_P;       //����ǰ������� 
	SArcNode *pba = new SArcNode;        //ָ��Ba�Զ������ڽӱ�Ľڵ� (��ͷ�ڵ�) 
	bool flag_v, flag_s;    //����Ƿ��ҵ���Ӧ�ı� 

	outfile << "N��RGname:" << n.RGname << " N��BAname:" << n.BAname << endl;
	//�����еĳ˻��� ����Ѱ�ҷ���Ҫ���ĳ���ڵ� ʹ����n���,�����Ǩ���� 
	for (it_P = status.begin(); it_P != status.end(); it_P++)
	{
		outfile << "�����õ���RGname:" << (*it_P).RGname << " BAname:" << (*it_P).BAname << endl;
		//���ݴ���ı�������λ(�����±�)����ʱ����idex���淵�ص��±�
		int idex = it_P->RGname_id;
		RGNode temprgnode = rg.rgnode[i];
		//ȥ����temprgnode��next����
		flag_v = false;//Ĭ��Ϊfalse �ҵ����Ϊtrue
		for (unsigned int k = 0; k < temprgnode.next.size(); k++)
		{
			if (temprgnode.next[k].num == idex)
			{
				flag_v = true;
				break;
			}

		}

		/*
		//��idexΪ���ȥ����
		prg = rg.rgnode[i].firstEdge;//prgָ��idex����ȥ�ĵ�һ���ڵ�

		while (prg != NULL)
		{
			//
			if (prg->target == idex) //�ڿɴ�ͼ���ҵ�
			{
				flag_v = true;
				break;
			}
			else
				prg = prg->nextedge;
		}
		if (prg == NULL)
			flag_v = false;//δ�ҵ�
*/

//�ô�������j  �Լ� status���ϱ����õ���BAname  �ж����Ƿ����Զ�����Ǩ�ƣ������ڽӱ����ж� 
		idex = it_P->BAname_id;//��ʱ����idex���淵�ص��±�

		//��idexΪ���  ȥѰ���Ƿ����t 
		pba = ba.svertics[j].firstarc;
		//cout << "ba.vertics[idex]:"<< ba.vertics[idex].data  << endl;
		while (pba != NULL)
		{
			if (pba->adjvex == idex) //���Զ����ڽӱ����ҵ� 
			{
				flag_s = true;
				break;
			}
			pba = pba->nextarc;
		}
		if (pba == NULL)
			flag_s = false;//δ�ҵ� 


		if (flag_v && flag_s) //�������� ���Զ�����Ǩ�ƹ�ϵ���� ���� 
		{
			T t;//��ʱ���� 
			t.s = n;
			t.e = (*it_P);
			transition.push_back(t);//����Ǩ�ƹ�ϵ 
			outfile << " ������������,��ӵ�T������,����id:" << t.s.pro_id << " �յ��id:" << t.e.pro_id << endl;
		}
	}
	outfile << "\n\n\n";
}


void  Product_Automata::addinitial_status(SBA ba, int i, int j, Product n)//���ӳ�ʼ״̬ 
{

	if (ba.svertics[j].isInitial  && i == 0)//��i����Ŀɴ�ͼ�ڵ�  �� j�����BA�ڵ�  ���Ǹ��ڵ㣨��ʼ״̬�ڵ㣩 
	{
		initial_status.push_back(n);
	}
}

void  Product_Automata::addisaccept(SBA ba, int i, Product n)//���ӿɽ���״̬ 
{
	//�ɴ�ͼ��ÿһ���ڵ�Ĭ���ǿɽ���״̬ ������ֻ��Ҫ�����Զ�������  	
	bool flag = ba.svertics[i].isAccept;//�ж����Ƿ��ǿɽ���״̬ 
	if (flag)
	{
		isaccept.push_back(n);
	}
}

bool isLabel(C_Petri ptnet, RG rg, SBA ba, int vi, int sj, ofstream &ofe)//vi�ǿɴ�ͼ���±� sj���Զ������±� 
{

	string str = ba.svertics[sj].label;//�����Զ����ڵ������� 
	if ("" == str)
		return true;//������Ϊ��  
	bool  mark = false; //mark����Ƿ���������� ��һֱ��false �ǽ����Ƿ�false
	while (1)
	{
		int pos = str.find_first_of("&&");
		ofe << "RG���:" << vi << " BA���:" << sj << " BA��label:" << str << endl;
		if (pos == string::npos)//�������һ�� 
		{
			//cout<<str;
			if (judgeF(str))//F���� 
			{
				//cout<<"F����"; 
				mark = handleFLTLF(ptnet, rg, str, vi);
				if (mark == true)
				{
					break;//����whileѭ��  
				}
			}
			else {//c���� 
				//cout<<"c����";
				mark = handleLTLC(ptnet, rg, str, vi, ofe);
				if (mark == true)
				{
					break;//����whileѭ��  
				}
			}
			break;//��Ϊ�����һ�� ����ѭ�� 
		}

		string subprop = str.substr(0, pos);//ȡ��һ������ 
		//cout<<subprop<<" ";

		if (judgeF(subprop))//F���� 
		{
			//	cout<<"F����"; 
			mark = handleFLTLF(ptnet, rg, subprop, vi);
			if (mark == true)//���ҵ�һ������ ���� ����ѭ��   
			{
				break;
			}
		}
		else//c���� 
		{
			//	cout<<"c����"; 
			mark = handleLTLC(ptnet, rg, subprop, vi, ofe);
			if (mark == true)//���ҵ�һ������ ���� ����ѭ��   
			{
				break;
			}
		}
		//cout<<endl;
		str = str.substr(pos + 2, str.length() - pos);
	}

	if (mark == true)//��һ�����������
	{

		ofe << "��Ϊfalse" << endl << endl;
		return false;
	}

	else //�������ⶼ������ markһֱ��false�� �����˷�false  
	{
		ofe << "��Ϊ��false ���ɽڵ�" << endl << endl;
		return true;
	}

}

void add(int x, int y, node *edge, int *heads)//ͨ��Ǩ�ƹ�ϵ �洢���Զ�����ͼ 
{
	//��ʽǰ���Ǵ洢 
	edge[++cnt].next = heads[x];
	edge[cnt].to = y;
	heads[x] = cnt;
	return;
}

Product_Automata::Product_Automata()
{

}
void Product_Automata::tarjan(int x, node *edge, int *LOW, int *DFN, int *stack, int *visit, int *heads)//����ڼ������ڴ����ݹ���ǵ㡣
{
	if (result == false)
	{
		return;
	}
	//x���ǵ�һ���㣬���䴦�� 
	DFN[x] = LOW[x] = ++tot;// �½���ĳ�ʼ����

	stack[++stack_id] = x;//��ջ 

	visit[x] = 1;//����� ����ջ��
   //forѭ��  ��x->����һ������д��� 
	for (int i = heads[x]; i != -1; i = edge[i].next)
	{

		if (!DFN[edge[i].to]) {//���û���ʹ�

			tarjan(edge[i].to, edge, LOW, DFN, stack, visit, heads);//���½������죬��ʼ�ݹ�
			if (!result)return;

			LOW[x] = min(LOW[x], LOW[edge[i].to]);//�ݹ�������Ƚ�˭��˭�Ķ��ӣ����ף��������Ķ�Ӧ��ϵ���漰��ǿ��ͨ����������С�������顣

		}

		else if (visit[edge[i].to]) {  //������ʹ������һ���ջ�

			LOW[x] = min(LOW[x], LOW[edge[i].to]);//�Ƚ�˭��˭�Ķ��ӣ����ס��������Ӷ�Ӧ��ϵ

		}

	}
	if (LOW[x] == DFN[x] && stack[stack_id] == x)//���˵�����
	{
		visit[stack[stack_id]] = 0;

		stack_id--;
	}
	else if (LOW[x] == DFN[x]) //����������ǿ��ͨ�������������С����

	{
		//cout << "flag****************************";
		vector<int>::iterator it;
		bool flag = false;//��ǽڵ��Ƿ��ǿɽ���״̬ 
		do {
			//cout << stack[stack_id] << " ";

			it = find(isAccept_id.begin(), isAccept_id.end(), stack[stack_id]);

			if (it != isAccept_id.end())//�ڿɽ���״̬�������ҵ���id
			{
				flag = true;
			}
			visit[stack[stack_id]] = 0;

			stack_id--;

		} while (x != stack[stack_id + 1]);//��ջ�����������

		if (flag)
		{
			//cout << "�����ɽ���״̬";
			result = false;
		}
		//cout << endl;
	}

	return;

}

void Product_Automata::ModelChecker(C_Petri ptnet, RG rg, SBA ba, string propertyid)
{
	ofstream outfile("transition.txt", ios::out);
	vector<Product>::iterator it_P;       //����ǰ������� 
	vector<T>::iterator it_T;                  //����ǰ������� 

	result = true;//���������ʼĬ��Ϊtrue
	tot = cnt = stack_id = 0;
	getProduct(ptnet, rg, ba);//���ɽ��Զ���
	//cout<<"ok \n";
	initate_array();
	int x, y;//�����x->y 
	//����һ��set���� �����ǿɽ���״̬��id
	outfile << isaccept.size() << endl;
	outfile << status.size() << endl;
	outfile << transition.size() << endl;
	outfile << endl;
	for (it_P = isaccept.begin(); it_P != isaccept.end(); it_P++)
	{
		//cout << "id:" << (*it_P).id << " BAname:" << (*it_P).BAname << " RGname:" << (*it_P).RGname << endl;
		outfile << it_P->pro_id << endl;
		isAccept_id.push_back(it_P->pro_id);
	}

	//cout << "����״̬�ĸ���:" << ct << endl;
	//cout << "�ߵĹ�ϵ���ϣ�\n";
	//outfile << "�߼��ϣ�" << endl;
	for (it_T = transition.begin(); it_T != transition.end(); it_T++)
	{
		x = (*it_T).s.pro_id;
		y = (*it_T).e.pro_id;
		outfile << x << " " << y << endl;

		//cout << "<" << x << "," << y << ">" << endl;
		add(x, y, edge, heads);//����add()���� ������ʽǰ���Ǵ洢  
	}
	//cout <<"Transition num:" << transition.size() << endl;
	//������ʼ״̬�ļ��� ���ӳ�ʼ״̬���� 
	outfile << "��ʼ״̬����ţ�" << endl;
	for (it_P = initial_status.begin(); it_P != initial_status.end(); it_P++)
	{
		int t = (*it_P).pro_id;
		outfile << t << endl;

		//cout << t << endl;
		if (!DFN[t])
			tarjan(t, edge, LOW, DFN, stack, visit, heads);//�������û�з��ʹ����ʹӴ˵㿪ʼ����ֹ���Զ�������ͼû����
	}
	/*for (int i = 1; i <= 24; i++)
	{
		if (!DFN[i])
			tarjan(i);
	}*/

	string re;
	if (result)
	{
		re = "TRUE";
	}
	else
	{
		re = "FALSE";
	}

	cout << "FORMULA " + propertyid + " " + re << endl;
	cout << "======================================================" << endl;
	initate_array();
	/*delete[] DFN;
	delete[] LOW;
	delete[] stack;
	delete[] visit;
	delete[] edge;
	delete[] heads;*/
	return;
}

void Product_Automata::initate_array()
{
	heads = new int[status.size() + 1];
	DFN = new int[status.size() + 1];
	LOW = new int[status.size() + 1];
	stack = new int[status.size() + 1];
	visit = new int[status.size() + 1];
	edge = new node[transition.size() + 1];
	result = true;
	for (unsigned int i = 0; i < status.size() + 1; i++)
	{
		heads[i] = -1;
		DFN[i] = LOW[i] = stack[i] = visit[i] = 0;
	}

}
Product_Automata::~Product_Automata()
{
	delete[] DFN;
	delete[] LOW;
	delete[] stack;
	delete[] visit;
	delete[] edge;
	delete[] heads;
}
