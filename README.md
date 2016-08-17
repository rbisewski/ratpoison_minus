# ratpoison minus - a fork of ratpoison with a simple Makefile system

ratpoison minus is a rather sloppy fork of ratpoison with a simple Makefile
based system for building, rather than having to deal with autoconf and the
like.

The original purpose of this project was to increase my familiarity with
how Xlib is utilized within the context of a window manager.

There is a bit of code cleanup / commenting in this version, as an attempt
was made to organize some of the files into proper modules.

I had considered porting parts of it to Xcb, but the current codebase has
a level of dependency hell that is difficult to work around without
investing hundreds of additional hours. 

This is released with the hope that perhaps someone will find it useful.

Finally, a tip-of-the-hat to the ratpoison developers (esp. Shawn Betts)
for their excellent C and Xlib knowledge. That project was helpful for
understanding how these low level concepts work.


# Requirements

Specifically, the following packages are required:

* freetype
* Xext
* Xft
* Xinerama
* Xlib
* Xtst

Recommend examining the Makefile for more details if you are unable to get
ratpoison minus to properly compile.


# Installation

Edit the Makefile to adjust the respective library requirements to their
proper places.

Afterwards enter the following command to build and install ratpoison minus
(if necessary as root):

    make clean install


# Running ratpoison minus

The binary features the same options of ratpoison, for the interest of
backwards compatibility.

For more detailed instructions regarding ratpoison, consider reading the
following documentation:

http://ratpoison.wxcvbn.org/ 


# Authors

For more information about the original authors of ratpoison, consider
contacting them at:

* Website -> http://www.nongnu.org/ratpoison/

This was forked and adapted by Robert Bisewski at Ibis Cybernetics. For
more information, contact:

* Website -> www.ibiscybernetics.com

* Email -> contact@ibiscybernetics.com
