savedcmd_/home/pi/Documents/pavan/uart_hello.mod := printf '%s\n'   uart_hello.o | awk '!x[$$0]++ { print("/home/pi/Documents/pavan/"$$0) }' > /home/pi/Documents/pavan/uart_hello.mod
