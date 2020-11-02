.DATA

.CODE

DrawingsScript PROC

push rax
push rcx
push rbx
push rdx

mov rax, qword ptr [data]
sub rax, 2Fh
cmp rdi, rax
jne originalcode

movzx rcx, byte ptr [data+8]
cmp byte ptr [data+9], cl
jb mycode
mov byte ptr [data+9], 0

mycode:
movzx rcx, byte ptr [data+9]
inc byte ptr [data+9]

mov rax, 12
mul rcx

lea rdx, [data+10]
mov edx, dword ptr [rax+rdx]
mov rbx, qword ptr [data]
sub rbx, 23h
mov dword ptr [rbx], edx

lea rdx, [data+10]
mov edx, dword ptr [rax+rdx+4]
mov rbx, qword ptr [data]
sub rbx, 27h
mov dword ptr [rbx], edx

lea rdx, [data+10]
mov edx, dword ptr [rax+rdx+8]
mov rbx, qword ptr [data]
sub rbx, 1Fh
mov dword ptr [rbx], edx

originalcode:
pop rdx
pop rbx
pop rcx
pop rax
movss xmm0, dword ptr [rdi+10h]

nop
nop
nop
nop
nop
data:
ret

DrawingsScript ENDP

END