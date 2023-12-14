
/*
 * Copyright (c) 2017, GlobalLogic Ukraine LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the GlobalLogic.
 * 4. Neither the name of the GlobalLogic nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY GLOBALLOGIC UKRAINE LLC ``AS IS`` AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL GLOBALLOGIC UKRAINE LLC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include "hello41.h"

MODULE_AUTHOR("Oleksii Chyzhov <chyzhov.oleksii@lll.kpi.ua>");
MODULE_DESCRIPTION("'Hello, world' module, Lab 4, part 1");
MODULE_LICENSE("Dual BSD/GPL");

EXPORT_SYMBOL(print_hello);

struct my_list_entry {
	struct list_head list_node;
	ktime_t timestamp_before;
	ktime_t timestamp_after;
};

static LIST_HEAD(my_list_head);

void print_hello(void)
{
	struct my_list_entry *ptr = kmalloc(sizeof(*ptr), GFP_KERNEL);

	INIT_LIST_HEAD(&ptr->list_node);
	ptr->timestamp_before = ktime_get();
	pr_info("Hello, world!\n");
	ptr->timestamp_after = ktime_get();
	list_add(&ptr->list_node, &my_list_head);
}

static int __init hello_init(void)
{
	pr_info("hello41 module initialized.\n");
	return 0;
}

static void __exit hello_exit(void)
{
	struct my_list_entry *ptr;
	struct my_list_entry *tmp;


	list_for_each_entry_safe(ptr, tmp, &my_list_head, list_node) {
		pr_emerg("Event timestamp: %lld microseconds\n", ktime_to_us(ptr->timestamp_after - ptr->timestamp_before));
		list_del(&ptr->list_node);
		kfree(ptr);
	}
	pr_info("Exiting hello41 module.\n");
}

module_init(hello_init);
module_exit(hello_exit);

