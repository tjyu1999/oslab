#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <stdlib.h>

#define HZ	    100
#define NR_PROC   5

void cpuio_bound(int last, int cpu_time, int io_time);

int main(int argc, char *argv[]) {
	pid_t pid[NR_PROC];

    for (int i = 0; i < NR_PROC; ++i) {
        pid_t cur_pid = fork();
        
        if (cur_pid == 0) {
			cpuio_bound(2 * (i + 1), 0, 1);
			printf("the pid is %d, the father pid is %d.\n", getpid(), getppid());
			return 0;
        }
        else {
			pid[i] = cur_pid;
			printf("the %d child pid is %d\n", i, pid[i]);
		}
    }
    
	printf("the parent is finished.\n");
	return 0;
}

void cpuio_bound(int last, int cpu_time, int io_time) {
	struct tms start_time, current_time;
	clock_t utime, stime;
	int sleep_time;

	while (last > 0) {
		/* CPU Burst */
		times(&start_time);
        
		do {
			times(&current_time);
			utime = current_time.tms_utime - start_time.tms_utime;
			stime = current_time.tms_stime - start_time.tms_stime;
		} while (((utime + stime) / HZ )  < cpu_time );
		
        last -= cpu_time;
		if (last <= 0 )
			break;

		/* IO Burst */
		sleep_time=0;
		while (sleep_time < io_time) {
			sleep(1);
			++sleep_time;
		}
        
		last -= sleep_time;
	}
}
