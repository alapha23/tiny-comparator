int main(void)
{
	char **a;
	char *b;
	char c = 'c';
	char arr[10] = "";
	int array[10] = {0};
	int *d;

	b = &c;
	// &c -> addr_expr : type, op 
	// type: pointer type
	// op var
	b = *a;
	// *a -> indirect_ref: type, op
	// type: pointer type
	// op var
	b = arr+3;
	// arr+3 pointer_plus: type, op
	// op-> nop_expr: type, op
	// op-> addr_expr: type, op
	// type-> pointer_type:  ptd: array
	// op var 
	d = array;

	return 0;
}
