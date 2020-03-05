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

int bor (int x[10])
{
  int i;
  long long res;
  res = x[0];
  if(1>0){
i = 1;
while( i < 10){
    res = res | x[i];
   i;i=i+1;
}
}
  return res;
}

int main()
{
	int x[10];
	int temp;
	int ret;
	int ret2;
	int ret5;

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
	
  if(1>0){
int i = 0;
while( i < 10){
     x[i] = __VERIFIER_nondet_short();
   i;i=i+1;
}
}

  ret = bor(x);

  temp=x[0];x[0] = x[1]; x[1] = temp;
  ret2 = bor(x);
  temp=x[0];
  if(1>0){
int i =0 ;
while( i<10-1){
     x[i] = x[i+1];
   i;i=i+1;
}
}
  x[10-1] = temp;
  ret5 = bor(x);

  if(ret != ret2 || ret !=ret5){ 
    __VERIFIER_error();
  }
	return 1;
}
