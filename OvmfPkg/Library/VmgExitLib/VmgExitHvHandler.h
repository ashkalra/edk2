/** @file
  X64 #HV Exception Handler functon header file.

  Copyright (C) 2021, Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __VMG_EXIT_HV_HANDLER_H__
#define __VMG_EXIT_HV_HANDLER_H__

#include <Base.h>
#include <Uefi.h>
#include <Library/VmgExitLib.h>

/**
  Handle a #HV exception.

  Performs the necessary processing to handle a #HV exception.

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
  );

/**
  Routine to allow ASSERT from within #HV.

  @param[in]       SevEsData  Pointer to the per-CPU data

**/
VOID
EFIAPI
VmgExitHvIssueAssert (
  IN     SEV_ES_PER_CPU_DATA   *SevEsData
  );

#endif
