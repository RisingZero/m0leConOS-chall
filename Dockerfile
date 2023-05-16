FROM ubuntu:22.04

RUN apt update && apt upgrade -y
RUN apt install -y socat
RUN apt install -y libssl-dev

RUN useradd -ms /bin/sh pwn

WORKDIR /home/pwn

COPY ./dist/m0leConOS ./
COPY ./entrypoint.sh ./
COPY ./m0lecat ./
COPY ./flag.txt ./

RUN \
    chown -R root:pwn /home/pwn && \
    chmod 750 /home/pwn && \
    chown root:pwn /home/pwn/flag.txt && \
    chmod 440 /home/pwn/flag.txt && \
    chmod 550 /home/pwn/entrypoint.sh && \
    chmod 440 /home/pwn/m0lecat && \
    chmod 550 /home/pwn/m0leConOS

EXPOSE 1337

CMD [ "/home/pwn/entrypoint.sh", "/home/pwn/m0leConOS" ]
