
The Anti-Grain Geometry Project
A high quality rendering engine for C++
http://antigrain.com

Anti-Grain Geometry - Version 2.3 
Copyright (C) 2002-2005 Maxim Shemanarev (McSeem) 

Permission to copy, use, modify, sell and distribute this software 
is granted provided this copyright notice appears in all copies. 
This software is provided "as is" without express or implied
warranty, and with no claim as to its suitability for any purpose.



Information to Mac users using CodeWarrior

MacOS port for CW by
Hansruedi Baer
baer@karto.baug.ethz.ch

The CW project file is pretty big, so, it isn't included into the
basic package. Please download and unzip it from:

http://www.antigrain.com/agg2.mcp.xml.zip and put it into this directory.
Also, to build the SVG example, you need to obtain expat.lib and put it
to agg2/svg/macos_cw/expat.lib.
Download: http://www.antigrain.com/expat.lib.zip
Otherwise, just remove svg_test from the project.

Open file 'agg2.mcp.xml' from CW File/Import Project...
Save it as a CW project.
First compile sub-project agg2.lib. You can then
compile and execute all examples.
This project uses the PowerPC linker so it should also run on MacOS 9,
although this has not been tested.
The examples cannot be run from the command line.
For these examples to run, you might want to include a call to 'ccommand()'
in the source code (#include "console.h").
To work properly, the data file for example 'mol_view' needs to
be converted to Mac line breaks.

The examples have been compiled and tested with CodeWarrior version 8.3.


