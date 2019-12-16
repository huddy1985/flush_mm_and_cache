#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static double diff_in_second(struct timespec t1, struct timespec t2)
{
        struct timespec diff;
        if (t2.tv_nsec-t1.tv_nsec < 0) {
                diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
                diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
        } else {
                diff.tv_sec  = t2.tv_sec - t1.tv_sec;
                diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
        }
        return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main()
{
	struct timespec req;
	req.tv_sec = 2;
	req.tv_nsec = 1000;

	struct timespec start, end;

	int page_count = 4096;
	char *mem = (char*)malloc(page_count);

	if (!mem) {
		printf("malloc mem failed\n");
		return -1;
	}

	int pc = page_count;
	while (--pc) {
		mem[pc] = (char)pc;
	}

	pc = page_count;
	while(--pc) {
		printf("%c ", mem[pc]);
	}
	puts("\n");

	while (1) {
		int count = 4095;
		char sz = 0;
		clock_gettime(CLOCK_REALTIME, &start);
		while (count--) {
			sz = mem[count];
		}
		clock_gettime(CLOCK_REALTIME, &end);

		printf("spend time is %lf \n", diff_in_second(start, end));
		nanosleep(&req, NULL);
	}
}