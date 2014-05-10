/* This file provide the dynamic memory from the shared region. */
#include <stddef.h>
# include <linux/kernel.h>
#include <linux/slab.h>
#define MAX_PAGES 10
#define L_PAGE_SIZE 1024

char *pages[MAX_PAGES];
char *free_page[MAX_PAGES];
#define PAGES_PER_SLAB 1 
#define SLABS_PER_CACHE 5
struct slab_t;

struct obj_t
{
//    int obj_id;
    struct slab_t *slab_owner;
    struct obj_t *next;
};

typedef struct obj_t obj;

struct slab_t {
//    int slab_;
    int total_free_objs;
    struct slab_t *next;
    obj *free_obj_list;
};
typedef struct slab_t slab;

typedef struct {
 //   char name[MAX_CACHE_NAME_LENGTH];
    int obj_length;
    slab *slab_list;
} lcache;

char *
get_free_lpage(void)
{
    int i;
    for(i=0;i<MAX_PAGES;i++) {
        if(free_page[i] == NULL) {
            free_page[i] = pages[i];
            printk("allocating page for index %d\n",i);
            return pages[i];
        }
    }
    printk("No kernel pages available\n");
    return NULL;
}

int
release_lpage(char *page)
{
    int i;
    for(i=0;i<MAX_PAGES;i++) {
        if(free_page[i] == page) {
            free_page[i] = NULL;
            return 0;
        }
    }
    return -1;
}

obj *
fill_slab(int obj_len, int slab_id, struct slab_t *slab_owner)
{
    int objs_per_slab = (L_PAGE_SIZE * PAGES_PER_SLAB) / (obj_len + sizeof(struct obj_t));
    char *page = (char *)get_free_lpage();
    obj *head = (obj *) page;
    int i;
    obj *slab_obj;
    obj *pre = NULL;
    for(i=1; i <= objs_per_slab; i++) {
        slab_obj = (obj *) page;
        slab_obj->slab_owner = slab_owner;
        slab_obj->next = NULL;
        if(pre) {
            pre->next = slab_obj;
        }
        pre = slab_obj;
        page = page + obj_len + sizeof(obj);
    }
    return (obj *)head;
}

lcache *
lcache_create(char *cache_name, int obj_len)
{
    slab *last = NULL;
    int i;

    lcache *cache_obj =  kmalloc(sizeof(lcache), GFP_KERNEL);
    if(!cache_obj) {
        return NULL;
    }
    cache_obj->obj_length = obj_len;
    for(i=1 ;i <= SLABS_PER_CACHE; i++) {
        slab *slab_obj = kmalloc(sizeof(slab), GFP_KERNEL);
        if(i == 1) {
            cache_obj->slab_list = slab_obj;
        }
        else {
            last->next = slab_obj;
        }
        //slab_obj->slab_id = i;
        slab_obj->next = NULL;
        slab_obj->free_obj_list = fill_slab(obj_len, i, slab_obj);
        last = slab_obj;
    }
    return cache_obj;
}

int
lcache_destroy(lcache *cache_obj) 
{
    int total = 0;
    int objs_per_slab;

    slab *slab_obj = cache_obj->slab_list;
    slab *ptr = NULL;
    while(slab_obj) {
        total++;
        slab_obj = slab_obj->next;
    } 
    if(total != SLABS_PER_CACHE) {
        printk("Failed to destroy cache."
               "Still slab to this cache are yet to be freed\n"); 
        return -1;
    } 
    slab_obj = cache_obj->slab_list;
    objs_per_slab = (L_PAGE_SIZE * PAGES_PER_SLAB) / (cache_obj->obj_length + sizeof(struct obj_t));
    while(slab_obj) {
        if(slab_obj->total_free_objs != objs_per_slab) {
            printk("Failed to destroy cache"
                   "corruption in lcache obj\n");
            return -1;
        }
        release_lpage((char *)slab_obj->free_obj_list);
        ptr = slab_obj;
        slab_obj = slab_obj->next;
        kfree(ptr);
        total++;
    }
    return 0;
}        

void *
lcache_alloc(lcache *cache_obj)
{
    slab *slab_obj = cache_obj->slab_list;
    void *obj = (void *)slab_obj->free_obj_list;
    slab_obj->total_free_objs--;
// check if this was the last free obj.
//    free_obj_list = free_obj_list->next;
    if(slab_obj->total_free_objs == 0) {
        cache_obj->slab_list = slab_obj->next;
        if(!cache_obj->slab_list) {
      //      cache_obj->slab_list = cache_obj->slab_list;
        //    if(!cache_obj->slab_list) {
                printk("no more space\n");
            }
            
      //  }
    }
    return obj + sizeof(struct obj_t);
} 

void
lcache_free(lcache *cache_obj, char *ptr)
{
    obj *obj = (struct obj_t *)(ptr - sizeof(struct obj_t));
    slab *slab_obj = obj->slab_owner;
    obj->next = slab_obj->free_obj_list;
    slab_obj->free_obj_list = obj;
    slab_obj->total_free_objs++;
/*    if(slab_obj->total_free_objs == cache_obj->max_objs_per_slab) {

        if(cache_obj->slab_list->pre == NULL) {
            cache_obj->slab_list = cache_obj->slab_list->next;
        }
        cache_obj->slab_list->pre = slab_obj;
        slab_obj->next = cache_obj->slab_list;
        slab_obj->pre = NULL;
        cache_obj->slab_list = slab_obj;
    }    
*/
}
