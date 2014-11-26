//----------------------------------------------------------------------------
// Anti-Grain Geometry - Version 2.3 
// Copyright (C) 2002-2005 Maxim Shemanarev (McSeem)
//
// Permission to copy, use, modify, sell and distribute this software 
// is granted provided this copyright notice appears in all copies. 
// This software is provided "as is" without express or implied
// warranty, and with no claim as to its suitability for any purpose.
//
//----------------------------------------------------------------------------
// Contact: mcseem@antigrain.com
//          mcseemagg@yahoo.com
//          http://www.antigrain.com
//----------------------------------------------------------------------------


Custom Rasterizer example. 

Due to some stupid Microsoft internal confilict one cannot link anything that 
uses new/delete with static MFC because of the name conflict between 
C-RunTime library and MFC. 

Well, actually one can if she/he uses the linker option "Force file output", 
which means "try to produce some shit that may be not working". 
Excuse my French.
This very options is used in this project, so, please ignore the linker 
warnings and please don't rely on the executable file.

