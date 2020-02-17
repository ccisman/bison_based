void __VERIFIER_error()
{
	return;
}

/* Rounding addition and subtraction in double-precision floats. */

void __VERIFIER_assert(int cond) { if (!(cond)) {  __VERIFIER_error(); } return; }

int main()
{
  double x, y, z, r;

  x = 1e9;
  y = x + 1;
  z = x - 1;
  r = y - z;  
  __VERIFIER_assert(r == 2.);
  return 0;
}
