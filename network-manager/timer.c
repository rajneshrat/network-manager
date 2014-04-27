#ifndef __bandwidthsetter__

#define __bandwidthsetter__

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/list.h>
#include <linux/netfilter_ipv4.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <linux/icmp.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>

#include <linux/skbuff.h>
#include <linux/string.h>
#include <linux/inet.h>

#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/sched.h>

MODULE_LICENSE("GPL");

struct sk_buff *my_skb;

int sent = 0;
static struct timer_list my_timer;

static struct iphdr *ip_header;

static struct tcphdr *tcp_header;

unsigned int sport ,
         dport;
uint ban = 10000;
uint  TotalTime = 0;
uint MinuteTimer = 0;
uint TotalSize = 0;

void SetBandwidthKBps(int MaxKBps)
{
    printk("Setting MaxKBps = %d\n",MaxKBps);
    ban = MaxKBps;
}
static overlaps = 0;
static enum hrtimer_restart function_timer(struct hrtimer *);
static struct hrtimer htimer, htimermin;
static ktime_t kt_periode, kt_periodemin;

static enum hrtimer_restart minute_timer(struct hrtimer * unused)
{
    printk("One sec hit\n");
    kt_periodemin = ktime_set(1, 000l); //seconds,nanoseconds
    hrtimer_forward_now(& htimermin, kt_periodemin);
    return HRTIMER_RESTART;
}

void intialize_bsetter(void)
{
    int ret;
    kt_periodemin = ktime_set(1, 000l); //seconds,nanoseconds
    hrtimer_init (& htimermin, CLOCK_REALTIME, HRTIMER_MODE_REL);
    htimermin.function = minute_timer;
    hrtimer_start(& htimermin, kt_periodemin, HRTIMER_MODE_REL);
}

void cleanup_bsetter()
{
    int ret;

    ret = hrtimer_cancel( &htimermin );
    if (ret) printk("The timer was still in use...\n");

    printk("HR Timer module uninstalling\n");

    return;
}

#endif
