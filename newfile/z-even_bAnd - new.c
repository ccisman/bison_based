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

short __VERIFIER_nondet_short()
{
	nondet_num_short = nondet_num_short - 1;
	return nondet_num_short;
}


int bAnd (int x[100])
{
  int i;
  long long res;
  res = x[0];
  if(1>0){
i = 1;
while( i < 100){
    res = res & x[i];
   i;i=i+1;
}
}
  return res;
}

void __VERIFIER_assert(int cond) {
  if (!(cond)) {
     __VERIFIER_error();
  }
  return;
}
int main(void) {
  int x[100];
  int temp;
  int ret;
  int ret2;
  int ret5;
  
  unsigned int y = 0;
  while (__VERIFIER_nondet_short()) {
    y =y + 3;
  }
  __VERIFIER_assert(!(x % 2));
  
  if(1>0){
int i = 0;
while( i < 100){
    x[i] = __VERIFIER_nondet_int();
   i;i=i+1;
}
}

  ret = bAnd(x);

  temp=x[0];x[0] = x[1]; x[1] = temp;
  ret2 = bAnd(x);
  temp=x[0];
  if(1>0){
int i =0 ;
while( i<100-1){
     x[i] = x[i+1];
   i;i=i+1;
}
}
  x[100-1] = temp;
  ret5 = bAnd(x);

  if(ret != ret2 || ret !=ret5){ 
    __VERIFIER_error();
  }

  return 0;
}
