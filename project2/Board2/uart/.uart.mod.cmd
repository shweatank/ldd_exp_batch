savedcmd_/home/pi/Project/uart/uart.mod := printf '%s\n'   uart.o | awk '!x[$$0]++ { print("/home/pi/Project/uart/"$$0) }' > /home/pi/Project/uart/uart.mod
