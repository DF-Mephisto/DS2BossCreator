.DATA

.CODE

BulletHelper PROC

push rax
push rdx
push rbx

mov rcx, 0
lea rbx, [bulletData]

check:
cmp [rbx], cl
je save

mov rax, 12
mul rcx
mov rdx, rdi
add rdx, 2Fh
cmp qword ptr [rbx+5+rax], rdx
je found
inc rcx
jmp check

found:
inc dword ptr [rbx+1+rax]
jmp originalcode

save:
movzx rcx, byte ptr [bulletData]
mov rax, 12
mul rcx
mov rdx, rdi
add rdx, 2Fh
mov dword ptr [rbx+1+rax], 1
mov qword ptr [rbx+5+rax], rdx
inc byte ptr [rbx]

originalcode:
pop rbx
pop rdx
pop rax
movsx ecx,byte ptr [rdi+2Fh]
cmp [rbx+120h],ecx

nop
nop
nop
nop
nop
bulletData:
ret

BulletHelper ENDP

END