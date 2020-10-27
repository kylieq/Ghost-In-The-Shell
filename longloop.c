#include <stdio.h>
int main(int argc, char *argv[]) {
	int x = 1;
	while(x < 1000000){
		x+=2;
		printf("x: %d\n", x);
	}
}
