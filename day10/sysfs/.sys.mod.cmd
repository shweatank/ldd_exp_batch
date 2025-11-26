savedcmd_sys.mod := printf '%s\n'   sys.o | awk '!x[$$0]++ { print("./"$$0) }' > sys.mod
