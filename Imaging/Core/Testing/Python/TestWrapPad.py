#!/usr/bin/env python
from vtkmodules.vtkIOImage import vtkImageReader
from vtkmodules.vtkImagingCore import vtkImageWrapPad
from vtkmodules.vtkInteractionImage import vtkImageViewer
import vtkmodules.vtkInteractionStyle
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

# Make an image larger by repeating the data.  Tile.
# Image pipeline
reader = vtkImageReader()
reader.ReleaseDataFlagOff()
reader.SetDataByteOrderToLittleEndian()
reader.SetDataExtent(0,63,0,63,1,93)
reader.SetDataSpacing(3.2,3.2,1.5)
reader.SetFilePrefix(VTK_DATA_ROOT + "/Data/headsq/quarter")
reader.SetDataMask(0x7fff)
pad = vtkImageWrapPad()
pad.SetInputConnection(reader.GetOutputPort())
pad.SetOutputWholeExtent(-100,155,-100,170,0,92)
pad.ReleaseDataFlagOff()
viewer = vtkImageViewer()
viewer.SetInputConnection(pad.GetOutputPort())
viewer.SetZSlice(22)
viewer.SetColorWindow(2000)
viewer.SetColorLevel(1000)
viewer.GetActor2D().SetDisplayPosition(150,150)
viewer.SetSize(256,270)
viewer.Render()
# --- end of script --
