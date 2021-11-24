; TODO: 
; - Add safeguards and support for reading 200+ char lines
; - Handle errors of file read/write fail
; - Handle input file error

; 11 uzduotis

.model small
.stack 100h
.data
	new_line    db 0Dh, 0Ah, 24h
	open_msg    db "Opening file: ", 24h
	success_msg db "Sucessfully generated result.htm file", 0Dh, 0Ah, 24h
	error_msg   db "Expected file name as argument", 0Dh, 0Ah, 24h
	file_name   db 128 dup(?)
	result_name db "result.htm", 00h
	file_data   db 1000 dup(?)
	
	html_data_b  db "<!DOCTYPE html>", 0Dh, 0Ah, "<html><body>", 0Dh, 0Ah
	html_data_e  db "</body></html>", 0Dh, 0Ah
	
	html_br           db "<br>", 0Dh, 0Ah
	html_d_quote      db "<span style=", 22h, "color:#8f897b", 22h, ">"
	html_s_quote      db "<span style=", 22h, "color:#9c9c2c", 22h, ">"
	html_comment      db "<span style=", 22h, "color:#008000", 22h, ">"
	html_directive    db "<span style=", 22h, "color:#0080ff", 22h, ">"
	html_label        db "<span style=", 22h, "color:#760d9e", 22h, ">"
	html_instruction  db "<span style=", 22h, "color:#0000ff", 22h, ">"
	html_register     db "<span style=", 22h, "color:#bfb769", 22h, ">"
	html_number       db "<span style=", 22h, "color:#ff8000", 22h, ">"
	html_span_end     db "</span>"
	
	html_less_than db "&lt;"
	html_more_than db "&gt;"
	html_and       db "&amp;"
	html_tab       db "&emsp;"
	
	; Size of these strings must be updated in "is_token_special" since their sizes are hardcoded there
	instruction_list db "cmp push pop call ret int mov add sub mul div inc dec and or xor not loop ja jae jb jbe jc jcxz je jg jge jl jle jmp jna jnae jnb jnbe jnc jne jng jnge jnl jnle jno jnp jns jnz jo jp jpe jpo js jz "
	register_list    db "ax ah al bx bh bl cx ch cl dx dh dl ds cs ss es sp bp si di ip "
	directive_list   db ".model .stack .data .code offset db dup end "
	
	token_buff     db 255 dup(?)
	line_buff      db 255 dup(?)
	
	; Global variable offset purposes (DON'T FORGET TO INITIALISE NEW VARIABLES)
	;  0: Read file handle
	;  2: How many bytes were read by last read from file operation
	;  4: Read buffer size
	;  6: Write file handle
	;  8: Stores offset to current file buffer (either file_data or file_data2)
	; 10: -
	; 12: Last new line position of previous buffer
	; 14: if > 0, currently in a string
	; 16: if > 0, currently in a comment
	; 18: "_insert_string_to_line_buff" function string address
	; 20: if > 0, currently in a single quote
	; 22: Length of token in token_buff
	; 24: Stores keyword type:
		; - 0 if token didn't match any keywords
		; - 1 if token is directive
		; - 2 if token is label
		; - 3 if token is instruction
		; - 4 if token is register/segment
		; - 5 if token is number
	global_vars dw 255 dup(?)
	
.code

start:
	MOV DX, @data
	mov ds, dx	

	xor cx, cx
	mov cl, es:[80h]
	
	; If user didn't pass any arguments, exit
	cmp cl, 0
	ja _continue
		mov ah, 09h
		mov dx, offset error_msg
		int 21h
		
		mov ax, 4c00h
		int 21h

_continue:
	dec cl ; Decrement, because we ignore first space, so size is smaller by 1
	mov si, 82h
	mov di, offset file_name

	; Read file name
	_read_loop:
		mov ah, es:[si]
		mov ds:[di], ah
		inc si
		inc di
		loop _read_loop

	; Insert zero at end to mark end of file name
	mov ah, 0
	mov ds:[di], ah	

	; Print file name
	mov ah, 09h
	mov dx, offset open_msg
	int 21h
	
	mov ah, 40h
	mov bx, 1
	mov dx, offset file_name
	xor cx, cx
	mov cl, es:[80h]
	dec cl ; Decrement, because we ignore first space, so size is smaller by 1
	int 21h
	
	mov ah, 09h
	mov dx, offset new_line
	int 21h	
	
	; Open file for reading (specified by user)
	mov ah, 3Dh
	mov al, 0                 ; Read only
	mov dx, offset file_name  ; DS:DX Address of ASCIZ filename
	int 21h
	
	mov ds:[global_vars + 0], ax
	mov ds:[global_vars + 4], 255
	
	; Create a file for writing (hard coded in result_name)
	mov ah, 3Ch
	mov cx, 0
	mov dx, offset result_name  ; DS:DX Address of ASCIZ filename
	int 21h
	
	; Open file for writing	(hard coded in result_name)
	mov ah, 3Dh
	mov al, 1                   ; Write only
	mov dx, offset result_name  ; DS:DX Address of ASCIZ filename
	int 21h
	
	mov ds:[global_vars + 6], ax
	mov ds:[global_vars + 8], offset file_data
	mov ax, ds:[global_vars + 4]
	mov ds:[global_vars + 12], ax
	mov ds:[global_vars + 14], 0
	mov ds:[global_vars + 16], 0
	mov ds:[global_vars + 20], 0
	mov ds:[global_vars + 22], 0
	
	xor di, di ; used for line_buff index
	mov si, 255 ; used for file_data buff index.
	
	call print_html_beginning

	_read_and_parse_loop:
		; Read file
		mov ah, 3Fh
		mov bx, ds:[global_vars + 0]  ; File handle
		mov cx, ds:[global_vars + 4]  ; Bytes to read
		mov dx, ds:[global_vars + 8]  ; DS:DX Address of buffer
		int 21h
		
		mov ds:[global_vars + 2], ax  ; Save how many chars read

		call print_with_syntax_highlight
		
		; Check if all file was read
		mov bx, ds:[global_vars + 4]
		mov cx, ds:[global_vars + 2]
		cmp bx, cx
		ja exit
		
		jmp _read_and_parse_loop

print_html_beginning:
	mov cx, 31
	mov ds:[global_vars + 18], offset html_data_b
	call insert_string_to_line_buff
	
	ret
	
print_html_end:
	mov cx, 16
	mov ds:[global_vars + 18], offset html_data_e
	call insert_string_to_line_buff
	
	ret

exit:
	call print_html_end
	call flush_line_buff
	
	; Print success message
	mov ah, 09h
	mov dx, offset success_msg
	int 21h
	
	mov ax, 4c00h
	int 21h


; Expects that you don't modify "di" in between calls
; Function
print_with_syntax_highlight:
	xor si, si
	_read_buffer_loop:
		mov ax, ds:[global_vars + 2]
		mov bx, si
		cmp ax, bx
		je _print_with_syntax_highlight_end
		
		; Load char from file buffer
		mov bx, ds:[global_vars + 8]
		mov dh, ds:[bx + si]
		
		call place_char_to_line_buff
		
		inc si

		jmp _read_buffer_loop

	_print_with_syntax_highlight_end:
		ret


; Function
flush_line_buff:
	mov ah, 40h
	mov bx, ds:[global_vars + 6]  ; File handle
	mov cx, di                    ; How many chars to print
	mov dx, offset line_buff
	int 21h
	
	xor di, di
	
	ret


; Function
insert_string_to_line_buff:
	mov ax, di
	add ax, cx
	cmp ax, 255 ; Hardcoded line buffer size
	jb _insert_line
		push cx
		call flush_line_buff
		pop cx
	_insert_line:
		push si
		xor si, si
		_replace_loop:
			mov bx, ds:[global_vars + 18]
			mov dh, ds:[bx + si]
			
			mov bx, offset line_buff
			mov ds:[bx + di], dh
			inc di
			
			inc si
			
			loop _replace_loop
		pop si

	; call flush_line_buff
	ret


; Sets flag in "ds:[global_vars + 24]" as
; - 0 if token didn't match any keywords
; - 1 if token is directive
; - 2 if token is label
; - 3 if token is instruction
; - 4 if token is register/segment
; - 5 if token is number
; Function
is_token_special:
	push si
	push di
	mov ds:[global_vars + 24], 0  ; Reset return value
	
	_check_if_label:
		mov bx, offset token_buff
		mov di, ds:[global_vars + 22]
		dec di
		mov cl, ds:[bx + di]
		cmp cl, ':'
		jne _check_if_number
			mov ds:[global_vars + 24], 2
			pop di
			pop si
			ret
	
	_check_if_number:
		mov bx, offset token_buff
		mov cl, ds:[bx]
		sub cl, '0'
		cmp cl, 9
		ja _check_if_directive
			mov ds:[global_vars + 24], 5
			pop di
			pop si
			ret
	
	_check_if_directive:
		xor si, si ; Memory list index
		xor di, di ; Token index
		mov dl, 1  ; Flag that tracks if letters match
		mov dh, 0  ; Number that tracks keyword length
		_check_if_directive_loop:
			mov ax, ds:[global_vars + 22]
			inc ax  ; We need loop to run +1 time
			cmp di, ax
			je _directive_keyword_didnt_match
				cmp si, 44  ; Hardcoded "directive_list" string size
				je _check_if_instruction
					mov bx, offset directive_list
					mov ah, ds:[bx + si]
					cmp ah, ' '
					je _directive_keyword_ended
						mov bx, offset token_buff
						mov cl, ds:[bx + di]
						or cl, 32   ; Convert to lower case
						cmp ah, cl
						je _directive_letter_matches
							mov dl, 0
						_directive_letter_matches:
							inc dh
							inc di
							inc si
							jmp _check_if_directive_loop
					_directive_keyword_ended:
						cmp dl, 0
						je _directive_keyword_didnt_match
							mov ax, ds:[global_vars + 22]
							cmp ax, di ; Token now can only match if di == token buff size
							jne _directive_keyword_didnt_match
								mov ax, di
								cmp ax, di  ; Check if keyword and token lengths are the same
								jne _directive_keyword_didnt_match
									mov ds:[global_vars + 24], 1  ; Keyword matched!
									jmp _check_if_instruction
									pop di
									pop si
									ret
						_directive_keyword_didnt_match:
							xor di, di
							inc si
							xor dh, dh
							mov dl, 1
							jmp _check_if_directive_loop
	
	_check_if_instruction:
	xor si, si ; Memory list index
	xor di, di ; Token index
	mov dl, 1  ; Flag that tracks if letters match
	mov dh, 0  ; Number that tracks keyword length
	_check_if_instruction_loop:
		mov ax, ds:[global_vars + 22]
		inc ax  ; We need loop to run +1 time
		cmp di, ax
		je _instruction_keyword_didnt_match
			cmp si, 197  ; Hardcoded "instruction_list" string size
			je _check_if_register
				mov bx, offset instruction_list
				mov ah, ds:[bx + si]
				cmp ah, ' '
				je _instruction_keyword_ended
					mov bx, offset token_buff
					mov cl, ds:[bx + di]
					or cl, 32   ; Convert to lower case
					cmp ah, cl
					je _instruction_letter_matches
						mov dl, 0
					_instruction_letter_matches:
						inc dh
						inc di
						inc si
						jmp _check_if_instruction_loop
				_instruction_keyword_ended:
					cmp dl, 0
					je _instruction_keyword_didnt_match
						mov ax, ds:[global_vars + 22]
						cmp ax, di ; Token now can only match if di == token buff size
						jne _instruction_keyword_didnt_match
							mov ax, di
							cmp ax, di  ; Check if keyword and token lengths are the same
							jne _instruction_keyword_didnt_match
								mov ds:[global_vars + 24], 3  ; Keyword matched!
								jmp _check_if_register
								pop di
								pop si
								ret
					_instruction_keyword_didnt_match:
						xor di, di
						inc si
						xor dh, dh
						mov dl, 1
						jmp _check_if_instruction_loop
	
	_check_if_register:
	xor si, si ; Memory list index
	xor di, di ; Token index
	mov dl, 1  ; Flag that tracks if letters match
	mov dh, 0  ; Number that tracks keyword length
	_check_if_register_loop:
		mov ax, ds:[global_vars + 22]
		inc ax  ; We need loop to run +1 time
		cmp di, ax
		je _register_keyword_didnt_match
			cmp si, 63  ; Hardcoded "register_list" string size
			je _is_token_special_end
				mov bx, offset register_list
				mov ah, ds:[bx + si]
				cmp ah, ' '
				je _register_keyword_ended
					mov bx, offset token_buff
					mov cl, ds:[bx + di]
					or cl, 32   ; Convert to lower case
					cmp ah, cl
					je _register_letter_matches
						mov dl, 0
					_register_letter_matches:
						inc dh
						inc di
						inc si
						jmp _check_if_register_loop
				_register_keyword_ended:
					cmp dl, 0
					je _register_keyword_didnt_match
						; Token now can only match if di == token buff size
						mov ax, ds:[global_vars + 22]
						cmp ax, di
						jne _register_keyword_didnt_match
							mov ax, di
							cmp ax, di  ; Check if keyword and token lengths are the same
							jne _instruction_keyword_didnt_match
								mov ds:[global_vars + 24], 4  ; Keyword matched!
								jmp _is_token_special_end
								pop di
								pop si
								ret
					_register_keyword_didnt_match:
						xor di, di
						inc si
						xor dh, dh
						mov dl, 1
						jmp _check_if_register_loop

_is_token_special_end:
	pop di
	pop si
	ret
	

; Function
flush_token_buff:
	call is_token_special
	
	mov ax, ds:[global_vars + 24]
	
	_check_if_token_is_directive:
		cmp ax, 1
		jne _check_if_token_is_label
			mov cx, 28
			mov ds:[global_vars + 18], offset html_directive
			call insert_string_to_line_buff
			jmp _flush_token
	
	_check_if_token_is_label:
		cmp ax, 2
		jne _check_if_token_is_instruction
			mov cx, 28
			mov ds:[global_vars + 18], offset html_label
			call insert_string_to_line_buff
			jmp _flush_token
	
	_check_if_token_is_instruction:
		cmp ax, 3
		jne _check_if_token_is_register
			mov cx, 28
			mov ds:[global_vars + 18], offset html_instruction
			call insert_string_to_line_buff
			jmp _flush_token
	
	_check_if_token_is_register:
		cmp ax, 4
		jne _check_if_token_is_number
			mov cx, 28
			mov ds:[global_vars + 18], offset html_register
			call insert_string_to_line_buff
			jmp _flush_token
	
	_check_if_token_is_number:
		cmp ax, 5
		jne _flush_token
			mov cx, 28
			mov ds:[global_vars + 18], offset html_number
			call insert_string_to_line_buff
			jmp _flush_token

_flush_token:
	mov cx, ds:[global_vars + 22]
	mov ds:[global_vars + 18], offset token_buff
	call insert_string_to_line_buff
	
	mov ds:[global_vars + 22], 0  ; Reset token buff index

	mov ax, ds:[global_vars + 24]
	cmp ax, 0
	je _flush_token_buff_end
		mov cx, 7
		mov ds:[global_vars + 18], offset html_span_end
		call insert_string_to_line_buff
	
_flush_token_buff_end:
	ret


; Function
add_char_to_token_buff:
	mov bx, offset token_buff
	add bx, ds:[global_vars + 22]
	mov ds:[bx], dh
	inc ds:[global_vars + 22]

	ret


; Function
place_char_to_line_buff:
	; Acceptable symbols for tokens: "a-z, A-Z, '_', '.', ':'"
	_check_if_token_char:
		; Skip if inside comment or single/double quote
		mov ax, ds:[global_vars + 14]
		add ax, ds:[global_vars + 16]
		add ax, ds:[global_vars + 20]
		cmp ax, 0
		ja _check_if_less_than
			cmp dh, '_'
			je _is_token_char
				cmp dh, '.'
				je _is_token_char
					cmp dh, ':'
					je _is_token_char
						; Check if char is number
						mov ah, dh
						sub ah, '0'
						cmp ah, 9 ; Total 9 digits exist
						jbe _is_token_char
							; Check if char is letter
							mov ah, dh
							or ah, 32   ; Convert to lower case
							sub ah, 'a'
							cmp ah, 25 ; Total 25 letters exist
							jbe _is_token_char
								mov bx, ds:[global_vars + 22]
								cmp bx, 0
								je _dont_flush_token_buff
									push dx
									call flush_token_buff
									pop dx
									
								_dont_flush_token_buff:
									jmp _check_if_less_than
				
			_is_token_char:
				call add_char_to_token_buff
				ret

	_check_if_less_than:
		cmp dh, '<'
		jne _check_if_more_than
		
		mov cx, 4
		mov ds:[global_vars + 18], offset html_less_than
		call insert_string_to_line_buff
		ret
	
	_check_if_more_than:
		cmp dh, '>'
		jne _check_if_and
		
		mov cx, 4
		mov ds:[global_vars + 18], offset html_more_than
		call insert_string_to_line_buff
		ret
		
	_check_if_and:
		cmp dh, '&'
		jne _check_if_tab
		
		mov cx, 5
		mov ds:[global_vars + 18], offset html_and
		call insert_string_to_line_buff
		ret
		
	_check_if_tab:
		cmp dh, 09h
		jne _check_if_single_quote
		
		mov cx, 6
		mov ds:[global_vars + 18], offset html_tab
		call insert_string_to_line_buff
		ret
	
	_check_if_single_quote:
		cmp dh, 27h
		jne _check_if_double_quote
		
		; Skip if inside comment or double quote
		mov ax, ds:[global_vars + 14]
		add ax, ds:[global_vars + 16]
		cmp ax, 0
		ja _check_if_double_quote
		
			; Check if quote already started
			mov ax, ds:[global_vars + 20]
			cmp ax, 0
			ja _end_s_quote
				mov cx, 28
				mov ds:[global_vars + 18], offset html_s_quote
				call insert_string_to_line_buff
				
				mov ds:[global_vars + 20], 1  ; Set single quote flag
				
				mov dh, 27h
				call _default_insert
				
				ret
			
			_end_s_quote:
				mov dh, 27h
				call _default_insert
				
				mov cx, 7
				mov ds:[global_vars + 18], offset html_span_end
				call insert_string_to_line_buff
				
				mov ds:[global_vars + 20], 0  ; Reset single quote flag
				
				ret
	
	_check_if_double_quote:
		cmp dh, '"'
		jne _check_if_comment
		
		; Skip if inside comment or single quote
		mov ax, ds:[global_vars + 16]
		add ax, ds:[global_vars + 20]
		cmp ax, 0
		ja _check_if_comment
		
			; Check if quote already started
			mov ax, ds:[global_vars + 14]
			cmp ax, 0
			ja _end_d_quote
				mov cx, 28
				mov ds:[global_vars + 18], offset html_d_quote
				call insert_string_to_line_buff
				
				mov ds:[global_vars + 14], 1  ; Set quote flag
				
				mov dh, 22h
				call _default_insert
				
				ret
			
			_end_d_quote:
				mov dh, 22h
				call _default_insert
				
				mov cx, 7
				mov ds:[global_vars + 18], offset html_span_end
				call insert_string_to_line_buff
				
				mov ds:[global_vars + 14], 0  ; Reset quote flag
				
				ret
	
	_check_if_comment:
		cmp dh, ';'
		jne _check_if_new_line
		
		; Skip if inside single/double quote
		mov ax, ds:[global_vars + 14]
		add ax, ds:[global_vars + 20]
		cmp ax, 0
		ja _check_if_new_line
		
			; Only change colour, if comment hasn't started
			mov ax, ds:[global_vars + 16]
			cmp ax, 0
			ja _check_if_new_line
			
				mov ds:[global_vars + 16], 1 ; Set comment flag
				
				mov cx, 28
				mov ds:[global_vars + 18], offset html_comment
				call insert_string_to_line_buff
				
				mov dh, ';'
				call _default_insert
				
				ret
		
	_check_if_new_line:
		cmp dh, 0Ah
		jne _default_insert
		
		call _default_insert
		
		; If there was a comment, single quote or double quote on that line, add end span
		mov ax, ds:[global_vars + 14]
		add ax, ds:[global_vars + 16]
		add ax, ds:[global_vars + 20]
		cmp ax, 0
		je _check_if_new_line_end
			mov cx, 7
			mov ds:[global_vars + 18], offset html_span_end
			call insert_string_to_line_buff

		_check_if_new_line_end:
			mov cx, 6
			mov ds:[global_vars + 18], offset html_br
			call insert_string_to_line_buff
			
			mov ds:[global_vars + 12], si ; Reset line buffer index
			mov ds:[global_vars + 14], 0  ; Reset double quote flag
			mov ds:[global_vars + 16], 0  ; Reset comment flag
			mov ds:[global_vars + 20], 0  ; Reset single quote flag
			ret

	_default_insert:
		mov cx, 1
		mov bx, offset file_data
		add bx, si
		mov ds:[global_vars + 18], bx
		call insert_string_to_line_buff
		ret

end start
