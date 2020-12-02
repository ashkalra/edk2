/** @file

  Secure Encrypted Virtualization (SEV) hypercall helper library

  Copyright (c) 2020, AMD Incorporated. All rights reserved.<BR>

  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/VmgExitLib.h>
#include <Register/Amd/Ghcb.h>
#include <Register/Amd/Msr.h>
#include <Library/MemEncryptSevLib.h>
#include <Library/MemEncryptHypercallLib.h>

//
// Interface exposed by the ASM implementation of the core hypercall
//
//

VOID
EFIAPI
SetMemoryEncDecHypercall3AsmStub (
  IN  UINTN  HypercallNum,
  IN  UINTN  PhysicalAddress,
  IN  UINTN  Length,
  IN  UINTN  Mode
  );

/**
 This function returns the current CPU privilege level, implemented
 in ASM helper stub.

**/

UINT8
EFIAPI
GetCurrentCpuPrivilegeLevel (
  VOID
  );

STATIC
VOID
GhcbSetRegValid (
  IN OUT GHCB       *Ghcb,
  IN GHCB_REGISTER  Reg
  )
{
  UINT32  RegIndex;
  UINT32  RegBit;

  RegIndex = Reg / 8;
  RegBit   = Reg & 0x07;

  Ghcb->SaveArea.ValidBitmap[RegIndex] |= (1 << RegBit);
}

VOID
EFIAPI
SetMemoryEncDecHypercall3 (
  IN PHYSICAL_ADDRESS PhysicalAddress,
  IN UINTN            Pages,
  IN UINTN            Mode
  )
{
  if (MemEncryptSevEsIsEnabled ()) {
    MSR_SEV_ES_GHCB_REGISTER  Msr;
    GHCB                      *Ghcb;
    BOOLEAN                   InterruptState;
    UINT64                    Status;

    Msr.GhcbPhysicalAddress = AsmReadMsr64 (MSR_SEV_ES_GHCB);
    Ghcb = Msr.Ghcb;

    VmgInit (Ghcb, &InterruptState);

    Ghcb->SaveArea.Rax = SEV_PAGE_ENC_HYPERCALL;
    GhcbSetRegValid (Ghcb, GhcbRax);
    Ghcb->SaveArea.Rbx = PhysicalAddress;
    GhcbSetRegValid (Ghcb, GhcbRbx);
    Ghcb->SaveArea.Rcx = Pages;
    GhcbSetRegValid (Ghcb, GhcbRcx);
    Ghcb->SaveArea.Rdx = Mode;
    GhcbSetRegValid (Ghcb, GhcbRdx);
    Ghcb->SaveArea.Cpl = GetCurrentCpuPrivilegeLevel();
    GhcbSetRegValid (Ghcb, GhcbCpl);

    Status = VmgExit (Ghcb, SVM_EXIT_VMMCALL, 0, 0);
    if (Status) {
      DEBUG ((DEBUG_ERROR, "SVM_EXIT_VMMCALL failed %lx\n", Status));
    }
    VmgDone (Ghcb, InterruptState);
  } else {
    SetMemoryEncDecHypercall3AsmStub (
      SEV_PAGE_ENC_HYPERCALL,
      PhysicalAddress,
      Pages,
      Mode);
  }
}
