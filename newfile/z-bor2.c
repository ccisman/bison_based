int nondet_num_short=101;
int nondet_num_int=101;

void __VERIFIER_error()
{
	return;
}

int __VERIFIER_nondet_int()
{
	nondet_num_int = nondet_num_int - 1;
	return nondet_num_int;
}


int bor (int x[100])
{
  int i;
  long long res;
  res = x[0];
  if(1>0){
i = 1;
while( i < 100){
    res = res | x[i];
   i;i=i+1;
}
}
  return res;
}

int main ()
{
  int x[100];
  int temp;
  int ret;
  int ret2;
  int ret5;

  if(1>0){
int i = 0;
while( i < 100){
    x[i] = __VERIFIER_nondet_int();
   i;i=i+1;
}
}

  ret = bor(x);

  temp=x[0];x[0] = x[1]; x[1] = temp;
  ret2 = bor(x);
  temp=x[0];
  if(1>0){
int i =0 ;
while( i<100-1){
     x[i] = x[i+1];
   i;i=i+1;
}
}
  x[100-1] = temp;
  ret5 = bor(x);

  if(ret != ret2 || ret !=ret5){ 
    __VERIFIER_error();
  }
  return 1;
}
