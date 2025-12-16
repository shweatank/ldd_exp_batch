savedcmd_/home/pi/Documents/pavan/hello_test.mod := printf '%s\n'   hello_test.o | awk '!x[$$0]++ { print("/home/pi/Documents/pavan/"$$0) }' > /home/pi/Documents/pavan/hello_test.mod
