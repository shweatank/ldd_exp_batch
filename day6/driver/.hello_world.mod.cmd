savedcmd_hello_world.mod := printf '%s\n'   hello_world.o | awk '!x[$$0]++ { print("./"$$0) }' > hello_world.mod
