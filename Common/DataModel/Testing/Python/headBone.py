#!/usr/bin/env python
from vtkmodules.vtkCommonColor import vtkNamedColors
from vtkmodules.vtkCommonDataModel import vtkMergePoints
from vtkmodules.vtkFiltersCore import (
    vtkMarchingCubes,
    vtkVectorNorm,
)
from vtkmodules.vtkFiltersModeling import vtkOutlineFilter
from vtkmodules.vtkIOImage import vtkVolume16Reader
from vtkmodules.vtkRenderingCore import (
    vtkActor,
    vtkDataSetMapper,
    vtkLight,
    vtkPolyDataMapper,
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


# Create the RenderWindow, Renderer and both Actors
#
ren1 = vtkRenderer()
renWin = vtkRenderWindow()
renWin.AddRenderer(ren1)
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
lgt = vtkLight()
# create pipeline
#
locator = vtkMergePoints()
locator.SetDivisions(32, 32, 46)
locator.SetNumberOfPointsPerBucket(2)
locator.AutomaticOff()

v16 = vtkVolume16Reader()
v16.SetDataDimensions(64, 64)
v16.GetOutput().SetOrigin(0.0, 0.0, 0.0)
v16.SetDataByteOrderToLittleEndian()
v16.SetFilePrefix(VTK_DATA_ROOT + "/Data/headsq/quarter")
v16.SetImageRange(1, 93)
v16.SetDataSpacing(3.2, 3.2, 1.5)

iso = vtkMarchingCubes()
iso.SetInputConnection(v16.GetOutputPort())
iso.SetValue(0, 1150)
iso.ComputeGradientsOn()
iso.ComputeScalarsOff()
iso.SetLocator(locator)

gradient = vtkVectorNorm()
gradient.SetInputConnection(iso.GetOutputPort())

isoMapper = vtkDataSetMapper()
isoMapper.SetInputConnection(gradient.GetOutputPort())
isoMapper.ScalarVisibilityOn()
isoMapper.SetScalarRange(0, 1200)

isoActor = vtkActor()
isoActor.SetMapper(isoMapper)

isoProp = isoActor.GetProperty()
isoProp.SetColor(GetRGBColor('antique_white'))

outline = vtkOutlineFilter()
outline.SetInputConnection(v16.GetOutputPort())

outlineMapper = vtkPolyDataMapper()
outlineMapper.SetInputConnection(outline.GetOutputPort())

outlineActor = vtkActor()
outlineActor.SetMapper(outlineMapper)

outlineProp = outlineActor.GetProperty()
# outlineProp.SetColor(0, 0, 0)

# Add the actors to the renderer, set the background and size
#
ren1.AddActor(outlineActor)
ren1.AddActor(isoActor)
ren1.SetBackground(1, 1, 1)
ren1.AddLight(lgt)

renWin.SetSize(250, 250)

ren1.SetBackground(0.1, 0.2, 0.4)
ren1.ResetCamera()

cam1 = ren1.GetActiveCamera()
cam1.Elevation(90)
cam1.SetViewUp(0, 0, -1)
cam1.Zoom(1.5)

lgt.SetPosition(cam1.GetPosition())
lgt.SetFocalPoint(cam1.GetFocalPoint())

ren1.ResetCameraClippingRange()

# render the image
#
renWin.Render()

#iren.Start()
