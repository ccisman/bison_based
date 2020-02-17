int nondet_num=0;
int nondet_num_y=0;



int bAnd (int x[10])
{
  int i;
  long long res;
  res = x[0];
  if(1>0){i = 1;
  while( i < 10){
    res = res & x[i];
   i;
 i= i+1;}}
  return res;
}

int bor (int x[10])
{
  int i;
  long long res;
  res = x[0];
  if(1>0){i = 1;
  while( i < 10){
    res = res | x[i];
   i;
 i= i+1;}}
  return res;
}

int __VERIFIER_nondet_int()
{
    nondet_num = nondet_num + 1;
    return nondet_num;
}
    
int __VERIFIER_nondet_short()
{
    nondet_num_y = nondet_num_y + 1;
    return nondet_num_y;
}
    
void __VERIFIER_error()
{
    return;
}
    
int main ()
{
  int x[10];
  int y[10];
  int temp;
  int temp_y;
  int ret;
  int ret_y;
  int ret2;
  int ret2_y;
  int ret5;

  if(1>0){int i = 0;
  while( i < 10){
    x[i] = __VERIFIER_nondet_int();
   i;
 i= i+1;}}
  if(1>0){int j = 0;
  while( j < 10){
    y[j] = __VERIFIER_nondet_short();
   j;
 j= j+1;}}

  ret = bAnd(x);
  ret_y = bor(y);
  temp=x[0];x[0] = x[1]; x[1] = temp;
  temp_y=y[0];y[0]=y[1]; y[1] = temp_y;
  ret2 = bAnd(x);
  ret2_y = bor(y);
  temp=x[0];
  temp_y = y[0];
  if(1>0){int i = 0 ;
  while( i<10-1){
     x[i] = x[i+1];
   i;
 i= i+1;}}
  x[10-1] = temp;
  if(1>0){int j = 0 ;
  while( j<10-1){
     y[j] = y[j+1];
   j;
 j= j+1;}}
  y[10-1] = temp_y;

  if(ret != ret2){ 
    __VERIFIER_error();
  }
  return 1;
}
