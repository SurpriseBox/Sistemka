org 0x7c00
mov al,50
mov cl,2
mov bx,0x8000
call read
mov ax,0xE801
int 0x15
mov [0xD000],ax
mov [0xD002],dx
mov ah,0x15
mov dl,0x80
int 0x13
mov [0xD004],ah


call initVesaMode

mov ax,0
mov es,ax
mov fs,ax
mov ss,ax
mov gs,ax
mov ds,ax
mov sp,0xfffc

cli

lgdt[gdtable]

mov eax,cr0
or eax,1
mov cr0,eax


mov esi,[lfb_address]
jmp CODE_D: 0x8000
ret



initVesaMode:
	mov ax,0x4f01
	mov cx,0x11b ;1024x768 24 bit
	mov [0xD006], cx
	;читаю и нфу о видеодапторе о режиме 118.
	mov di,info_table
	int 0x10

	mov ax,0x4f02
	mov bx,0x411b ; четверка говорит, что я устанавливаю 118 режим для веза
	int 0x10

	mov eax,dword[info_table+0x28]
	mov dword[lfb_address],eax
	
ret





descriptors_begin:
null_descriptor:
dd 0
dd 0
code_descriptor:
dw 0xffff
dw 0
db 0
db 0b10011010
db 0b11001111
db 0
data_descriptor:
dw 0xffff
dw 0
db 0
db 0b10010010
db 0b11001111
db 0
descriptors_end:

gdtable:
dw descriptors_end-descriptors_begin
dd descriptors_begin
read:
	mov ah,2
	mov ch,0
	mov dh,0
	mov dl, 0x80
	int 0x13
	ret

CODE_D equ code_descriptor-descriptors_begin
DATA_D equ data_descriptor-descriptors_begin

info_table dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
lfb_address dd 0

