.386
.model flat, stdcall

EXTRN ExitProcess@4:PROC

.code
start proc
	push 0
	call ExitProcess@4
start endp
End
