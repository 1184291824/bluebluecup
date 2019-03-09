#ifndef __TIMER_H__
#define __TIMER_H__


void Timer0Init(void);
int get_t1(void);
void set_t1(u16 x);
void start_timer0(void);
void stop_timer0(void);

#endif