#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <stdio.h>  
#include <string.h>
#include<string>
#include <iomanip>
#include<fstream>
#include<stack>
#include<sstream>
#include<algorithm>
#include<time.h>
#include"tree.h"
#include"cpn.h"
#include"RG.h"
#include "product.h"
#include "xml2ltl.h"
#include "SBA.h"

int model_check(C_Petri petri,RG rg)
{
	//公式解析
	char filename[50] = "formulaC.xml";  //C公式
	char filenameF[50] = "formulaF.xml"; //F公式

	//***************************************************************
	//C公式
	ifstream infile(filename, ios::in);
	if (!infile)
		return 0;
	bool Reachability = false;

	//char file[50] = "LTLCardinality.xml";
	convertc(filename);

	string S, propertyid; //propertyid stores names of LTL formulae
	char form[20000];     //store LTL formulae

	ifstream read("formulaC.txt", ios::in);
	if (!read) {
		cout << "error!";
		getchar();
		exit(-1);
	}

	while (getline(read, propertyid, ':')) {

		cout << propertyid << ':';
		getline(read, S);
		strcpy_s(form, S.c_str());
		//cout << form << endl;
		cout << endl;
		//lexer
		Lexer *lex = new Lexer(form, S.length());
		//syntax analysis
		Syntax_Tree *ST;
		ST = new Syntax_Tree;
		formula_stack Ustack;
		ST->reverse_polish(*lex);
		ST->build_tree();
		cout << "The syntax tree of unsimplified formula£º" << endl;
		ST->print_syntax_tree(ST->root, 0);
		//LTL formula rewrite
		ST->simplify_LTL(ST->root->left);
		/*cout << endl;
		cout << "The syntax tree of simplified formula£º" << endl;
		ST.print_syntax_tree(ST.root, 0);*/
		//syntax tree convert
		ST->negconvert(ST->root->left, Ustack);
		delete lex;
		/*cout << endl;
		cout << "The converted formula£º" << endl;
		cout << ST.root->left->formula << endl;
		cout << endl;*/
		//Êä³öU×ÓÊ½
		/*cout << "The subformulas of LTL whose main operator is \'U\'£º" << endl;
		vector<STNode>::iterator Uiter;
		for (Uiter = Ustack.loc.begin(); Uiter != Ustack.loc.end(); Uiter++)
		{
			cout << (*Uiter)->formula << endl;
		}
		cout << endl;*/
		//TGBA
		TGBA *Tgba;
		Tgba = new TGBA;
		Tgba->CreatTGBA(Ustack, ST->root->left);
		Tgba->SimplifyStates();
		delete ST;
		//cout << endl;
		//¹¹ÔìTBA
		TBA *tba;
		tba = new TBA;
		tba->CreatTBA(*Tgba, Ustack);
		delete Tgba;
		string filename = propertyid + ".txt";
		tba->PrintBuchi(filename);
		/*cout << "Please check the file" + filename + ". In this file you can see the Buchi automata related to the LTL formula!";
		cout << endl;*/
		//SBA
		SBA sba;

		sba.CreatSBA(*tba);
		sba.Simplify();
		sba.Compress();
		delete tba;


		//生成交自动机 并搜索
		Product_Automata *product = new Product_Automata;
		product->ModelChecker(petri, rg, sba, propertyid);
		delete product;
	}

	//***************************************************************
	//F公式
	ifstream infileF(filenameF, ios::in);
	if (!infileF)
		return 0;
	Reachability = false;
	convertf(filenameF);

	ifstream readF("formulaF.txt", ios::in);
	if (!readF) {
		cout << "error!";
		getchar();
		exit(-1);
	}

	while (getline(readF, propertyid, ':')) {

		cout << propertyid << ':';
		getline(readF, S);
		strcpy_s(form, S.c_str());
		//cout << form << endl;
		cout << endl;
		//lexer
		Lexer *lex = new Lexer(form, S.length());
		//syntax analysis
		Syntax_Tree *ST;
		ST = new Syntax_Tree;
		formula_stack Ustack;
		ST->reverse_polish(*lex);
		ST->build_tree();
		cout << "The syntax tree of unsimplified formula￡o" << endl;
		ST->print_syntax_tree(ST->root, 0);
		//LTL formula rewrite
		ST->simplify_LTL(ST->root->left);
		/*cout << endl;
		cout << "The syntax tree of simplified formula￡o" << endl;
		ST.print_syntax_tree(ST.root, 0);*/
		//syntax tree convert
		ST->negconvert(ST->root->left, Ustack);
		delete lex;
		/*cout << endl;
		cout << "The converted formula￡o" << endl;
		cout << ST.root->left->formula << endl;
		cout << endl;*/
		//ê?3?U×óê?
		/*cout << "The subformulas of LTL whose main operator is \'U\'￡o" << endl;
		vector<STNode>::iterator Uiter;
		for (Uiter = Ustack.loc.begin(); Uiter != Ustack.loc.end(); Uiter++)
		{
			cout << (*Uiter)->formula << endl;
		}
		cout << endl;*/
		//TGBA
		TGBA *Tgba;
		Tgba = new TGBA;
		Tgba->CreatTGBA(Ustack, ST->root->left);
		Tgba->SimplifyStates();
		delete ST;
		//cout << endl;
		//11?ìTBA
		TBA *tba;
		tba = new TBA;
		tba->CreatTBA(*Tgba, Ustack);
		delete Tgba;
		string filename = propertyid + ".txt";
		tba->PrintBuchi(filename);
		/*cout << "Please check the file" + filename + ". In this file you can see the Buchi automata related to the LTL formula!";
		cout << endl;*/
		//11?ìSBA
		SBA sba;

		sba.CreatSBA(*tba);
		sba.Simplify();
		sba.Compress();
		delete tba;


		//生成交自动机 并搜索
		Product_Automata *product = new Product_Automata;
		product->ModelChecker(petri, rg, sba, propertyid);
		delete product;
	}
}

void GetLTLC(C_Petri &petri,  TiXmlElement *p)
{
	string value = p->Value();
	if (value == "all-paths")
	{
		GetLTLC(petri,  p->FirstChildElement());
	}
	else if (value == "globally")
	{
//		outltl << "[](";
		GetLTLC(petri, p->FirstChildElement());
//		outltl << ")";
	}
	else if (value == "finally")
	{
//		outltl << "<>(";
		GetLTLC(petri, p->FirstChildElement());
//		outltl << ")";
	}
	else if (value == "next")
	{
//		outltl << "X(";
		GetLTLC(petri, p->FirstChildElement());
//		outltl << ")";
	}
	else if (value == "until")
	{
		TiXmlElement *m, *n;
		m = p->FirstChildElement();
		n = m->NextSiblingElement();
		string mValue = m->Value();
		string nValue = n->Value();
		if (mValue != "before")
		{
			cerr << "Error in XML file! The element until\'s first child is not before!" << endl;
			exit(-1);
		}
		if (nValue != "reach")
		{
			cerr << "Error in XML file! The element until\'s second child is not reach!" << endl;
			exit(-1);
		}
//		outltl << "(";
		GetLTLC(petri, m->FirstChildElement());
//		outltl << ")U(";
		GetLTLC(petri, n->FirstChildElement());
//		outltl << ")";
	}
	else if (value == "integer-le")
	{
		TiXmlElement *m, *n;
		m = p->FirstChildElement();
		n = m->NextSiblingElement();
		string mValue = m->Value();
		string nValue = n->Value();
//		outltl << "{";
		if (mValue == "tokens-count")
		{
//			outltl << "token-count(";
			TiXmlElement *left = m->FirstChildElement();
			while (left != NULL)
			{
				
//				outltl << left->GetText() << ",";
				string temp = left->GetText();
				temp = find_P_name(petri, temp);
				if (temp == "")
					break;
				char variable[50];
				strcpy(variable, temp.c_str());
				left->FirstChild()->SetValue(variable);
				left = left->NextSiblingElement();
			}
//			outltl << ")";
		}
		else if (mValue == "integer-constant")
		{
//			outltl << m->GetText();
		}
		else {
			cerr << "Error in XML file about the element integer-le!" << endl;
			exit(-1);
		}
//		outltl << "<=";
		if (nValue == "tokens-count")
		{
//			outltl << "token-count(";
			TiXmlElement *right = n->FirstChildElement();
			while (right != NULL)
			{
//				outltl << right->GetText() << ",";
				string temp = right->GetText();
				temp = find_P_name(petri, temp);
				if (temp == "")
					break;
				char variable[50];
				strcpy(variable, temp.c_str());
				right->FirstChild()->SetValue(variable);
				right = right->NextSiblingElement();
			}
//			outltl << ")";
		}
		else if (mValue == "integer-constant")
		{
//			outltl << n->GetText();
		}
		else {
			cerr << "Error in XML file about the element integer-le!" << endl;
			exit(-1);
		}
//		outltl << "}";
	}
}

void xml_trans_C(C_Petri petri, char *filename, char *to_file)
{

	//ofstream outltl("FormulaC.txt", ios::out);
	TiXmlDocument *doc = new TiXmlDocument(filename);
	if (!doc->LoadFile())
	{
		cerr << doc->ErrorDesc() << endl;
	}
	TiXmlElement *root = doc->RootElement();
	if (root == NULL)
	{
		cerr << "Failed to load file: no root element!" << endl;
		doc->Clear();
	}
	TiXmlElement *p = root->FirstChildElement();
	while (p != NULL)
	{
		TiXmlElement *id = p->FirstChildElement("id");

		TiXmlElement *formula = p->FirstChildElement("formula");
		GetLTLC(petri, formula->FirstChildElement());

		p = p->NextSiblingElement();
	}

	doc->SaveFile(to_file);
}

