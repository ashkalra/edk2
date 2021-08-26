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
VmgExitHandleHv (
  IN OUT EFI_EXCEPTION_TYPE  *ExceptionType,
  IN OUT EFI_SYSTEM_CONTEXT  SystemContext
  )
{
  UINT64                    HvFeatures;
  UINT64                    HvSupport;
  MSR_SEV_ES_GHCB_REGISTER  Msr;
  GHCB                      *Ghcb;
  SEV_ES_PER_CPU_DATA       *SevEsData;
  EFI_STATUS                HvRet;

  HvFeatures = PcdGet64 (PcdGhcbHypervisorFeatures);

  HvSupport = HvFeatures & GHCB_HV_FEATURES_SNP_RESTRICTED_INJECTION;
  if (HvSupport != GHCB_HV_FEATURES_SNP_RESTRICTED_INJECTION) {
    *ExceptionType = HV_EXCEPTION;
    HvRet = EFI_UNSUPPORTED;
  }

  Msr.GhcbPhysicalAddress = AsmReadMsr64 (MSR_SEV_ES_GHCB);
  ASSERT (Msr.GhcbInfo.Function == 0);
  ASSERT (Msr.Ghcb != 0);

  Ghcb = Msr.Ghcb;
  SevEsData = (SEV_ES_PER_CPU_DATA *) (Ghcb + 1);

  HvRet = InternalVmgExitHandleHv (SevEsData, ExceptionType, SystemContext);

  return HvRet;
}
