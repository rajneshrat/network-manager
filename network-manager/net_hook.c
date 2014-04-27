#include <linux/netfilter.h>
#include <linux/skbuff.h>

static struct nf_hook_ops netfilter_ops_in;/* IP PRE ROUTING */

unsigned int main_hook(unsigned int hooknum,
                       const struct sk_buff *skb,
                       const struct net_device *in,
                       const struct net_device *out,
                       int(*okfn)(struct sk_buff*))
{
    //  struct icmphdr* icmp;
//    if(in != NULL) {
    //      printk("in is NULL = %d\n", in->ifindex);

    //  }
    if((skb->priority == 0) ) { // && (tcp_header->ece !=1)){ //||  dport == 8080 || tcp_header->dest == 21){
        //      my_skb = skb_get ( skb);//, GFP_ATOMIC);
//       my_skb->priority = 1;
//        InsertPacketInQueue(my_skb);
        //      return NF_DROP;
    }
    return NF_ACCEPT;
}


int init_net_hook(void)
{
    netfilter_ops_in.hook       = main_hook;
    netfilter_ops_in.pf         = PF_INET;
    netfilter_ops_in.hooknum    = NF_INET_PRE_ROUTING; /*NF_INET_PRE_ROUTING;*/
    netfilter_ops_in.priority   = 0;//NF_IP_PRI_FIRST;

    nf_register_hook(&netfilter_ops_in);


    printk(KERN_INFO "qos initialized called\n");
    return 0;
}

void cleanup_net_hook(void)
{
    printk(KERN_INFO "ending qos\n");
    nf_unregister_hook(&netfilter_ops_in);
}

