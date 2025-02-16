# RPC Arm Project C25

## Project Overview: 
This will be the project overview!

## Setup:
For the setup of this project a Rasberry Pi 3B model is used. It is required to `ssh` into the Raspberry Pi (PI) by either the local or actual IP address of the device. It is also worth mentioning that VSCode IDE was used with the *Remote - SSH* extension for working on this project and its software. For accessing the PI by `ssh`, either `ssh rpc-arm@raspberrypi.local` or `ssh rpc-arm@42.10.0.1` can be used for access over ethernet. 

As for the PI workspace, the pigpio library was used for programming *MG995* servos and can be installed if not already by running the commands:

```bash
sudo apt update
sudo apt install pigpio
```

#### pigpio initialize error:
*pigpio* is responsible for handling servo motor control, however, initiating an executable program to run the pigpio daemon can cause problems if it is already running.

The solution simply required the daemon to be reset by killing all uses of it so that it is ready to be used upon the start of your program. This can be done by the command: 

```bash
sudo killall pigpiod
```

#### running the program:
When running the program, the workspace is compiled into an executable by the local makefile while linking the required libraries. When going to run, the executable should be run as super-user with the command: 

```bash
sudo ./main
```

## PI Setup:
The Raspberry Pi (PI) can be configured and communicated with in many ways. For this project, the PI was put on a metered ethernet network with a local PC sharing its WiFi. This was done by the three steps of (1) enabling the local PC to make its WiFi network share-able, (2) setting a manual metered connection with IPv4 address, netmask, and gateway on the PC as `42.10.0.3`, `255.255.255.0`, and `42.10.0.2` respectively, and (3) setting the IPv4 address, netmask, and gateway on the PI as `42.10.0.1`, `255.255.255.0`, and `42.10.0.2`. This should enable communication to ssh and share a WiFi interface. 
