#!/bin/bash

socat -T60 TCP-LISTEN:6745,reuseaddr,fork,su=pwn EXEC:$1,pty,raw,stderr,echo=0