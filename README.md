# m0leConOS

## m0leCon CTF 2023 Teaser challenge

[m0leCon CTF 2023 Teaser](https://ctftime.org/event/1898) is an online jeopardy-style CTF organized by [pwnthem0le](https://pwnthem0le.polito.it/). The top 10 teams will be invited to the final event, which will take place in Fall 2023 at Politecnico di Torino.

### Description
The challenge basically simulates a shell and gives the user the possibility to create, copy and delete files. The m0lecat program loaded into the shell allows to print the content of such in-memory files created with the shell or files actually present in the file system

The original flag has been substituted by `ptm{sample_flag}`

### Deploy
Both the `Dockerfile` and the `docker-compose.yaml` files are provided to launch the server through [Docker](https://www.docker.com/) on port `6745`<br>

## Solution
Vulnerability:
The client has the ability to exploit a use-after-free bug related to the logic of the `rm` and `ln` operations combined. While the `cp` operation actually creates a new file in memory with the same content of the original one, the `ln` command only writes the pointer to the file into another position of the file system array. The `rm` operation doesn't handle correctly the possibility of such *hard links*.

It is possible to get a file which points to the same memory location of the first position in the file system array, which is dedicated to the executable `m0lecat`. 

The client can write a shellcode to the executable location, spawning a shell to retrieve the `./flag.txt` file

### Order of execution of the operations:
1.  `touch tmp`
2.  `ln m0lecat tmp2`
3.  `rm tmp`
4.  `rm tmp2`
5.  `touch tmp` (*inject shellcode*)
6.  `m0lecat tmp` (*run shellcode*)

#### **Unintended solution**
A mistake on the `run_executable()` function allowed to exploit a bug and retrieve the flag, due to the fact that every real file that gets opened for print is not actually removed from the program virtual mapped memory:

1. `m0lecat flag.txt` <br>
   This will load the flag file into memory, but a first check doesn't allow it to be printed

2. `m0lecat /proc/self/mem` <br>
   This will print the memory of the program, now containing the flag. The check on the flag format is someway passed in this case and the flag gets printed



The python script `solve.py` should be able to recover automatically the flag.