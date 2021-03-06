/*
 * Architecture specific OF callbacks.
 */
#include <linux/bootmem.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <linux/io.h>
=======
#include <linux/export.h>
#include <linux/io.h>
#include <linux/irqdomain.h>
>>>>>>> refs/remotes/origin/cm-10.0
=======
#include <linux/export.h>
#include <linux/io.h>
#include <linux/irqdomain.h>
>>>>>>> refs/remotes/origin/master
#include <linux/interrupt.h>
#include <linux/list.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/of_irq.h>
#include <linux/slab.h>
#include <linux/pci.h>
#include <linux/of_pci.h>
#include <linux/initrd.h>

#include <asm/hpet.h>
<<<<<<< HEAD
<<<<<<< HEAD
#include <asm/irq_controller.h>
=======
>>>>>>> refs/remotes/origin/cm-10.0
#include <asm/apic.h>
#include <asm/pci_x86.h>

__initdata u64 initial_dtb;
char __initdata cmd_line[COMMAND_LINE_SIZE];
<<<<<<< HEAD
static LIST_HEAD(irq_domains);
static DEFINE_RAW_SPINLOCK(big_irq_lock);

int __initdata of_ioapic;

#ifdef CONFIG_X86_IO_APIC
static void add_interrupt_host(struct irq_domain *ih)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&big_irq_lock, flags);
	list_add(&ih->l, &irq_domains);
	raw_spin_unlock_irqrestore(&big_irq_lock, flags);
}
#endif

static struct irq_domain *get_ih_from_node(struct device_node *controller)
{
	struct irq_domain *ih, *found = NULL;
	unsigned long flags;

	raw_spin_lock_irqsave(&big_irq_lock, flags);
	list_for_each_entry(ih, &irq_domains, l) {
		if (ih->controller ==  controller) {
			found = ih;
			break;
		}
	}
	raw_spin_unlock_irqrestore(&big_irq_lock, flags);
	return found;
}

unsigned int irq_create_of_mapping(struct device_node *controller,
				   const u32 *intspec, unsigned int intsize)
{
	struct irq_domain *ih;
	u32 virq, type;
	int ret;

	ih = get_ih_from_node(controller);
	if (!ih)
		return 0;
	ret = ih->xlate(ih, intspec, intsize, &virq, &type);
	if (ret)
		return 0;
	if (type == IRQ_TYPE_NONE)
		return virq;
	irq_set_irq_type(virq, type);
	return virq;
}
EXPORT_SYMBOL_GPL(irq_create_of_mapping);

=======

int __initdata of_ioapic;

>>>>>>> refs/remotes/origin/cm-10.0
unsigned long pci_address_to_pio(phys_addr_t address)
{
	/*
	 * The ioport address can be directly used by inX / outX
	 */
	BUG_ON(address >= (1 << 16));
	return (unsigned long)address;
}
EXPORT_SYMBOL_GPL(pci_address_to_pio);

=======
#include <asm/apic.h>
#include <asm/pci_x86.h>
#include <asm/setup.h>

__initdata u64 initial_dtb;
char __initdata cmd_line[COMMAND_LINE_SIZE];

int __initdata of_ioapic;

>>>>>>> refs/remotes/origin/master
void __init early_init_dt_scan_chosen_arch(unsigned long node)
{
	BUG();
}

void __init early_init_dt_add_memory_arch(u64 base, u64 size)
{
	BUG();
}

void * __init early_init_dt_alloc_memory_arch(u64 size, u64 align)
{
	return __alloc_bootmem(size, align, __pa(MAX_DMA_ADDRESS));
}

<<<<<<< HEAD
#ifdef CONFIG_BLK_DEV_INITRD
void __init early_init_dt_setup_initrd_arch(unsigned long start,
					    unsigned long end)
{
	initrd_start = (unsigned long)__va(start);
	initrd_end = (unsigned long)__va(end);
	initrd_below_start_ok = 1;
}
#endif

=======
>>>>>>> refs/remotes/origin/master
void __init add_dtb(u64 data)
{
	initial_dtb = data + offsetof(struct setup_data, data);
}

/*
 * CE4100 ids. Will be moved to machine_device_initcall() once we have it.
 */
static struct of_device_id __initdata ce4100_ids[] = {
	{ .compatible = "intel,ce4100-cp", },
	{ .compatible = "isa", },
	{ .compatible = "pci", },
	{},
};

static int __init add_bus_probe(void)
{
	if (!of_have_populated_dt())
		return 0;

	return of_platform_bus_probe(NULL, ce4100_ids, NULL);
}
module_init(add_bus_probe);

#ifdef CONFIG_PCI
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> refs/remotes/origin/master
struct device_node *pcibios_get_phb_of_node(struct pci_bus *bus)
{
	struct device_node *np;

	for_each_node_by_type(np, "pci") {
		const void *prop;
		unsigned int bus_min;

		prop = of_get_property(np, "bus-range", NULL);
		if (!prop)
			continue;
		bus_min = be32_to_cpup(prop);
		if (bus->number == bus_min)
			return np;
	}
	return NULL;
}

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
static int x86_of_pci_irq_enable(struct pci_dev *dev)
{
	struct of_irq oirq;
=======
static int x86_of_pci_irq_enable(struct pci_dev *dev)
{
>>>>>>> refs/remotes/origin/master
	u32 virq;
	int ret;
	u8 pin;

	ret = pci_read_config_byte(dev, PCI_INTERRUPT_PIN, &pin);
	if (ret)
		return ret;
	if (!pin)
		return 0;

<<<<<<< HEAD
	ret = of_irq_map_pci(dev, &oirq);
	if (ret)
		return ret;

	virq = irq_create_of_mapping(oirq.controller, oirq.specifier,
			oirq.size);
=======
	virq = of_irq_parse_and_map_pci(dev, 0, 0);
>>>>>>> refs/remotes/origin/master
	if (virq == 0)
		return -EINVAL;
	dev->irq = virq;
	return 0;
}

static void x86_of_pci_irq_disable(struct pci_dev *dev)
{
}

<<<<<<< HEAD
void __cpuinit x86_of_pci_init(void)
{
<<<<<<< HEAD
	struct device_node *np;

	pcibios_enable_irq = x86_of_pci_irq_enable;
	pcibios_disable_irq = x86_of_pci_irq_disable;

	for_each_node_by_type(np, "pci") {
		const void *prop;
		struct pci_bus *bus;
		unsigned int bus_min;
		struct device_node *child;

		prop = of_get_property(np, "bus-range", NULL);
		if (!prop)
			continue;
		bus_min = be32_to_cpup(prop);

		bus = pci_find_bus(0, bus_min);
		if (!bus) {
			printk(KERN_ERR "Can't find a node for bus %s.\n",
					np->full_name);
			continue;
		}

		if (bus->self)
			bus->self->dev.of_node = np;
		else
			bus->dev.of_node = np;

		for_each_child_of_node(np, child) {
			struct pci_dev *dev;
			u32 devfn;

			prop = of_get_property(child, "reg", NULL);
			if (!prop)
				continue;

			devfn = (be32_to_cpup(prop) >> 8) & 0xff;
			dev = pci_get_slot(bus, devfn);
			if (!dev)
				continue;
			dev->dev.of_node = child;
			pci_dev_put(dev);
		}
	}
=======
	pcibios_enable_irq = x86_of_pci_irq_enable;
	pcibios_disable_irq = x86_of_pci_irq_disable;
>>>>>>> refs/remotes/origin/cm-10.0
=======
void x86_of_pci_init(void)
{
	pcibios_enable_irq = x86_of_pci_irq_enable;
	pcibios_disable_irq = x86_of_pci_irq_disable;
>>>>>>> refs/remotes/origin/master
}
#endif

static void __init dtb_setup_hpet(void)
{
#ifdef CONFIG_HPET_TIMER
	struct device_node *dn;
	struct resource r;
	int ret;

	dn = of_find_compatible_node(NULL, NULL, "intel,ce4100-hpet");
	if (!dn)
		return;
	ret = of_address_to_resource(dn, 0, &r);
	if (ret) {
		WARN_ON(1);
		return;
	}
	hpet_address = r.start;
#endif
}

static void __init dtb_lapic_setup(void)
{
#ifdef CONFIG_X86_LOCAL_APIC
	struct device_node *dn;
	struct resource r;
	int ret;

	dn = of_find_compatible_node(NULL, NULL, "intel,ce4100-lapic");
	if (!dn)
		return;

	ret = of_address_to_resource(dn, 0, &r);
	if (WARN_ON(ret))
		return;

	/* Did the boot loader setup the local APIC ? */
	if (!cpu_has_apic) {
		if (apic_force_enable(r.start))
			return;
	}
	smp_found_config = 1;
	pic_mode = 1;
	register_lapic_address(r.start);
	generic_processor_info(boot_cpu_physical_apicid,
			       GET_APIC_VERSION(apic_read(APIC_LVR)));
#endif
}

#ifdef CONFIG_X86_IO_APIC
static unsigned int ioapic_id;

static void __init dtb_add_ioapic(struct device_node *dn)
{
	struct resource r;
	int ret;

	ret = of_address_to_resource(dn, 0, &r);
	if (ret) {
		printk(KERN_ERR "Can't obtain address from node %s.\n",
				dn->full_name);
		return;
	}
	mp_register_ioapic(++ioapic_id, r.start, gsi_top);
}

static void __init dtb_ioapic_setup(void)
{
	struct device_node *dn;

	for_each_compatible_node(dn, NULL, "intel,ce4100-ioapic")
		dtb_add_ioapic(dn);

	if (nr_ioapics) {
		of_ioapic = 1;
		return;
	}
	printk(KERN_ERR "Error: No information about IO-APIC in OF.\n");
}
#else
static void __init dtb_ioapic_setup(void) {}
#endif

static void __init dtb_apic_setup(void)
{
	dtb_lapic_setup();
	dtb_ioapic_setup();
}

#ifdef CONFIG_OF_FLATTREE
static void __init x86_flattree_get_config(void)
{
	u32 size, map_len;
<<<<<<< HEAD
	void *new_dtb;
=======
	struct boot_param_header *dt;
>>>>>>> refs/remotes/origin/master

	if (!initial_dtb)
		return;

	map_len = max(PAGE_SIZE - (initial_dtb & ~PAGE_MASK),
			(u64)sizeof(struct boot_param_header));

<<<<<<< HEAD
	initial_boot_params = early_memremap(initial_dtb, map_len);
	size = be32_to_cpu(initial_boot_params->totalsize);
	if (map_len < size) {
		early_iounmap(initial_boot_params, map_len);
		initial_boot_params = early_memremap(initial_dtb, size);
		map_len = size;
	}

	new_dtb = alloc_bootmem(size);
	memcpy(new_dtb, initial_boot_params, size);
	early_iounmap(initial_boot_params, map_len);

	initial_boot_params = new_dtb;

	/* root level address cells */
	of_scan_flat_dt(early_init_dt_scan_root, NULL);

	unflatten_device_tree();
=======
	dt = early_memremap(initial_dtb, map_len);
	size = be32_to_cpu(dt->totalsize);
	if (map_len < size) {
		early_iounmap(dt, map_len);
		dt = early_memremap(initial_dtb, size);
		map_len = size;
	}

	initial_boot_params = dt;
	unflatten_and_copy_device_tree();
	early_iounmap(dt, map_len);
>>>>>>> refs/remotes/origin/master
}
#else
static inline void x86_flattree_get_config(void) { }
#endif

void __init x86_dtb_init(void)
{
	x86_flattree_get_config();

	if (!of_have_populated_dt())
		return;

	dtb_setup_hpet();
	dtb_apic_setup();
}

#ifdef CONFIG_X86_IO_APIC

struct of_ioapic_type {
	u32 out_type;
	u32 trigger;
	u32 polarity;
};

static struct of_ioapic_type of_ioapic_type[] =
{
	{
		.out_type	= IRQ_TYPE_EDGE_RISING,
		.trigger	= IOAPIC_EDGE,
		.polarity	= 1,
	},
	{
		.out_type	= IRQ_TYPE_LEVEL_LOW,
		.trigger	= IOAPIC_LEVEL,
		.polarity	= 0,
	},
	{
		.out_type	= IRQ_TYPE_LEVEL_HIGH,
		.trigger	= IOAPIC_LEVEL,
		.polarity	= 1,
	},
	{
		.out_type	= IRQ_TYPE_EDGE_FALLING,
		.trigger	= IOAPIC_EDGE,
		.polarity	= 0,
	},
};

<<<<<<< HEAD
<<<<<<< HEAD
static int ioapic_xlate(struct irq_domain *id, const u32 *intspec, u32 intsize,
			u32 *out_hwirq, u32 *out_type)
{
	struct mp_ioapic_gsi *gsi_cfg;
	struct io_apic_irq_attr attr;
	struct of_ioapic_type *it;
	u32 line, idx, type;

	if (intsize < 2)
		return -EINVAL;

	line = *intspec;
	idx = (u32) id->priv;
	gsi_cfg = mp_ioapic_gsi_routing(idx);
	*out_hwirq = line + gsi_cfg->gsi_base;

	intspec++;
	type = *intspec;

	if (type >= ARRAY_SIZE(of_ioapic_type))
		return -EINVAL;

	it = of_ioapic_type + type;
	*out_type = it->out_type;

	set_io_apic_irq_attr(&attr, idx, line, it->trigger, it->polarity);

	return io_apic_setup_irq_pin_once(*out_hwirq, cpu_to_node(0), &attr);
}

=======
=======
>>>>>>> refs/remotes/origin/master
static int ioapic_xlate(struct irq_domain *domain,
			struct device_node *controller,
			const u32 *intspec, u32 intsize,
			irq_hw_number_t *out_hwirq, u32 *out_type)
{
	struct io_apic_irq_attr attr;
	struct of_ioapic_type *it;
	u32 line, idx;
	int rc;

	if (WARN_ON(intsize < 2))
		return -EINVAL;

	line = intspec[0];

	if (intspec[1] >= ARRAY_SIZE(of_ioapic_type))
		return -EINVAL;

	it = &of_ioapic_type[intspec[1]];

	idx = (u32) domain->host_data;
	set_io_apic_irq_attr(&attr, idx, line, it->trigger, it->polarity);

	rc = io_apic_setup_irq_pin_once(irq_find_mapping(domain, line),
					cpu_to_node(0), &attr);
	if (rc)
		return rc;

	*out_hwirq = line;
	*out_type = it->out_type;
	return 0;
}

const struct irq_domain_ops ioapic_irq_domain_ops = {
	.xlate = ioapic_xlate,
};

<<<<<<< HEAD
>>>>>>> refs/remotes/origin/cm-10.0
=======
static void dt_add_ioapic_domain(unsigned int ioapic_num,
		struct device_node *np)
{
	struct irq_domain *id;
	struct mp_ioapic_gsi *gsi_cfg;
	int ret;
	int num;

	gsi_cfg = mp_ioapic_gsi_routing(ioapic_num);
	num = gsi_cfg->gsi_end - gsi_cfg->gsi_base + 1;

	id = irq_domain_add_linear(np, num, &ioapic_irq_domain_ops,
			(void *)ioapic_num);
	BUG_ON(!id);
	if (gsi_cfg->gsi_base == 0) {
		/*
		 * The first NR_IRQS_LEGACY irq descs are allocated in
		 * early_irq_init() and need just a mapping. The
		 * remaining irqs need both. All of them are preallocated
		 * and assigned so we can keep the 1:1 mapping which the ioapic
		 * is having.
		 */
		irq_domain_associate_many(id, 0, 0, NR_IRQS_LEGACY);

		if (num > NR_IRQS_LEGACY) {
			ret = irq_create_strict_mappings(id, NR_IRQS_LEGACY,
					NR_IRQS_LEGACY, num - NR_IRQS_LEGACY);
			if (ret)
				pr_err("Error creating mapping for the "
						"remaining IRQs: %d\n", ret);
		}
		irq_set_default_host(id);
	} else {
		ret = irq_create_strict_mappings(id, gsi_cfg->gsi_base, 0, num);
		if (ret)
			pr_err("Error creating IRQ mapping: %d\n", ret);
	}
}

>>>>>>> refs/remotes/origin/master
static void __init ioapic_add_ofnode(struct device_node *np)
{
	struct resource r;
	int i, ret;

	ret = of_address_to_resource(np, 0, &r);
	if (ret) {
		printk(KERN_ERR "Failed to obtain address for %s\n",
				np->full_name);
		return;
	}

	for (i = 0; i < nr_ioapics; i++) {
		if (r.start == mpc_ioapic_addr(i)) {
<<<<<<< HEAD
			struct irq_domain *id;
<<<<<<< HEAD

			id = kzalloc(sizeof(*id), GFP_KERNEL);
			BUG_ON(!id);
			id->controller = np;
			id->xlate = ioapic_xlate;
			id->priv = (void *)i;
			add_interrupt_host(id);
=======
			struct mp_ioapic_gsi *gsi_cfg;

			gsi_cfg = mp_ioapic_gsi_routing(i);

			id = irq_domain_add_legacy(np, 32, gsi_cfg->gsi_base, 0,
						   &ioapic_irq_domain_ops,
						   (void*)i);
			BUG_ON(!id);
>>>>>>> refs/remotes/origin/cm-10.0
=======
			dt_add_ioapic_domain(i, np);
>>>>>>> refs/remotes/origin/master
			return;
		}
	}
	printk(KERN_ERR "IOxAPIC at %s is not registered.\n", np->full_name);
}

void __init x86_add_irq_domains(void)
{
	struct device_node *dp;

	if (!of_have_populated_dt())
		return;

	for_each_node_with_property(dp, "interrupt-controller") {
		if (of_device_is_compatible(dp, "intel,ce4100-ioapic"))
			ioapic_add_ofnode(dp);
	}
}
#else
void __init x86_add_irq_domains(void) { }
#endif
