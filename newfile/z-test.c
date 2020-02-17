int nondet_num=101;
int func(int a,int b)
{
	return a*b;
}

int main()
{
	int x;
	x=0;
	if(1>0)
	{
		while(x<10)
		{
			func(3,4);
			x=x+1;
		}
	}
	func(1,2);
	return 0;
}