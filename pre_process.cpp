#include<sstream>
#include<iostream>
#include<fstream>
#include<regex>
#include<string>
using namespace std;


//pre-process

extern int string_replace(string &s1, const string &s2, const string &s3);

bool for_to_while(string s, string &oldtext, string &newtext)//true代表搜索到，false代表没搜索到，这里s不是引用
{
	regex pattern("for *\\((.*?);(.*?);(.*)\\)");
	smatch result;

	int position;
	int sum_num = 0, sum = 0;
	string text, res1, res2, res3;
	//string newtext, oldtext;

	if (regex_search(s, result, pattern)) {
		position = result.position();
		res1 = result[1];
		res2 = result[2];
		res3 = result[3];
	}
	else
		return false;
	newtext = "if(1>0){\n" + res1 + ';' + "\nwhile(" + res2 + ')';
	for (unsigned int i = position; i < s.length(); i++)
	{
		if (s[i] == '{')
			sum_num++;
		else if (s[i] == '}')
		{
			sum_num--;
			if (sum_num == 0)
				break;
		}
		if (sum_num > 0)
			text = text + s[i];
		sum++;
	}
	//cout << text << endl;
	//cout << sum;
	text = text + res3 + ";}\n}";
	newtext = newtext + text;
	oldtext = s.substr(position, sum + 1);
	return true;
}

void for_to_while_all(string &s)
{
	string oldtext, newtext;
	while (1)
	{
		if (!for_to_while(s, oldtext, newtext))
			break;
		string_replace(s, oldtext, newtext);
	}
}

bool trans_plusplus(string s, string &newtext)//true代表搜索到，false代表没搜索到，这里s是引用
{
	regex pattern("\\+\\+");
	smatch result;

	int position, insert_position;
	int sum_num = 0, sum = 0;
	string text, res1, res2, res3;
	bool following_plus;
	//string newtext, oldtext;

	if (regex_search(s, result, pattern)) {
		position = result.position();
	}
	else
		return false;

	newtext = "";
	if (isalpha(s[position - 1]) || s[position - 1] == '_')
	{
		following_plus = true;
		for (unsigned int i = position - 1; i > 0; i--)
		{
			if (s[i] == '_' || isalpha(s[i]))
				text = s[i] + text;
			else
				break;
			sum++;
		}
	}
	else
	{
		following_plus = false;
		for (unsigned int i = position + 2; i < s.length(); i++)
		{
			if (s[i] == '_' || isalpha(s[i]))
				text = s[i] + text;
			else
				break;
			sum++;
		}
	}
	//cout << text << endl;
	//cout << sum;
	text = text + "=" + text + "+1;\n";
	if (!following_plus)
	{
		for (unsigned int i = position; i >= 0; i--)
			if (s[i] == ';' || s[i] == '}')
			{
				insert_position = i + 1;
				break;
			}
		newtext = s.substr(0, insert_position) + text + s.substr(insert_position, position - insert_position) + s.substr(position + 2);
	}
	else
	{
		for (unsigned int i = position; i < s.length(); i++)
			if (s[i] == ';')
			{
				insert_position = i + 1;
				break;
			}
		newtext = s.substr(0, position) + s.substr(position + 2, insert_position - position - 2) + text + s.substr(insert_position);
	}

	return true;
}

void trans_plusplus_all(string &s)
{
	string newtext;
	while (1)
	{
		if (!trans_plusplus(s, newtext))
			break;
		s = newtext;
	}
}

bool trans_assign(string &s)
{
	vector<string> sign_list;
	sign_list.push_back("+");
	sign_list.push_back("-");
	sign_list.push_back("*");
	sign_list.push_back("/");
	for (unsigned int i = 0; i < sign_list.size(); i++)
	{
		string sign = sign_list[i];
		string p = "([a-zA-Z_]+ *)\\" + sign + "=";
		regex pattern(p);
		smatch result;

		int position;
		int sum_num = 0, sum = 0;
		string text, res1, newtext;

		if (regex_search(s, result, pattern)) {
			position = result.position();
			text = result[0];
			res1 = result[1];
		}
		else
			return false;
		newtext = "=" + res1 + sign;
		text.replace(text.find(sign + "="), 2, newtext);
		s.replace(s.find(result[0]), result[0].length(), text);
	}
}

bool trans_define(string &s)
{
	regex pattern("#define +(.*)? +(.*)?\n");
	smatch result;

	int position;
	int sum_num = 0, sum = 0;
	string text, text1, res1, res2;

	if (regex_search(s, result, pattern)) {
		position = result.position();
		res1 = result[1];
		res2 = result[2];
	}
	else
		return false;

	s = s.replace(s.find(result[0]), result[0].length(), "");

	regex pattern1("[^a-zA-Z_]" + res1 + "[^a-zA-Z_]");
	while (1)
	{
		if (regex_search(s, result, pattern1))
		{
			text = result[0];
			text1 = text;
			text1 = text1.replace(text1.find(res1), res1.length(), res2);
			s = s.replace(s.find(text), text.length(), text1);
		}
		else
			break;
	}
}

void trans_define_all(string &s)
{
	while (1)
	{
		if (trans_define(s) == false)
			return;

	}
}

void trans_some_function(string &s)
{
	string err = "extern void __VERIFIER_error() __attribute__ ((__noreturn__));";
	string err1 = "extern void __VERIFIER_error();";
	string ass = "void __VERIFIER_assert(int cond) { if (!(cond)) { ERROR: __VERIFIER_error(); } return; }";
	string ass1 = "void __VERIFIER_assert(int cond) {\n\
		if (!(cond)) {\n\
		ERROR: __VERIFIER_error();\n\
		}\n\
	return;\n\
}";
	
	string assum = "extern void __VERIFIER_assume(int expression);";

	string newassum="void __VERIFIER_assume(int expression)\n\
{\n\
	if (expression == 0)\n\
	{\n\
		__VERIFIER_error();\n\
	}\n\
}\n";

	string newerr = "void __VERIFIER_error()\n\
{\n\
	return;\n\
}\n";
	string newass = "void __VERIFIER_assert(int cond)\n\
{\n\
	if (cond == 0)\n\
	{\n\
		__VERIFIER_error();\n\
	}\n\
}\n";
	
	string nondet_int = "extern int __VERIFIER_nondet_int();";
	string newnondet_int = "int __VERIFIER_nondet_int()\n\
{\n\
	nondet_num = nondet_num - 1;\n\
	return nondet_num;\n\
}\n";
	string nondet_short = "extern short __VERIFIER_nondet_short(void);";
	string newnondet_short = "short __VERIFIER_nondet_short()\n\
{\n\
	nondet_num = nondet_num - 1;\n\
	return nondet_num;\n\
}\n";
	string nondet_long = "extern long __VERIFIER_nondet_long();";
	string newnondet_long = "long __VERIFIER_nondet_long()\n\
{\n\
	nondet_num = nondet_num - 1;\n\
	return nondet_num;\n\
}\n";
	string nondet_double = "extern double __VERIFIER_nondet_double();";
	string newnondet_double = "double __VERIFIER_nondet_double()\n\
{\n\
	nondet_num = nondet_num - 1;\n\
	return nondet_num;\n\
}\n";
	string_replace(s, err, newerr);
	string_replace(s, err1, newerr);
	//string_replace(s, ass, newass);
	//string_replace(s, ass1, newass);
	string_replace(s, "ERROR:", "");
	string_replace(s, assum, newassum);
	string_replace(s, nondet_int, newnondet_int);
	string_replace(s, nondet_short, newnondet_short);
	string_replace(s, nondet_double, newnondet_double);
	if (s.find("int nondet_num") == string::npos)
		s = "int nondet_num=101;\n" + s;
}

void pre_process(string &s)
{
	
	trans_define_all(s);
	for_to_while_all(s);
	trans_plusplus_all(s);
	trans_some_function(s);
	trans_assign(s);
	//cout << s << endl;
}