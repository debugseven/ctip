typedef struct {
	int hours;
	int minutes;
	int error;
	char* message;
} ctip_clock;

const static float MINUTES_PER_HOUR = 60 * 24;

enum { no_delimiter = 1, err_hours, err_minutes, no_digit };

typedef struct {
	int err;
	int len_hours;
	int len_minutes;
	int len;
} str_result;

ctip_clock default_clock();
void update_local(ctip_clock* c);
void update_arg(ctip_clock* c, char* str);
float percent(ctip_clock* c);
void validate(ctip_clock* c);