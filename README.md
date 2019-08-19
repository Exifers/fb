# fb

This project aims at writing directly pixels anywhere on the screen
programmatically.

## What has been found out

WARNING : THIS SECTION SHOWS THE RESULT OF PERSONAL RESEARCH. THERE MAY BE SOME
MISTAKES AND IT SHOULD NOT IN ANY WAY BE CONSIDERED AS AN ACCURATE SOURCE OF
INFORMATION.

### /dev/fb0
It is possible to write pixels on the screen in console mode. To enter console
mode in ubuntu, type CTRL + ALT + F3.

In console mode what's shown on the screen is a mirror of what's in /dev/fb0
device file. 

We can fill this file with anything we want and get the pixels appear on the
screen.

```bash
# you need elevated priviledges to fill /dev/fb0
sudo bash

# fill the top of the screen :
echo "random stuff ..." > /dev/fb0 

# fill the whole screen with noise :
# (/dev/urandom is an infinite stream of random bytes)
cat /dev/urandom > /dev/fb0 
```

The format for pixels in the /dev/fb0 file is very simple, it is a series of
bytes for red, green and blue values of each pixel, left to right, top to
bottom. I didn't gather more precise informations about the format for this
document.

### The Linux graphics stack
However, the /dev/fb0 trick doesn't work out of console mode. On the regular
mode with a desktop environment, things are handled differently.

There is however some kind of frame buffer, but it leaves somewhere in the
graphics card, and accessing it may not be that easy. Let's have a look at the
different components of the Linux graphics stack to understand a bit better how
it works.


<pre>
User space
		+--------+ +--------+
		|X Client| |X Client|
		+--------+ +--------+
		     |	       |
                X protocol X protocol
		     |         |
		+-------------------+
		|		    |
		|     	 XORG  	    |
		|        	    |
		+-------------------+
		          |
		Xorg driver interface
			  |
                +-------------------+
                |                   |
                |     GPU driver    |
                |                   |
                +-------------------+
	                  |
--------------------------?-----------------------------------------------------
Kernel space              |
                +-------------------+
		|                   |
		| GPU Kernel module |
		|                   |
		+-------------------+
			  *		
--------------------------*-----------------------------------------------------
Hardware		  *
	        *********************
		*	 GPU	    *
		********************\*
</pre>

Any application having a window in the desktop environment is called an X client
because it sends requests to the XORG X server to spawn the window and draw in
it. 

XORG's job is to manage each window of each of its clients. Each client is
allowed to draw inside its own windows. XORG then sends drawing requests to GPU
driver which depends of the graphics card installed on your computer. 

There's also an open-source alternative for screen rendering called DRI
(Direct Rendering Infrastructure) which uses a kernel module called DRM
(Direct Rendering module). When using those, the entry point to the kernel
('?' in the schema above) in some file in /dev/dri/ like /dev/dri/card0.

## Project results
For now, I couldn't do what I wanted because :
- I don't find how to call the GPU driver directly without passing through Xorg
- Nvidia seems to support DRI but ioctl to /dev/dri/card0 gives a null memory
location for the framebuffer


