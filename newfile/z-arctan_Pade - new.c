int nondet_num_double=101;
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

int main()
{
  double a, r;
  double x;

  a = __VERIFIER_nondet_double();
  __VERIFIER_assume(a >= -1.79e308 && a <= 1.79e308);

  r = ARCTAN(a);
  
  x = ARCTAN(a);
  
  __VERIFIER_assert(r >= -1.571 && r <= 1.571);
  return 0;
}