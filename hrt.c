/* thx to gandro @ https://gist.github.com/gandro/95fa51fb2263891c56926595a90190db */
/* compile as follows: gcc -Wall -lpthread -lrt -D_GNU_SOURCE hrt.c -o hrt */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

struct timer_conf;
typedef void (*timer_callback)(struct timer_conf *timer);

/**
 * configuration struct passed down to the timer thread (and the callback)
 */
struct timer_conf {
    // timer interval in nanoseconds
    long tick_ns;
    // callback, timer is terminated if NULL
    timer_callback callback;
};

/** 
 * the timer thread loop, this is started in a new thread.
 * takes ownership overthe passed timer_conf
 */
void *timer_loop(void *arg) {
    struct timer_conf *timer = arg;
    struct timespec ts;

    int rc = clock_getres(CLOCK_MONOTONIC, &ts);
    assert_perror(rc); /* should only fail if user does not have permission */
    assert(ts.tv_sec == 0); /* resolution should not be bigger than 1 sec */

    printf("Timer thread started. System resolution: %ld ns\n", ts.tv_nsec);

    /* read the initial time */
    rc = clock_gettime(CLOCK_MONOTONIC, &ts);
    assert(rc == 0);
    while (timer->callback != NULL) {
        /* set next wakeup time to current time + tick. for this to work, we
         * need to convert the timespec struct to a nanosecond timestamp and
         * back again.
         */
        long next_tick = (ts.tv_sec * 1000000000L + ts.tv_nsec) + timer->tick_ns;
        ts.tv_sec = next_tick / 1000000000L;
        ts.tv_nsec = next_tick % 1000000000L;

        /* sleep until "next_tick" happens */
        rc = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
        assert_perror(rc);
        
        /* call the user function */
        timer->callback(timer);
    }

    free(timer);
    return NULL;
}

/**
 * the callback called for each tick. is allowed to modify the timer struct
 */
static void on_tick(struct timer_conf *timer) {
    static int count_down = 10;
    printf("Tick! Countdown: %d\n", count_down);

    /* print current time, just as a test.
     * IMPORTANT: This operation can range from 50 to 200ns
     * https://blog.stalkr.net/2010/03/nanosecond-time-measurement-with.htm
     */
    struct timespec ts;
    int rc = clock_gettime(CLOCK_REALTIME, &ts);
    assert_perror(rc);
    printf("Current Unix Time: %ld.%09ld\n", ts.tv_sec, ts.tv_nsec);

    if (count_down == 0) {
        /* disable timer */
        timer->callback = NULL;
    } else {
        count_down--;
    }
}



 int main (int argc, char *argv[]) {
    /* a config struct for the timer itself */
    struct timer_conf *timer = calloc(1, sizeof(struct timer_conf));
    timer->tick_ns = 100 * 1000000L; /* set tick to 100ms */
    timer->callback = on_tick; /* this function is called on each tick */

    /* thread handle, used to wait for the thread to finish */
    pthread_t timer_thr;
    
    int rc = pthread_create(&timer_thr, NULL, timer_loop, (void *)timer);
    assert_perror(rc);

    /* do other stuff here */
    
    /* wait for timer to finish (and ignore its return value) */
    rc = pthread_join(timer_thr, NULL);
    assert_perror(rc);

    /* Last thing that main() should do */
    pthread_exit(NULL);
 }