#include <stdio.h>

void hello()
{
	printf("hello world!\n");
}

int main( int argc, char** argv)
{
	int a, b;
	a = 1;
	b = 2;
	if( a < b)
	{
		hello();
	}
}


