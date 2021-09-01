;------------------------------------------------------------------------------
;
; Copyright (c) 2006, Intel Corporation. All rights reserved.<BR>
; SPDX-License-Identifier: BSD-2-Clause-Patent
;
; Module Name:
;
;   AsmEnableInterrupts.Asm
;
; Abstract:
;
;   AsmEnableInterrupts function
;
; Notes:
;
;------------------------------------------------------------------------------

    DEFAULT REL
    SECTION .text

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; AsmEnableInterrupts (
;   VOID
;   );
;------------------------------------------------------------------------------
global ASM_PFX(AsmEnableInterrupts)
ASM_PFX(AsmEnableInterrupts):
    sti
    ret

