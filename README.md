# Container-c
Program in c for creating a container that isolate an environment for a child process,
making the right use of limit of resources and owns environment within the host machine

Using namespaces and cgroups this small container in C simulate what Docker does .

# Bash console inside the program 
This program create a child process with a sh console , simulating a small OS inside the 
current process , the environment variables , hostname and filesystem are isolate from 
the father .

# CGroups
With cgroups tools the maximum and minimum amount of memory can be 
modifiable as well as others resources as number of process that child 
can create . 
