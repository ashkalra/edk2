/** @file
  X64 #HV Exception Handler functon.

  Copyright (C) 2021, Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemEncryptSevLib.h>
#include <Library/VmgExitLib.h>
#include <Register/Amd/Msr.h>

#include "VmgExitHvHandler.h"

/**
  Handle a #HV exception.

  Performs the necessary processing to handle a #HV exception.

  The base library function returns an error equal to HV_EXCEPTION,
  to be propagated to the standard exception handling stack.

  @param[in]       SevEsData      Pointer to the per-CPU data
  @param[in, out]  ExceptionType  Pointer to an EFI_EXCEPTION_TYPE to be set
                                  as value to use on error.
  @param[in, out]  SystemContext  Pointer to EFI_SYSTEM_CONTEXT

  @retval  EFI_SUCCESS            Exception handled
  @retval  EFI_UNSUPPORTED        #HV not supported, (new) exception value to
                                  propagate provided
  @retval  EFI_PROTOCOL_ERROR     #HV handling failed, (new) exception value to
                                  propagate provided

**/
EFI_STATUS
EFIAPI
InternalVmgExitHandleHv (
  IN     SEV_ES_PER_CPU_DATA  *SevEsData,
  IN OUT EFI_EXCEPTION_TYPE   *ExceptionType,
  IN OUT EFI_SYSTEM_CONTEXT   SystemContext
  )
{
  EFI_SYSTEM_CONTEXT_X64  *Regs;
  IA32_EFLAGS32           Rflags;
  HVDB                    *Hvdb;
  UINT16                  *PendingEvents;
  HVDB_EVENTS             Events;

  Regs = SystemContext.SystemContextX64;

  Hvdb = SevEsData->Hvdb;
  ASSERT (Hvdb != NULL);

  SevEsData->HvdbPendingEvent = TRUE;

  Rflags.UintN = Regs->Rflags;
  if (!Rflags.Bits.IF) {
    //
    // Interrupts are disabled, only process non-maskable events
    //
    if (Hvdb->Events.PendingEvents.Bits.NmiRequested) {
    }

    if (Hvdb->Events.PendingEvents.Bits.MceRequested) {
    }

    Hvdb->Events.PendingEvents.Bits.NoFurtherSignal = 0;

    return EFI_SUCCESS;
  }

  PendingEvents = &Hvdb->Events.PendingEvents.Uint16;
  Events.PendingEvents.Uint16 = 0;
  asm volatile ("xchgw %w0, %1\n"
		: "+r" (Events.PendingEvents.Uint16), "+m" (*PendingEvents)
		: : "memory", "cc");

  if (Events.PendingEvents.Bits.NmiRequested) {
  }

  if (Events.PendingEvents.Bits.MceRequested) {
  }

  SevEsData->HvdbPendingEvent = FALSE;

  //
  // Return the supplied vector to have the exception handler
  // infrastructure process it. No vector will look like EFI_SUCCESS
  //
  *ExceptionType = Events.PendingEvents.Fields.Vector;

  return EFI_PROTOCOL_ERROR;
}

/**
  Routine to allow ASSERT from within #HV.

  @param[in]       SevEsData  Pointer to the per-CPU data

**/
VOID
EFIAPI
VmgExitHvIssueAssert (
  IN     SEV_ES_PER_CPU_DATA  *SevEsData
  )
{
  ASSERT (FALSE);
  CpuDeadLoop ();
}
