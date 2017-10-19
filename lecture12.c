#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/slab.h>

MODULE_AUTHOR("Denis Suprunenko <chipsoft@gmail.com");
MODULE_DESCRIPTION("Thread demo, Module 10");
MODULE_LICENSE("Dual BSD/GPL");

static u32 ctor_count;

static struct kmem_cache *my_cachep;

void ctor(void *q)
{
	printk(KERN_INFO "Constructor #%d- ok\n", ++ctor_count);
}

static void init_my_cache( void )
{

   my_cachep = kmem_cache_create( 
                  "my_cache",            /* Name */
                  128,                   /* Object Size */
                  0,                     /* Alignment */
                  SLAB_HWCACHE_ALIGN,    /* Flags */
                  ctor );          /* Constructor */
	if (my_cachep) printk(KERN_INFO "kmem_cache_create - ok\n");
   return;
}

int slab_test( void )
{
  void *object;

  printk( "Cache object size is %d\n", kmem_cache_size( my_cachep ) );

  object = kmem_cache_alloc( my_cachep, GFP_KERNEL );

  if (object) {
    printk(KERN_INFO "kmem_cache_alloc - ok\n");
	kmem_cache_free( my_cachep, object );
	printk(KERN_INFO "kmem_cache_free - ok\n");
  }

  return 0;
}

static void remove_my_cache( void )
{

  if (my_cachep) {
	  kmem_cache_destroy( my_cachep );
	  printk(KERN_INFO "remove_my_cache - ok\n");
  }
  return;
}

static int __init hello_init(void)
{
	u32 ctor_count = 0;
	printk(KERN_INFO "-> Module 12 load!\n");
	init_my_cache();
	slab_test();
	remove_my_cache();
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "<- Module 12 unload!\n");
}

module_init(hello_init);
module_exit(hello_exit);
