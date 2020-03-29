.386
.model flat, stdcall

EXTRN ExitProcess@4:PROC
EXTRN GetStdHandle@4:PROC
EXTRN WriteFile@20:PROC

.data
msg db 'Hello, world!', 10, 0
bytesWritten dd 0

.code
start proc
	push -11 ; STD_OUTPUT_HANDLE
	call GetStdHandle@4

	push 0 ; lpOverlapped
	push offset bytesWritten ; lpNumberOfBytesWritten
	push 14 ; nNumberOfBytesToWrite
	push offset msg ; lpBuffer
	push eax ; hFile
	call WriteFile@20

	push 0
	call ExitProcess@4
start endp
End
