DEFAULT REL
SECTION .text

; VOID
; EFIAPI
; SetMemoryEncDecHypercall3AsmStub (
;   IN UINT HypercallNum,
;   IN INTN Arg1,
;   IN INTN Arg2,
;   IN INTN Arg3
;   );
global ASM_PFX(SetMemoryEncDecHypercall3AsmStub)
ASM_PFX(SetMemoryEncDecHypercall3AsmStub):
  ; UEFI calling conventions require RBX to
  ; be nonvolatile/callee-saved.
  push rbx
  ; Copy HypercallNumber to rax
  mov rax, rcx
  ; Copy Arg1 to the register expected by KVM
  mov rbx, rdx
  ; Copy Arg2 to register expected by KVM
  mov rcx, r8
  ; Copy Arg2 to register expected by KVM
  mov rdx, r9
  ; Call VMMCALL
  vmmcall
  pop rbx
  ret
