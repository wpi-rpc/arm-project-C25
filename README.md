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

The solution simply required the daemon to be reset by killing all uses of it so that it is ready to be used upon start of your program. This can be done by the command: 

```bash
sudo killall pigpiod
```

#### running the program:
When running the program, the workspace is compiled into an executable by the local makefile while linking the required libraries. When going to run, the executable should be ran while as super-user with the command: 

```bash
sudo ./main
```

### more stuff coming later! 