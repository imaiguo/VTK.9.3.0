#!/usr/bin/env python
from vtkmodules.vtkIOImage import vtkImageReader
from vtkmodules.vtkImagingCore import vtkImageReslice
from vtkmodules.vtkInteractionImage import vtkImageViewer
import vtkmodules.vtkInteractionStyle
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

# This example demonstrates how to permute an image with vtkImageReslice.
# The advantage of using vtkImageReslice for this (rather than
# vtkImagePermute) is that you can specify negative or even oblique axes,
# and you can resample the image at the same time.
reader = vtkImageReader()
reader.ReleaseDataFlagOff()
reader.SetDataByteOrderToLittleEndian()
reader.SetDataExtent(0,63,0,63,1,93)
reader.SetDataSpacing(3.2,3.2,1.5)
reader.SetFilePrefix(VTK_DATA_ROOT + "/Data/headsq/quarter")
reader.SetDataMask(0x7fff)
reslice = vtkImageReslice()
reslice.SetInputConnection(reader.GetOutputPort())
# specify the new axes in terms of the original axes
reslice.SetResliceAxesDirectionCosines([0,+1,0,0,0,-1,-1,0,0])
# resample the image to enlarge & to get the aspect right
reslice.SetOutputSpacing(1.0,1.0,1.0)
viewer = vtkImageViewer()
viewer.SetInputConnection(reslice.GetOutputPort())
viewer.SetZSlice(100)
viewer.SetColorWindow(2000)
viewer.SetColorLevel(1000)
viewer.SetSize(200,140)
viewer.Render()
# --- end of script --
