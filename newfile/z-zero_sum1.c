int nondet_num_short=101;
void __VERIFIER_error()
{
	return;
}

extern void __VERIFIER_assume(int);
void __VERIFIER_assert(int cond) { if(!(cond)) {  __VERIFIER_error(); } }
short __VERIFIER_nondet_short()
{
	nondet_num_short = nondet_num_short - 1;
	return nondet_num_short;
}


short SIZE;

int main()
{
	SIZE = __VERIFIER_nondet_short();
	if(SIZE > 1)
	{
		int i;
		short a[100];
		long long sum=0;
		
		if(1>0){
int i = 0;
while( i < SIZE){
		  a[i] = __VERIFIER_nondet_short();
		 i;i=i+1;
}
}

		if(1>0){
i = 0;
while( i < SIZE){
			sum = sum + a[i];
		 i ;i=i+1;
}
}

		if(1>0){
i = 0;
while( i < SIZE){
			sum = sum - a[i];
		 i;i=i+1;
}
}
		__VERIFIER_assert(sum == 0);
	}
	return 1;
}
