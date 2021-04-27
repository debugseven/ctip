#include "clock.h"
#include <time.h>
#include <errno.h>
#include <string.h>

// returns a default clock struct
ctip_clock default_clock() {
	ctip_clock c;
	c.error = 0;
	c.hours = 0;
	c.minutes = 0;
	c.message = "";
	return c;
}

// update information in the clock to the local system time 
void update_local(ctip_clock* c) {
	time_t now = time(NULL);
	struct tm tm;
	errno_t err = localtime_s(&tm, &now);
	if (err) {
		c->error = 1;
		c->message = "System time not available.";
	} 
	else {
		c->hours = tm.tm_hour;
		c->minutes = tm.tm_min;
	}
}

// returns 0 if str has : else return the lenght of the string
str_result str_has_delimiter(char* str) {
	str_result res;
	res.err = no_delimiter;
	res.len_hours = 0;
	res.len_minutes = 0;
	res.len = strlen(str);

	int toggle = 0;

	for (int i = 0; i < res.len; i++) {
		if (str[i] == ':') {
			res.err = 0;
			toggle = 1;
		}
		else if (!toggle) {
			res.len_hours++;
		} 
		else {
			res.len_minutes++;
		}

		// check the ascii bounds of digit (exept :)
		if (str[i] < 48 || 58 < str[i]) {
			res.err = no_digit;
		}
	}

	if (res.len_hours < 1 || 2 < res.len_hours) {
		res.err = err_hours;
	} 
	else if (res.len_minutes < 1 || 2 < res.len_minutes) {
		res.err = err_minutes;
	}

	return res;
}

void handle_res_error(ctip_clock* c, int err) {
	switch (err) {
	case no_delimiter:
		c->message = "String has no delimiter ':'";
		break;
	case err_hours:
		c->message = "Hours have wrong format.";
		break;
	case err_minutes:
		c->message = "Minutes have wrong format.";
		break;
	case no_digit:
		c->message = "A non digit character in time.";
		break;
	}
}


void update_arg(ctip_clock* c, char* str) {
	str_result res = str_has_delimiter(str);
	char hours[] = "00";
	char minutes[] = "00";
	int index = 0;

	if (res.err) {
		handle_res_error(c, res.err);
		c->error = 1;
		return;
	}

	for (int i = 0; i < res.len; i++) {
		// handle hours
		if (i < res.len_hours) {
			if (res.len_hours == 1) {
				hours[1] = str[i];
			}
			else {
				hours[index] = str[i];
				index++;
			}
		}
		// handle :
		else if (i == res.len_hours) {
			index = 0;
		}
		// handle minutes
		else {
			if (res.len_minutes == 1) {
				minutes[1] = str[i];
			}
			else {
				minutes[index] = str[i];
				index++;
			}
		}

	}

	c->hours = atoi(hours);
	c->minutes = atoi(minutes);
}

float percent(ctip_clock* c) {
	float minutes = (c->hours * 60) + c->minutes;
	return minutes / MINUTES_PER_HOUR;
}

void validate(ctip_clock* c) {
	int h = 0 <= c->hours || c->hours < 24;
	int m = 0 <= c->minutes || c->minutes < 60;
	int v = h && m;

	if (!v) {
		c->error = 1;
		c->message = "Out of bounds.";
	} 
}