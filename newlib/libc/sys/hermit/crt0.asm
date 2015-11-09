; Copyright (c) 2012, Stefan Lankes, RWTH Aachen University
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;    * Redistributions of source code must retain the above copyright
;      notice, this list of conditions and the following disclaimer.
;    * Redistributions in binary form must reproduce the above copyright
;      notice, this list of conditions and the following disclaimer in the
;      documentation and/or other materials provided with the distribution.
;    * Neither the name of the University nor the names of its contributors
;      may be used to endorse or promote products derived from this
;      software without specific prior written permission.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY
; DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
; ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;

[BITS 64]
SECTION .text
global _start
extern main
extern environ
extern __env
extern _init
extern _fini
extern _hermit_reent_init
extern atexit
extern exit
extern optind
phys equ 0x40200000
_start:
   ; create first frame
   xor rbp, rbp
   push rbp ; pseudo return address
   push rbp ; pseudo base pointer
   mov rbp, rsp
   and rsp, ~0xF ; align rsp

   ; initialize libc
   call _hermit_reent_init

   ; register a function to be called at normal process termination
   mov rdi, _fini
   call atexit

   ; call init function
   call _init

   ; optind is the index of the next element to be processed in argv
   mov dword [optind], 0

   ; set default environment
   mov rax, environ
   mov rdx, [rsp+32]
   cmp rdx, 0
   je L3
   mov qword [rax], rdx
   jmp L4
L3:
   mov rdx, __env
   mov qword [rax], rdx
L4:

   ; arguments are already on the stack
   ; call the user's function
   mov rdi, [rbp+16]   ; argc
   mov rsi, [rbp+24]   ; argv pointer
   mov rdx, [rbp+32]   ; env pointer

   call main

   ; call exit from the C library so atexit gets called, and the
   ; C++ destructors get run. This calls our exit routine below    
   ; when it's done.
   ; call "exit"
   mov rdi, rax
   call exit

   ; endless loop
   jmp $

extern _ftext
global get_cpufreq
get_cpufreq:
   mov rcx, phys - 0x1000 + 0x18
   xor rax, rax
   mov eax, dword [rcx]
   ret

global isle_id
isle_id:
   mov rcx, phys - 0x1000 + 0x34
   xor rax, rax
   mov eax, dword [rcx]
   ret

global get_num_cpus
get_num_cpus:
   mov rcx, phys - 0x1000 + 0x20
   xor rax, rax
   mov eax, dword [rcx]
   ret

SECTION .note.ident
ALIGN 4
DD Ln1 - Ln0          ; name size (not including padding)
DD 0                  ; desc size (not including padding)
DD 0x01               ; type
Ln0:
DB "HermitCore", 0    ; name
Ln1:
ALIGN 4

SECTION .note.GNU-stack noalloc noexec nowrite progbits
