# determing devices connected and its nodes
raspberrypi:~$ v4l2-ctl --list-devices
bcm2835-codec-decode (platform:bcm2835-codec):
	/dev/video10
	/dev/video11
	/dev/video12

USB2.0 Camera: USB2.0 Camera (usb-3f980000.usb-1.4):
	/dev/video0
	/dev/video1

# determing device parameters for /dev/video0
pi@raspberrypi:~$ sudo v4l2-ctl -d /dev/video0 --get-parm
Streaming Parameters Video Capture:
	Capabilities     : timeperframe
	Frames per second: 30.000 (30/1)
	Read buffers     : 0

# determing image formats using v4l2-ctl
pi@raspberrypi:~$ v4l2-ctl --list-formats-ext
ioctl: VIDIOC_ENUM_FMT
	Type: Video Capture
	[0]: 'YUYV' (YUYV 4:2:2)
		Size: Discrete 640x480
			Interval: Discrete 0.033s (30.000 fps)
		Size: Discrete 352x288
			Interval: Discrete 0.033s (30.000 fps)
		Size: Discrete 320x240
			Interval: Discrete 0.033s (30.000 fps)
		Size: Discrete 176x144
			Interval: Discrete 0.033s (30.000 fps)
		Size: Discrete 160x120
			Interval: Discrete 0.033s (30.000 fps)
