/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_POWERPC_PGTABLE_TYPES_H
#define _ASM_POWERPC_PGTABLE_TYPES_H

#if defined(__CHECKER__) || !defined(CONFIG_PPC32)
#define STRICT_MM_TYPECHECKS
#endif

/* PTE level */
#if defined(CONFIG_PPC_8xx) && defined(CONFIG_PPC_16K_PAGES)
typedef struct { pte_basic_t pte, pte1, pte2, pte3; } pte_t;
#elif defined(STRICT_MM_TYPECHECKS)
typedef struct { pte_basic_t pte; } pte_t;
#else
typedef pte_basic_t pte_t;
#endif

#if defined(STRICT_MM_TYPECHECKS) || \
    (defined(CONFIG_PPC_8xx) && defined(CONFIG_PPC_16K_PAGES))
#define __pte(x)	((pte_t) { (x) })
static inline pte_basic_t pte_val(pte_t x)
{
	return x.pte;
}
#else
#define __pte(x)	((pte_t)(x))
static inline pte_basic_t pte_val(pte_t x)
{
	return x;
}
#endif

/* PMD level */
#ifdef CONFIG_PPC64
typedef struct { unsigned long pmd; } pmd_t;
#define __pmd(x)	((pmd_t) { (x) })
static inline unsigned long pmd_val(pmd_t x)
{
	return x.pmd;
}

/* 64 bit always use 4 level table. */
typedef struct { unsigned long pud; } pud_t;
#define __pud(x)	((pud_t) { (x) })
static inline unsigned long pud_val(pud_t x)
{
	return x.pud;
}
#endif /* CONFIG_PPC64 */

/* PGD level */
#if defined(CONFIG_PPC_85xx) && defined(CONFIG_PTE_64BIT)
typedef struct { unsigned long long pgd; } pgd_t;

static inline unsigned long long pgd_val(pgd_t x)
{
	return x.pgd;
}
#else
typedef struct { unsigned long pgd; } pgd_t;

static inline unsigned long pgd_val(pgd_t x)
{
	return x.pgd;
}
#endif
#define __pgd(x)	((pgd_t) { (x) })

/* Page protection bits */
typedef struct { unsigned long pgprot; } pgprot_t;
#define pgprot_val(x)	((x).pgprot)
#define __pgprot(x)	((pgprot_t) { (x) })

/*
 * With hash config 64k pages additionally define a bigger "real PTE" type that
 * gathers the "second half" part of the PTE for pseudo 64k pages
 */
#ifdef CONFIG_PPC_64K_PAGES
typedef struct { pte_t pte; unsigned long hidx; } real_pte_t;
#else
typedef struct { pte_t pte; } real_pte_t;
#endif

#ifdef CONFIG_PPC_BOOK3S_64
#include <asm/cmpxchg.h>

static inline bool pte_xchg(pte_t *ptep, pte_t old, pte_t new)
{
	unsigned long *p = (unsigned long *)ptep;

	/* See comment in switch_mm_irqs_off() */
	return pte_val(old) == __cmpxchg_u64(p, pte_val(old), pte_val(new));
}
#endif

#endif /* _ASM_POWERPC_PGTABLE_TYPES_H */
