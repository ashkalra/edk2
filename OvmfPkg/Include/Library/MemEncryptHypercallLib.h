/** @file

  Define Secure Encrypted Virtualization (SEV) hypercall library.

  Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _MEM_ENCRYPT_HYPERCALL_LIB_H_
#define _MEM_ENCRYPT_HYPERCALL_LIB_H_

#include <Base.h>

#define SEV_PAGE_ENC_HYPERCALL  12

/**
 This hyercall is used to notify hypervisor when a page is marked as
 'decrypted' (i.e C-bit removed).

 @param[in]   PhysicalAddress       The physical address that is the start address
                                    of a memory region.
 @param[in]   Length                The length of memory region
 @param[in]   Mode                  SetCBit or ClearCBit

**/

VOID
EFIAPI
SetMemoryEncDecHypercall3 (
  IN  UINTN     PhysicalAddress,
  IN  UINTN     Length,
  IN  UINTN     Mode
  );

#endif
