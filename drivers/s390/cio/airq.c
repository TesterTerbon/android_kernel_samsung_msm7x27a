/*
<<<<<<< HEAD
 *  drivers/s390/cio/airq.c
 *    Support for adapter interruptions
 *
 *    Copyright IBM Corp. 1999,2007
=======
 *    Support for adapter interruptions
 *
 *    Copyright IBM Corp. 1999, 2007
>>>>>>> refs/remotes/origin/master
 *    Author(s): Ingo Adlung <adlung@de.ibm.com>
 *		 Cornelia Huck <cornelia.huck@de.ibm.com>
 *		 Arnd Bergmann <arndb@de.ibm.com>
 *		 Peter Oberparleiter <peter.oberparleiter@de.ibm.com>
 */

#include <linux/init.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/rcupdate.h>
=======
#include <linux/irq.h>
#include <linux/kernel_stat.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/rculist.h>
#include <linux/slab.h>
>>>>>>> refs/remotes/origin/master

#include <asm/airq.h>
#include <asm/isc.h>

#include "cio.h"
#include "cio_debug.h"
<<<<<<< HEAD

#define NR_AIRQS		32
#define NR_AIRQS_PER_WORD	sizeof(unsigned long)
#define NR_AIRQ_WORDS		(NR_AIRQS / NR_AIRQS_PER_WORD)

union indicator_t {
	unsigned long word[NR_AIRQ_WORDS];
	unsigned char byte[NR_AIRQS];
} __attribute__((packed));

struct airq_t {
	adapter_int_handler_t handler;
	void *drv_data;
};

static union indicator_t indicators[MAX_ISC+1];
static struct airq_t *airqs[MAX_ISC+1][NR_AIRQS];

static int register_airq(struct airq_t *airq, u8 isc)
{
	int i;

	for (i = 0; i < NR_AIRQS; i++)
		if (!cmpxchg(&airqs[isc][i], NULL, airq))
			return i;
	return -ENOMEM;
}

/**
 * s390_register_adapter_interrupt() - register adapter interrupt handler
 * @handler: adapter handler to be registered
 * @drv_data: driver data passed with each call to the handler
 * @isc: isc for which the handler should be called
 *
 * Returns:
 *  Pointer to the indicator to be used on success
 *  ERR_PTR() if registration failed
 */
void *s390_register_adapter_interrupt(adapter_int_handler_t handler,
				      void *drv_data, u8 isc)
{
	struct airq_t *airq;
	char dbf_txt[16];
	int ret;

	if (isc > MAX_ISC)
		return ERR_PTR(-EINVAL);
	airq = kmalloc(sizeof(struct airq_t), GFP_KERNEL);
	if (!airq) {
		ret = -ENOMEM;
		goto out;
	}
	airq->handler = handler;
	airq->drv_data = drv_data;

	ret = register_airq(airq, isc);
out:
	snprintf(dbf_txt, sizeof(dbf_txt), "rairq:%d", ret);
	CIO_TRACE_EVENT(4, dbf_txt);
	if (ret < 0) {
		kfree(airq);
		return ERR_PTR(ret);
	} else
		return &indicators[isc].byte[ret];
}
EXPORT_SYMBOL(s390_register_adapter_interrupt);

/**
 * s390_unregister_adapter_interrupt - unregister adapter interrupt handler
 * @ind: indicator for which the handler is to be unregistered
 * @isc: interruption subclass
 */
void s390_unregister_adapter_interrupt(void *ind, u8 isc)
{
	struct airq_t *airq;
	char dbf_txt[16];
	int i;

	i = (int) ((addr_t) ind) - ((addr_t) &indicators[isc].byte[0]);
	snprintf(dbf_txt, sizeof(dbf_txt), "urairq:%d", i);
	CIO_TRACE_EVENT(4, dbf_txt);
	indicators[isc].byte[i] = 0;
	airq = xchg(&airqs[isc][i], NULL);
	/*
	 * Allow interrupts to complete. This will ensure that the airq handle
	 * is no longer referenced by any interrupt handler.
	 */
	synchronize_sched();
	kfree(airq);
}
EXPORT_SYMBOL(s390_unregister_adapter_interrupt);

#define INDICATOR_MASK	(0xffUL << ((NR_AIRQS_PER_WORD - 1) * 8))

void do_adapter_IO(u8 isc)
{
	int w;
	int i;
	unsigned long word;
	struct airq_t *airq;

	/*
	 * Access indicator array in word-sized chunks to minimize storage
	 * fetch operations.
	 */
	for (w = 0; w < NR_AIRQ_WORDS; w++) {
		word = indicators[isc].word[w];
		i = w * NR_AIRQS_PER_WORD;
		/*
		 * Check bytes within word for active indicators.
		 */
		while (word) {
			if (word & INDICATOR_MASK) {
				airq = airqs[isc][i];
				/* Make sure gcc reads from airqs only once. */
				barrier();
				if (likely(airq))
					airq->handler(&indicators[isc].byte[i],
						      airq->drv_data);
				else
					/*
					 * Reset ill-behaved indicator.
					 */
					indicators[isc].byte[i] = 0;
			}
			word <<= 8;
			i++;
		}
	}
}
=======
#include "ioasm.h"

static DEFINE_SPINLOCK(airq_lists_lock);
static struct hlist_head airq_lists[MAX_ISC+1];

/**
 * register_adapter_interrupt() - register adapter interrupt handler
 * @airq: pointer to adapter interrupt descriptor
 *
 * Returns 0 on success, or -EINVAL.
 */
int register_adapter_interrupt(struct airq_struct *airq)
{
	char dbf_txt[32];

	if (!airq->handler || airq->isc > MAX_ISC)
		return -EINVAL;
	if (!airq->lsi_ptr) {
		airq->lsi_ptr = kzalloc(1, GFP_KERNEL);
		if (!airq->lsi_ptr)
			return -ENOMEM;
		airq->flags |= AIRQ_PTR_ALLOCATED;
	}
	if (!airq->lsi_mask)
		airq->lsi_mask = 0xff;
	snprintf(dbf_txt, sizeof(dbf_txt), "rairq:%p", airq);
	CIO_TRACE_EVENT(4, dbf_txt);
	isc_register(airq->isc);
	spin_lock(&airq_lists_lock);
	hlist_add_head_rcu(&airq->list, &airq_lists[airq->isc]);
	spin_unlock(&airq_lists_lock);
	return 0;
}
EXPORT_SYMBOL(register_adapter_interrupt);

/**
 * unregister_adapter_interrupt - unregister adapter interrupt handler
 * @airq: pointer to adapter interrupt descriptor
 */
void unregister_adapter_interrupt(struct airq_struct *airq)
{
	char dbf_txt[32];

	if (hlist_unhashed(&airq->list))
		return;
	snprintf(dbf_txt, sizeof(dbf_txt), "urairq:%p", airq);
	CIO_TRACE_EVENT(4, dbf_txt);
	spin_lock(&airq_lists_lock);
	hlist_del_rcu(&airq->list);
	spin_unlock(&airq_lists_lock);
	synchronize_rcu();
	isc_unregister(airq->isc);
	if (airq->flags & AIRQ_PTR_ALLOCATED) {
		kfree(airq->lsi_ptr);
		airq->lsi_ptr = NULL;
		airq->flags &= ~AIRQ_PTR_ALLOCATED;
	}
}
EXPORT_SYMBOL(unregister_adapter_interrupt);

static irqreturn_t do_airq_interrupt(int irq, void *dummy)
{
	struct tpi_info *tpi_info;
	struct airq_struct *airq;
	struct hlist_head *head;

	__this_cpu_write(s390_idle.nohz_delay, 1);
	tpi_info = (struct tpi_info *) &get_irq_regs()->int_code;
	head = &airq_lists[tpi_info->isc];
	rcu_read_lock();
	hlist_for_each_entry_rcu(airq, head, list)
		if ((*airq->lsi_ptr & airq->lsi_mask) != 0)
			airq->handler(airq);
	rcu_read_unlock();

	return IRQ_HANDLED;
}

static struct irqaction airq_interrupt = {
	.name	 = "AIO",
	.handler = do_airq_interrupt,
};

void __init init_airq_interrupts(void)
{
	irq_set_chip_and_handler(THIN_INTERRUPT,
				 &dummy_irq_chip, handle_percpu_irq);
	setup_irq(THIN_INTERRUPT, &airq_interrupt);
}

/**
 * airq_iv_create - create an interrupt vector
 * @bits: number of bits in the interrupt vector
 * @flags: allocation flags
 *
 * Returns a pointer to an interrupt vector structure
 */
struct airq_iv *airq_iv_create(unsigned long bits, unsigned long flags)
{
	struct airq_iv *iv;
	unsigned long size;

	iv = kzalloc(sizeof(*iv), GFP_KERNEL);
	if (!iv)
		goto out;
	iv->bits = bits;
	size = BITS_TO_LONGS(bits) * sizeof(unsigned long);
	iv->vector = kzalloc(size, GFP_KERNEL);
	if (!iv->vector)
		goto out_free;
	if (flags & AIRQ_IV_ALLOC) {
		iv->avail = kmalloc(size, GFP_KERNEL);
		if (!iv->avail)
			goto out_free;
		memset(iv->avail, 0xff, size);
		iv->end = 0;
	} else
		iv->end = bits;
	if (flags & AIRQ_IV_BITLOCK) {
		iv->bitlock = kzalloc(size, GFP_KERNEL);
		if (!iv->bitlock)
			goto out_free;
	}
	if (flags & AIRQ_IV_PTR) {
		size = bits * sizeof(unsigned long);
		iv->ptr = kzalloc(size, GFP_KERNEL);
		if (!iv->ptr)
			goto out_free;
	}
	if (flags & AIRQ_IV_DATA) {
		size = bits * sizeof(unsigned int);
		iv->data = kzalloc(size, GFP_KERNEL);
		if (!iv->data)
			goto out_free;
	}
	spin_lock_init(&iv->lock);
	return iv;

out_free:
	kfree(iv->ptr);
	kfree(iv->bitlock);
	kfree(iv->avail);
	kfree(iv->vector);
	kfree(iv);
out:
	return NULL;
}
EXPORT_SYMBOL(airq_iv_create);

/**
 * airq_iv_release - release an interrupt vector
 * @iv: pointer to interrupt vector structure
 */
void airq_iv_release(struct airq_iv *iv)
{
	kfree(iv->data);
	kfree(iv->ptr);
	kfree(iv->bitlock);
	kfree(iv->vector);
	kfree(iv->avail);
	kfree(iv);
}
EXPORT_SYMBOL(airq_iv_release);

/**
 * airq_iv_alloc_bit - allocate an irq bit from an interrupt vector
 * @iv: pointer to an interrupt vector structure
 *
 * Returns the bit number of the allocated irq, or -1UL if no bit
 * is available or the AIRQ_IV_ALLOC flag has not been specified
 */
unsigned long airq_iv_alloc_bit(struct airq_iv *iv)
{
	unsigned long bit;

	if (!iv->avail)
		return -1UL;
	spin_lock(&iv->lock);
	bit = find_first_bit_inv(iv->avail, iv->bits);
	if (bit < iv->bits) {
		clear_bit_inv(bit, iv->avail);
		if (bit >= iv->end)
			iv->end = bit + 1;
	} else
		bit = -1UL;
	spin_unlock(&iv->lock);
	return bit;

}
EXPORT_SYMBOL(airq_iv_alloc_bit);

/**
 * airq_iv_free_bit - free an irq bit of an interrupt vector
 * @iv: pointer to interrupt vector structure
 * @bit: number of the irq bit to free
 */
void airq_iv_free_bit(struct airq_iv *iv, unsigned long bit)
{
	if (!iv->avail)
		return;
	spin_lock(&iv->lock);
	/* Clear (possibly left over) interrupt bit */
	clear_bit_inv(bit, iv->vector);
	/* Make the bit position available again */
	set_bit_inv(bit, iv->avail);
	if (bit == iv->end - 1) {
		/* Find new end of bit-field */
		while (--iv->end > 0)
			if (!test_bit_inv(iv->end - 1, iv->avail))
				break;
	}
	spin_unlock(&iv->lock);
}
EXPORT_SYMBOL(airq_iv_free_bit);

/**
 * airq_iv_scan - scan interrupt vector for non-zero bits
 * @iv: pointer to interrupt vector structure
 * @start: bit number to start the search
 * @end: bit number to end the search
 *
 * Returns the bit number of the next non-zero interrupt bit, or
 * -1UL if the scan completed without finding any more any non-zero bits.
 */
unsigned long airq_iv_scan(struct airq_iv *iv, unsigned long start,
			   unsigned long end)
{
	unsigned long bit;

	/* Find non-zero bit starting from 'ivs->next'. */
	bit = find_next_bit_inv(iv->vector, end, start);
	if (bit >= end)
		return -1UL;
	clear_bit_inv(bit, iv->vector);
	return bit;
}
EXPORT_SYMBOL(airq_iv_scan);
>>>>>>> refs/remotes/origin/master
