section	.rodata			; we define (global) read-only variables in .rodata section
	format_string: db "%s", 10, 0	; format string

section .bss			; we define (global) uninitialized variables in .bss section
	an: resb 12		; enough to store integer in [-2,147,483,648 (-2^31) : 2,147,483,647 (2^31-1)]
	

section .text
	global convertor
	extern printf

convertor:
	push ebp
	mov ebp, esp	
	pushad			

	mov ecx, dword [ebp+8]	; get function argument (pointer to string)
		
	; your code comes here...
	mov ebx,0
	mov edi,0
	mov edx,0
	mov eax,0
	mov esi,0
	mov byte[an],0		;empty an from prev runs
start:	
	cmp byte[ecx],0x0A		;check if we in the end of the string
	jz middel
	mov ebx,10
	mul ebx
	movzx ebx, byte[ecx]
	sub ebx, '0'
	add eax, ebx
	inc ecx			; get the next char
	jmp start
middel:
	push -1 ;
	mov ebx,16
	cmp  eax,0		; if we got only 0 as number
	jz  continue_hex
to_hex:
	mov edx,0
	cmp  eax,0		; if it's the end of the number
	jz  flip
	div ebx
	cmp edx,9	
	jng continue_hex
	add edx,7
continue_hex:
	add edx,'0'
	push edx
	jmp to_hex
flip:
	pop edx
	cmp edx,-1
	jz end
	mov [an+esi], edx		;insert digit to an 
	inc esi		; increment the counter
	jmp flip

end:
	push an			; call printf with 2 arguments -  
	push format_string	; pointer to str and pointer to format string
	call printf
	add esp, 8		; clean up stack after call
	
		
	popad			
	mov esp, ebp	
	pop ebp
	ret
