;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   AsmDisableInterrupts.Asm
;
; Abstract:
;
;   AsmDisableInterrupts function
;
; Notes:
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; AsmDisableInterrupts (
;   VOID
;   );
;------------------------------------------------------------------------------
global ASM_PFX(AsmDisableInterrupts)
ASM_PFX(AsmDisableInterrupts):
    cli
    ret

