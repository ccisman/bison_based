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
  int y[100];
  int temp_y;
  int ret_y;
  int ret2_y;
  int ret5_y;

  if(1>0){
int i = 0;
while( i < 100){
    x[i] = __VERIFIER_nondet_int();
   i;i=i+1;
}
}
  if(1>0){
int i = 0;
while( i < 100){
    y[i] = __VERIFIER_nondet_short();
   i;i=i+1;
}
}

  ret = bAnd(x);
  ret_y = bor(y);

  temp=x[0];x[0] = x[1]; x[1] = temp;
  temp_y=y[0];y[0] = y[1]; y[1] = temp_y;
  ret2 = bAnd(x);
  ret2_y = bor(y);
  temp=x[0];
  temp_y=y[0];
  if(1>0){
int i =0 ;
while( i<100-1){
     x[i] = x[i+1];
   i;i=i+1;
}
}
  if(1>0){
int i =0 ;
while( i<100-1){
     y[i] = y[i+1];
   i;i=i+1;
}
}
  x[100-1] = temp;

  ret5 = bAnd(x);
  ret5_y = bor(y);

  if(ret != ret2 || ret !=ret5){ 
    __VERIFIER_error();
  }
  
  if(ret_y != ret2_y || ret_y !=ret5_y){ 
    __VERIFIER_error();
  }

  return 1;
}
