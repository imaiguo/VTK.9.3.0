#!/usr/bin/env python
from vtkmodules.vtkCommonColor import vtkNamedColors
from vtkmodules.vtkFiltersGeneral import vtkEdgePoints
from vtkmodules.vtkIOImage import vtkVolume16Reader
from vtkmodules.vtkRenderingCore import (
    vtkActor,
    vtkDataSetMapper,
    vtkRenderWindow,
    vtkRenderWindowInteractor,
    vtkRenderer,
)
import vtkmodules.vtkInteractionStyle
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

def GetRGBColor(colorName):
    '''
        Return the red, green and blue components for a
        color as doubles.
    '''
    rgb = [0.0, 0.0, 0.0]  # black
    vtkNamedColors().GetColorRGB(colorName, rgb)
    return rgb

# create pipeline
# reader reads slices
v16 = vtkVolume16Reader()
v16.SetDataDimensions(64, 64)
v16.SetDataByteOrderToLittleEndian()
v16.SetFilePrefix(VTK_DATA_ROOT + "/Data/headsq/quarter")
v16.SetDataSpacing(3.2, 3.2, 1.5)
v16.SetImageRange(30, 50)
v16.SetDataMask(0x7fff)

# create points on edges
edgePoints = vtkEdgePoints()
edgePoints.SetInputConnection(v16.GetOutputPort())
edgePoints.SetValue(1150)

#
mapper = vtkDataSetMapper()
mapper.SetInputConnection(edgePoints.GetOutputPort())
mapper.ScalarVisibilityOff()

head = vtkActor()
head.SetMapper(mapper)
head.GetProperty().SetColor(GetRGBColor('raw_sienna'))

# Create the RenderWindow, Renderer and Interactor
#
ren1 = vtkRenderer()
renWin = vtkRenderWindow()
renWin.AddRenderer(ren1)
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

# Add the actors to the renderer, set the background and size
#
ren1.AddActor(head)
ren1.SetBackground(1, 1, 1)

renWin.SetSize(300, 300)

ren1.SetBackground(GetRGBColor('slate_grey'))
ren1.GetActiveCamera().SetPosition(99.8847, 537.86, 22.4716)
ren1.GetActiveCamera().SetFocalPoint(99.8847, 109.81, 15)
ren1.GetActiveCamera().SetViewAngle(20)
ren1.GetActiveCamera().SetViewUp(0, -1, 0)
ren1.ResetCameraClippingRange()

# render the image
#
renWin.Render()

#iren.Start()
