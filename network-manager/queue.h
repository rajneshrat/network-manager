#include "packet.h"

typedef struct
{
    packet pkt;
    void   *skb;
} node;

typedef struct
{
    node *head;
    node *tail;
    int  total_nodes;
} queue;

