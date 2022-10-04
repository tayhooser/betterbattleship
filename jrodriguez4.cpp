//
#include <stdio.h>

int  show_jason() {

	printf("Jason\n");
	return 0;
}

unsigned int show_help(unsigned int toggle) {

	toggle = toggle ^ 1;
	return toggle;

}
