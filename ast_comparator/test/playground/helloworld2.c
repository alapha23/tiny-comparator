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
  b = a;
  d = 55519 % a;
  f = 77719;

  if( a == 0)
  { 
	  a++;
	  b++;
	  add( f / b, b * 2);
  }

  int *c;
  *c = 1;
  *(c+1) = 2;
//  char* c_a = "abc";
//  char c_b;


  return 0;
}
