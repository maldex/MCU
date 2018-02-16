/* compile as follows: gcc -Wall -lpthread -std=gnu99 -lrt hrt.c -o hrt */

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


typedef void (*closure_t)(void *arg);

struct pwm_conf {
    /* PWM configuration */
    long period_ns;
    long duration_ns;
    float duty_perc;

    /* callbacks */
    closure_t on_raising;
    void *raising_arg;
    closure_t on_falling;
    void *falling_arg;
};

static void timespec_add_ns(struct timespec *ts, long ns) {
    long total_ns = (ts->tv_sec * 1000000000L + ts->tv_nsec) + ns;
    ts->tv_sec = total_ns / 1000000000L;
    ts->tv_nsec = total_ns % 1000000000L;
}

static void *pwm_thread(void *arg) {
    struct pwm_conf *pwm = arg;

    /* calculate duty cycle length */
    long duty_ns = (double) pwm->period_ns * (double) pwm->duty_perc;
    long idle_ns = pwm->period_ns - duty_ns;

    /* calculate number of periods */
    long remaining = pwm->duration_ns / pwm->period_ns;

    /* fetch the initial starting time */
    struct timespec wakeup;
    int rc = clock_gettime(CLOCK_MONOTONIC, &wakeup);
    if (rc != 0) {
        printf("pwm: failed to initalize clock: %s\n", strerror(rc));
        goto cleanup;
    }

    while (remaining--) {
        /* raising edge */
        timespec_add_ns(&wakeup, idle_ns);
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wakeup, NULL);
        pwm->on_raising(pwm->raising_arg);

        /* falling edge */
        timespec_add_ns(&wakeup, duty_ns);
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &wakeup, NULL);
        pwm->on_falling(pwm->falling_arg);
    }

cleanup:
    free(pwm);
    return NULL;
}

int pwm_start_thread(struct pwm_conf conf, pthread_t *ret) {
    assert(conf.duty_perc > 0.0 && conf.duty_perc < 1.0);
    assert(conf.duration_ns > conf.period_ns);
    assert(conf.on_raising != NULL);
    assert(conf.on_falling != NULL);

    /* copy struct into thread */
    int rc = 0;
    pthread_attr_t attr;
    struct pwm_conf *arg = NULL;

    /* we configure the scheduling policy in the attributes */
    rc = pthread_attr_init(&attr);
    if (rc != 0) {
        return rc;
    }

    /* do not inherit the policy from the parent */
    rc = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    if (rc != 0) {
        goto cleanup;
    }

    /* real-time thread with FIFO scheduling policy */
    rc = pthread_attr_setschedpolicy(&attr, SCHED_FIFO);
    if (rc != 0) {
        goto cleanup;
    }

    /* highest priority on Linux */
    struct sched_param param = { 99 }; 
    rc = pthread_attr_setschedparam(&attr, &param);
    if (rc != 0) {
        goto cleanup;
    }

    /* allocate configuration struct for newly created thread */
    arg = malloc(sizeof(struct pwm_conf));
    if (arg == NULL) {
        rc = EAGAIN;
        goto cleanup;
    } else {
        /* copy config over to thread */
        *arg = conf;
    }

    rc = pthread_create(ret, &attr, pwm_thread, (void *)arg);
    if (rc != 0) {
        /* only deallocate configuration if thread was never started */
        free(arg);
    }

cleanup:
    pthread_attr_destroy(&attr);
    return rc;
}

/* CUSTOMIZATIONS AFTER THIS LINE */


static void start_signal(void *arg) {
    FILE *fp = arg;
	pwrite(fileno(fp), "1", 1, 0);
}

static void stop_signal(void *arg) {
    FILE *fp = arg;
	pwrite(fileno(fp), "0", 1, 0);
}

#define MILLIS 1000*1000L

int main(int argc, char *argv[]) {
    struct pwm_conf pwm = { 0 };

	FILE *fp = fopen("/sys/class/gpio/gpio3/value", "w");

    pwm.period_ns = 20 * 1000 * 1000L; /* 1ms */
    pwm.period_ns = 15 * 1000 * 1000L; /* 1ms */

	pwm.duty_perc = 0.1;
	
    pwm.duration_ns = 200000 * MILLIS;

    pwm.on_raising = start_signal;
	pwm.raising_arg = fp;
    pwm.on_falling = stop_signal;
	pwm.falling_arg = fp;
	
    /* thread handle, used to wait for the thread to finish */
    pthread_t pwm_thread;
    int rc = pwm_start_thread(pwm, &pwm_thread);
    if (rc != 0) {
        printf("failed to create pwm thread: %s\n", strerror(rc));
        exit(-1);
    }

    rc = pthread_join(pwm_thread, NULL);
    if (rc != 0) {
        printf("failed to wait for pwm thread: %s\n", strerror(rc));
        exit(-1);
    }
	
	fclose(fp);
}