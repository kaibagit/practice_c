#include <stdio.h>
#include <stdlib.h>

int main()
{
	int nurse = 22;
	int * ptr = &nurse;
	int val = * ptr;
	printf("%d",val);
}
