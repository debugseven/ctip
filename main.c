#include <stdio.h>
#include "clock.h"

int main(int argc, char* argv[]) {
	ctip_clock c = default_clock();

	// updates
	if (argc < 2) {
		update_local(&c);
	} 
	else {
		update_arg(&c, argv[1]);
	}

	// update error checking
	if (c.error) {
		printf("Wrong input: %s\n", c.message);
		return 1;
	}
	
	printf("The time is %02d:%02d.\n", c.hours, c.minutes);

	// validation
	validate(&c);

	// validation error checking
	if (c.error) {
		printf("Wrong input: %s\n", c.message);
		return 1;
	}

	// calculating the percent and print it to the console
	float p = percent(&c) * 100;
	printf("Time in percent is %.2f%%.\n", p);
	
	return 0;
}