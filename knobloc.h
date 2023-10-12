#ifndef KNOBLOC_H
#define KNOBLOC_H

#ifndef STATIC_ASSERT
#define STATIC_ASSERT( condition, name )  typedef char assert_failed_ ## name [ (condition) ? 1 : -1 ];
#endif

#ifndef KNL_TYPE
#define KNL_TYPE unsigned char
#endif

struct knl_blk
{
  struct knl_blk *prev;
  struct knl_blk *next;
  struct knl_head *head;
};

struct knl_head
{
  struct knl_blk stub;
  size_t blksiz;
  size_t numblk;
  KNL_TYPE *pool;
  struct knl_blk *frp;
};

STATIC_ASSERT( sizeof(struct knl_blk)>=sizeof(KNL_TYPE), KNL_TYPE_not_large_enough );

#define KNL_ISEMPTY(head) (&(head)->stub!=(head)->frp?0:1)

#define KNL_ALLOC(head) ({ void *r=(head)->frp; \
                           if(&((head)->stub)!=(head)->frp) { \
                             (head)->frp->prev->next=(head)->frp->next; \
                             (head)->frp->next->prev=(head)->frp->prev; \
                             (head)->frp=(head)->frp->next; } \
                             else { r=NULL; } \
                           r; })

#define KNL_FREE(head,p) ({ ((struct knl_blk *)(p))->prev=(head)->frp; \
                            ((struct knl_blk *)(p))->next=(head)->frp->next; \
                                  (head)->frp->next->prev=((struct knl_blk *)(p)); \
                                        (head)->frp->next=((struct knl_blk *)(p)); \
                            if(&((head)->stub)==(head)->frp) { (head)->frp=(head)->frp->next; } \
                                   NULL;     })

#define KNL_IDX2PTR(head,i) ((struct knl_blk *)(&((head)->pool[(head)->blksiz*(i)])))


static int knl_init(struct knl_head *head, KNL_TYPE *pool, size_t numblk, size_t blksiz)
{
  int ret=-1,i;
  struct knl_blk *b;

  if(NULL!=head&&NULL!=pool&&numblk>0&&blksiz>0)
  {
    head->blksiz=blksiz;
    head->numblk=numblk;
    head->pool=pool;
    head->frp=KNL_IDX2PTR(head,0);
    for(i=0;i<numblk;i++)
    {
      b=KNL_IDX2PTR(head,i);
      b->prev=(i>0?KNL_IDX2PTR(head,i-1):NULL);
      b->next=KNL_IDX2PTR(head,i+1);
    }
    head->stub.prev=KNL_IDX2PTR(head,numblk-1);
    head->stub.next=KNL_IDX2PTR(head,0);
    KNL_IDX2PTR(head,0)->prev=&head->stub;
    KNL_IDX2PTR(head,numblk-1)->next=&head->stub;
    ret=0;
  }
  
  return(ret);
}

#endif
