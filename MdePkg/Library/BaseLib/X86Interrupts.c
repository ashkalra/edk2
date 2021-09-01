/** @file
  x64 enable interrupt functions.

  Copyright (c) 2021, Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/


#include <Register/Amd/Msr.h>
#include <Register/Amd/Ghcb.h>

#include "BaseLibInternals.h"

typedef struct {
  UINT32  Dr7Cached;
  UINT64  Dr7;

  UINTN   VcCount;
  VOID    *GhcbBackupPages;

  HVDB    *Hvdb;
  BOOLEAN HvdbPendingEvent;
} SEV_ES_PER_CPU_DATA;

STATIC
VOID
EFIAPI
RunHvdbPendingEvents (
  VOID
  )
{
  MSR_SEV_ES_GHCB_REGISTER  Msr;
  GHCB                      *Ghcb;
  SEV_ES_PER_CPU_DATA       *SevEsData;

  Msr.GhcbPhysicalAddress = AsmReadMsr64 (MSR_SEV_ES_GHCB);
  ASSERT (Msr.GhcbInfo.Function == 0);

  if (Msr.Ghcb == 0)
    return;

  Ghcb = Msr.Ghcb;
  SevEsData = (SEV_ES_PER_CPU_DATA *) (Ghcb + 1);

  if (SevEsData->HvdbPendingEvent) {
    asm volatile ("int %0\t\n" :: "i" (28));
  }
}

VOID
EFIAPI
EnableInterrupts (
  VOID
  )
{
  AsmEnableInterrupts ();
  RunHvdbPendingEvents ();
}

VOID
EFIAPI
DisableInterrupts (
  VOID
  )
{
  AsmDisableInterrupts ();
}

VOID
EFIAPI
EnableDisableInterrupts (
  VOID
  )
{
  AsmEnableInterrupts ();
  RunHvdbPendingEvents ();
  AsmDisableInterrupts ();
}
