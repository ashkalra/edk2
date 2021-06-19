/** @file
  X64 #HV Exception Handler functon.

  Copyright (C) 2021, Advanced Micro Devices, Inc. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#include <Base.h>
#include <Uefi.h>
#include <Library/VmgExitLib.h>

/**
  Handle a #HV exception.

  Performs the necessary processing to handle a #HV exception.

  The base library function returns an error equal to HV_EXCEPTION,
  to be propagated to the standard exception handling stack.

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
  *ExceptionType = HV_EXCEPTION;

  return EFI_UNSUPPORTED;
}
