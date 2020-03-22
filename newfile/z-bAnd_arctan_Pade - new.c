int nondet_num_double=101;
int nondet_num_int=101;
void __VERIFIER_error()
		{
			return;
		}

/* Example from "Towards an industrial use of FLUCTUAT on safety-critical 
   avionics software" by Delmas, Goubault, Putot, Souryis, Tekkal,
   and Védrine. Published in FMICS 09.

   Padé approximation of arctan.
*/


double __VERIFIER_nondet_double()
		{
			nondet_num_double = nondet_num_double - 1;
			return nondet_num_double;
		}

void __VERIFIER_assume(int expression)
		{
			if (expression == 0)
			{
				__VERIFIER_error();
			}
		}

void __VERIFIER_assert(int cond) { if (!(cond)) {  __VERIFIER_error(); } return; }

int __VERIFIER_nondet_int()
		{
			nondet_num_int = nondet_num_int - 1;
			return nondet_num_int;
		}


const double Pi=3.141592653589793238;

double PADE_2_2(double x) 
{
  double x_2;
  x_2=x*x;
  return 1. - x_2 / (3. + 9./5.*x_2);
}

double ARCTAN_0_1(double x)
{
  return x*PADE_2_2(x);
}

double ARCTAN_POS(double x)
{
  if (x > 1.) return Pi / 2. - ARCTAN_0_1(1. / x);
  else return ARCTAN_0_1(x);
}

double ARCTAN(double x)
{
  if (x < 0) return -ARCTAN_POS(-x);
  else return ARCTAN_POS(x);
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

int main()
{
  double a, r;
  int x[100];
  int temp;
  int ret;
  int ret2;
  int ret5;

  a = __VERIFIER_nondet_double();
  __VERIFIER_assume(a >= -1.79e308 && a <= 1.79e308);

  r = ARCTAN(a);
  
  __VERIFIER_assert(r >= -1.571 && r <= 1.571);
  
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
  

  if(ret != ret2 ){ 
    __VERIFIER_error();
  }
  
  return 0;
}
