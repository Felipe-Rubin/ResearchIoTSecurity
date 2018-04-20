# ResearchIoTSecurity
Study about IoT Security for the "Integradora 2" class.

# Credits

https://github.com/prplfoundation/prpl-hypervisor

# Normal

- The master branch one

# Secure

- The one with Secure Boot, branch is secure-boot-vm

# Download Submodules:
- git submodule init

- git submodule update


# Additional Step to run shared-ethernet.cfg
- Additional step required which is not written in the cfg file: On the makefile, you also need to comment the line "CONFIG_PIC32MZ_ETHERNET_DRV = yes".
- Side Note: deploying shared-ethernet is slower than normal, maybe because of the config file
- Side Note 2: shared ethernet has a bug, which makes none/one/both of the VM's not work properly. A fix was not researched thoroughly.

# Using Parallels Desktop:
- Need to configure another Network Interface to share Ethernet.

# Common Mistakes:
- Do not open the comm interface (/dev/ttyAC0) before fully finishing make load

# Secure-Boot-VM Additional Step:
- git checkout secure-boot-vm
- ...
- After executing make, copy the two generated file keys (public and private) to ~/temp
- Not sure if is ~/temp (at home) or just /temp (top level)

# Secure-Boot-VM getting the execution time:
- Open the file located at prpl-hypervisor/cryptography/security.c
- Uncomment all the print lines at the bottom of the file.
- Keep in mind that the values are printed as cicles
-  ns  = cicles/100000
- Side Note: What makes it generate the hash generation and verification faster or slower is the Flash size of the VM (which is configured at each cfg file, for each different VM).

# Remember this:
## sudo ifconfig enp0s6:1 192.168.0.1
- My en interface while using parallels is enp0s6. To use applications which requires to set an ip at the code, this is required to telnet/ping/access webpage of the VM running them.

## sudo ifconfig enp0s6:1 down
- Remove the interface

##  Cables:
- The thick cable is for Data Transmission, connect it just below ethernet port.
- The thin cable is used for Powering up the board, connect it to the port located in the middle.

