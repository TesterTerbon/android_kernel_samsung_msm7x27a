/*
 * Arch specific extensions to struct device
 *
 * This file is released under the GPLv2
 */
#ifndef _ASM_IA64_DEVICE_H
#define _ASM_IA64_DEVICE_H

struct dev_archdata {
<<<<<<< HEAD
#ifdef CONFIG_ACPI
	void	*acpi_handle;
#endif
<<<<<<< HEAD
#ifdef CONFIG_DMAR
=======
#ifdef CONFIG_INTEL_IOMMU
>>>>>>> refs/remotes/origin/cm-10.0
=======
#ifdef CONFIG_INTEL_IOMMU
>>>>>>> refs/remotes/origin/master
	void *iommu; /* hook for IOMMU specific extension */
#endif
};

struct pdev_archdata {
};

#endif /* _ASM_IA64_DEVICE_H */
