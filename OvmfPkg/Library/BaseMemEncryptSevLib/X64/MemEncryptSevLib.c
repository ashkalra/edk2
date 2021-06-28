/** @file

  Secure Encrypted Virtualization (SEV) library helper function

  Copyright (c) 2017 - 2020, AMD Incorporated. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/MemEncryptSevLib.h>
#include <Register/Amd/Cpuid.h>
#include <Register/Amd/Msr.h>
#include <Register/Cpuid.h>

#include "VirtualMemory.h"

/**
  This function clears memory encryption bit for the memory region specified by
  BaseAddress and NumPages from the current page table context.

  @param[in]  Cr3BaseAddress          Cr3 Base Address (if zero then use
                                      current CR3)
  @param[in]  BaseAddress             The physical address that is the start
                                      address of a memory region.
  @param[in]  NumPages                The number of pages from start memory
                                      region.

  @retval RETURN_SUCCESS              The attributes were cleared for the
                                      memory region.
  @retval RETURN_INVALID_PARAMETER    Number of pages is zero.
  @retval RETURN_UNSUPPORTED          Clearing the memory encryption attribute
                                      is not supported
**/
RETURN_STATUS
EFIAPI
MemEncryptSevClearPageEncMask (
  IN PHYSICAL_ADDRESS         Cr3BaseAddress,
  IN PHYSICAL_ADDRESS         BaseAddress,
  IN UINTN                    NumPages
  )
{
  return InternalMemEncryptSevSetMemoryDecrypted (
           Cr3BaseAddress,
           BaseAddress,
           EFI_PAGES_TO_SIZE (NumPages)
           );
}

/**
  This function sets memory encryption bit for the memory region specified by
  BaseAddress and NumPages from the current page table context.

  @param[in]  Cr3BaseAddress          Cr3 Base Address (if zero then use
                                      current CR3)
  @param[in]  BaseAddress             The physical address that is the start
                                      address of a memory region.
  @param[in]  NumPages                The number of pages from start memory
                                      region.

  @retval RETURN_SUCCESS              The attributes were set for the memory
                                      region.
  @retval RETURN_INVALID_PARAMETER    Number of pages is zero.
  @retval RETURN_UNSUPPORTED          Setting the memory encryption attribute
                                      is not supported
**/
RETURN_STATUS
EFIAPI
MemEncryptSevSetPageEncMask (
  IN PHYSICAL_ADDRESS         Cr3BaseAddress,
  IN PHYSICAL_ADDRESS         BaseAddress,
  IN UINTN                    NumPages
  )
{
  return InternalMemEncryptSevSetMemoryEncrypted (
           Cr3BaseAddress,
           BaseAddress,
           EFI_PAGES_TO_SIZE (NumPages)
           );
}

/**
  Returns the encryption state of the specified virtual address range.

  @param[in]  Cr3BaseAddress          Cr3 Base Address (if zero then use
                                      current CR3)
  @param[in]  BaseAddress             Base address to check
  @param[in]  Length                  Length of virtual address range

  @retval MemEncryptSevAddressRangeUnencrypted  Address range is mapped
                                                unencrypted
  @retval MemEncryptSevAddressRangeEncrypted    Address range is mapped
                                                encrypted
  @retval MemEncryptSevAddressRangeMixed        Address range is mapped mixed
  @retval MemEncryptSevAddressRangeError        Address range is not mapped
**/
MEM_ENCRYPT_SEV_ADDRESS_RANGE_STATE
EFIAPI
MemEncryptSevGetAddressRangeState (
  IN PHYSICAL_ADDRESS         Cr3BaseAddress,
  IN PHYSICAL_ADDRESS         BaseAddress,
  IN UINTN                    Length
  )
{
  return InternalMemEncryptSevGetAddressRangeState (
           Cr3BaseAddress,
           BaseAddress,
           Length
           );
}

/**
  This function clears memory encryption bit for the mmio region specified by
  BaseAddress and NumPages.

  @param[in]  Cr3BaseAddress          Cr3 Base Address (if zero then use
                                      current CR3)
  @param[in]  BaseAddress             The physical address that is the start
                                      address of a mmio region.
  @param[in]  NumPages                The number of pages from start memory
                                      region.

  @retval RETURN_SUCCESS              The attributes were cleared for the
                                      memory region.
  @retval RETURN_INVALID_PARAMETER    Number of pages is zero.
  @retval RETURN_UNSUPPORTED          Clearing the memory encryption attribute
                                      is not supported
**/
RETURN_STATUS
EFIAPI
MemEncryptSevClearMmioPageEncMask (
  IN PHYSICAL_ADDRESS         Cr3BaseAddress,
  IN PHYSICAL_ADDRESS         BaseAddress,
  IN UINTN                    NumPages
  )
{
  return InternalMemEncryptSevClearMmioPageEncMask (
           Cr3BaseAddress,
           BaseAddress,
           EFI_PAGES_TO_SIZE (NumPages)
           );

}

//
// Interface exposed by the ASM implementation of the core hypercall
//
INTN
EFIAPI
SetMemoryEncDecHypercall3AsmStub (
  IN  UINTN  HypercallNum,
  IN  UINTN  PhysicalAddress,
  IN  UINTN  Pages,
  IN  UINTN  Attributes
  );

/**
 This hyercall is used to notify hypervisor when a page is marked as
 'decrypted' (i.e C-bit removed).

 @param[in]   PhysicalAddress       The physical address that is the start address
                                    of a memory region. The physical address is
                                    expected to be PAGE_SIZE aligned.
 @param[in]   Pages                 Number of Pages in the memory region.
 @param[in]   Mode                  SetCBit or ClearCBit

@retval RETURN_SUCCESS              Hypercall returned success.
**/
RETURN_STATUS
EFIAPI
SetMemoryEncDecHypercall3 (
  IN  UINTN     PhysicalAddress,
  IN  UINTN     Pages,
  IN  UINTN     Mode
  )
{
  RETURN_STATUS Status = RETURN_UNSUPPORTED;
  INTN Error;

  if (MemEncryptSevLiveMigrationIsEnabled ()) {
    Status = EFI_SUCCESS;
    /*
     * The encryption bit is set/clear on the smallest page size, hence
     * use the 4k page size in MAP_GPA_RANGE hypercall below.
     * Also, the hypercall expects the guest physical address to be
     * page-aligned.
     */
    Error = SetMemoryEncDecHypercall3AsmStub (
              KVM_HC_MAP_GPA_RANGE,
              (PhysicalAddress & (~(EFI_PAGE_SIZE-1))),
              Pages,
              KVM_MAP_GPA_RANGE_PAGE_SZ_4K | KVM_MAP_GPA_RANGE_ENC_STAT(Mode)
              );

    if (Error != 0) {
      DEBUG ((DEBUG_ERROR,
              "SetMemoryEncDecHypercall3, Phys = %Lx, Pages = %Ld, Err = %Ld\n",
              PhysicalAddress,
              Pages,
              (INT64)Error));
      Status = RETURN_NO_MAPPING;
    }
  }

  return Status;
}
