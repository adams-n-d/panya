panya: computer vision tracking project for experimental rats

An interface to track the movements of laboratory animals for experimental purposes.
To be released under a BSD license.
Developed at Emory University under the supervision of Dr. Cengiz Gunay by

Nathan Adams    - Lead Developer
Xia Hong        - GUI
Jung-Ting Chien - Testing

for Dr. Joseph Manns, Emory Department of Behavioral Neuroscience


Usage:

$ panya OPTIONS input_video.avi [input_video2.avi ...]

OPTIONS:
May include any combination of the following options, with or without preceding '-' (must be lowercase)

r	Track the brightest red pixel
b	Track the brightest blue pixel
g	Track the brightest green pixel
v	Output video showing tracking results
		Note: this increases running time nontrivially.
		Not recommended for batch mode.
h	Display this help text and exit

Example: track red and green pixels for two video files,
	and output videos for each
$ ./vidtrack rgv video1.avi video2.avi

=============================
NB: COMING SYNTAX CHANGES
flags: v, h
options: led {r,g,b}
$ panya -h
$ panya -v --led g [FILE]
$ panya -v --l rgb [FILE]

============================

OUTPUT generated:
output 2d-coordinate txt file (csv) (including orientation)
JPG heat map image
Video file showing chosen pixels (if 'v' options selected)

