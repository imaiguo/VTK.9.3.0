#!/usr/bin/env python
from vtkmodules.vtkFiltersCore import vtkImageAppend
from vtkmodules.vtkIOImage import vtkSLCReader
from vtkmodules.vtkImagingCore import vtkImageMagnify
from vtkmodules.vtkImagingHybrid import vtkImageCursor3D
from vtkmodules.vtkInteractionImage import vtkImageViewer
import vtkmodules.vtkInteractionStyle
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

# Derived from Cursor3D.  This script increases the coverage of the
# vtkImageInplaceFilter super class.
# global values
CURSOR_X = 20
CURSOR_Y = 20
CURSOR_Z = 20
IMAGE_MAG_X = 2
IMAGE_MAG_Y = 2
IMAGE_MAG_Z = 1

# pipeline stuff
reader = vtkSLCReader()
reader.SetFileName(VTK_DATA_ROOT + "/Data/nut.slc")

# make the image a little bigger
magnify1 = vtkImageMagnify()
magnify1.SetInputConnection(reader.GetOutputPort())
magnify1.SetMagnificationFactors(IMAGE_MAG_X, IMAGE_MAG_Y, IMAGE_MAG_Z)
magnify1.ReleaseDataFlagOn()

magnify2 = vtkImageMagnify()
magnify2.SetInputConnection(reader.GetOutputPort())
magnify2.SetMagnificationFactors(IMAGE_MAG_X, IMAGE_MAG_Y, IMAGE_MAG_Z)
magnify2.ReleaseDataFlagOn()

# a filter that does in place processing (magnify ReleaseDataFlagOn)
cursor = vtkImageCursor3D()
cursor.SetInputConnection(magnify1.GetOutputPort())
cursor.SetCursorPosition(CURSOR_X * IMAGE_MAG_X,
                          CURSOR_Y * IMAGE_MAG_Y,
                          CURSOR_Z * IMAGE_MAG_Z)
cursor.SetCursorValue(255)
cursor.SetCursorRadius(50 * IMAGE_MAG_X)

# stream to increase coverage of in place filter.
# put the two together in one image
imageAppend = vtkImageAppend()
imageAppend.SetAppendAxis(0)
imageAppend.AddInputConnection(magnify2.GetOutputPort())
imageAppend.AddInputConnection(cursor.GetOutputPort())

viewer = vtkImageViewer()
viewer.SetInputConnection(imageAppend.GetOutputPort())
viewer.SetZSlice(CURSOR_Z * IMAGE_MAG_Z)
viewer.SetColorWindow(200)
viewer.SetColorLevel(80)
# viewer DebugOn
viewer.Render()
viewer.SetPosition(50, 50)
# make interface
viewer.Render()
