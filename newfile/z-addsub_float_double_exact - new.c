int nondet_num_short=101;
void __VERIFIER_error()
{
	return;
}

/* Rounding addition and subtraction in single-precision floats. */

void __VERIFIER_assert(int cond) { if (!(cond)) {  __VERIFIER_error(); } return; }

int main()
{
  float x, y, z, r;
  double x1, y1, z1, r1;
  x = 1e7f;
  y = x + 1.f;
  z = x - 1.f;
  r = y + z;
  __VERIFIER_assert(r == 2.f);
  
  x1 = 1e8;
  y1 = x1 + 1;
  z1 = x1 - 1;
  r1 = y1 - z1;  
  __VERIFIER_assert(r1 == 2.);
  
  return 0;
}
