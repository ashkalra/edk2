/** @file

  Secure Encrypted Virtualization (SEV) hypercall helper library

  Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>

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
  IN PHYSICAL_ADDRESS PhysicalAddress,
  IN UINTN            Pages,
  IN UINTN            Mode
  )
{
  //
  // Memory encryption bit is not accessible in 32-bit mode
  //
}
