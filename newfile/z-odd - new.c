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

void __VERIFIER_assert(int cond) {
  if (!(cond)) {
     __VERIFIER_error();
  }
  return;
}
int main(void) {
  unsigned int x = 1;
  while (__VERIFIER_nondet_int()) {
    x =x + 3;
  }
  __VERIFIER_assert(x % 2);
  return 0;
}
