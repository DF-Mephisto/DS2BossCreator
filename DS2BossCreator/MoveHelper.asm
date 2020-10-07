.DATA

.CODE

MoveHelper PROC

push rax
push rdx
push rbx

mov rcx, 0
lea rbx, [moveData]

check:
cmp [rbx], cl
je save

mov rax, 12
mul rcx
mov rdx, r15
cmp qword ptr [rbx+5+rax], rdx
je found
inc rcx
jmp check

found:
inc dword ptr [rbx+1+rax]
jmp originalcode

save:
movzx rcx, byte ptr [moveData]
mov rax, 12
mul rcx
mov rdx, r15
mov dword ptr [rbx+1+rax], 1
mov qword ptr [rbx+5+rax], rdx
inc byte ptr [rbx]

originalcode:
pop rbx
pop rdx
pop rax
mov ecx,[r15]
mov rdi,[rsp+78h]

nop
nop
nop
nop
nop
moveData:
ret

MoveHelper ENDP

END