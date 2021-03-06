# rnsavinelli's build of slstatus - suckless status

slstatus is a suckless status monitor for window managers that use WM_NAME
(e.g. dwm) or stdin to fill the status bar.

This fork of [slstatus](https://tools.suckless.org/slstatus/) has my personal configurations, patches and modifications.

## Features

- Battery percentage/state/time left
- Brightness level
- CPU usage
- CPU frequency
- **Custom shell commands**
- Date and time
- Disk status (free storage, percentage, total storage and used storage)
- Username
- Hostname
- IP address (IPv4 and IPv6)
- Load average
- Network speeds (RX and TX)
- Memory status (free memory, percentage, total memory and used memory)
- Swap status (free swap, percentage, total swap and used swap)
- Temperature
- Uptime
- WiFi signal percentage and ESSID

## Requirements

Currently slstatus works on FreeBSD, Linux and OpenBSD.
In order to build slstatus you need the Xlib header files.

## Installation

Edit config.mk to match your local setup (slstatus is installed into the
/usr/local namespace by default).

Afterwards enter the following command to build and install slstatus (if
necessary as root):

```
git clone https://github.com/rnsavinelli/slstatus
cd slstatus
make clean install
```

## Running slstatus

See the man page for details.

## Configuration

slstatus can be customized by creating a custom config.h and (re)compiling the
source code. This keeps it fast, secure and simple.
