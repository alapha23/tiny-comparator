int add(int n, int a)
{
	return n-1-a;
}


int main(void)
{
  int a;
  int b;
  int d;
  int f;

  a = 22419;
  ++a;
  b = a*a;
  d = 55519 % a;
  f = 77719;

  if( a == 0)
  { 
	  a++;
	  b++;
	  add( f / b, b * 2);
  }

  int *c;
  a = 1;
  *c = 1;
  *(c+1+a) = 3;
  char* c_a = "abc";
  printf("test: %d fdfdsa\n", a);
//  char c_b;


  return 0;
}
