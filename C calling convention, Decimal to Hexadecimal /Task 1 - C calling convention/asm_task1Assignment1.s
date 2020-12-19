section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "%d", 10, 0	; format string

section .bss			; we define (global) uninitialized variables in .bss section
	an: resb 12		; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]

section .text
	global assFunc
	extern printf
	extern c_checkValidity

assFunc:
	push ebp
	mov ebp, esp				
	pushad

	sub esp, 4 ; allocate space for local variable sum and for the answer from vaild function
	mov ebx, [ebp+8] ; get first argument
	mov ecx, [ebp+12] ; get second argument

	;now we check what to do
	push ecx		;saves the arguments in littel indian way for c function
	push ebx		
	call c_checkValidity
	cmp eax, '0'		;means we need to Add numbers
	jz addNumbers
	sub ebx, ecx
	push ebx		;for printf
	push format_string
	call printf 
	add esp, 20		; clean up stack after call
	popad			
	mov esp, ebp	
	pop ebp
	ret
addNumbers:
	add ebx, ecx
	push ebx		;for printf
	push format_string
	call printf
	add esp, 20		; clean up stack after call	

	popad		
	mov esp, ebp	
	pop ebp
	ret
