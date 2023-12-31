#!/usr/bin/env python
from vtkmodules.vtkCommonCore import (
    vtkMath,
    vtkPoints,
)
from vtkmodules.vtkCommonDataModel import vtkPolyData
from vtkmodules.vtkFiltersCore import vtkDelaunay3D
from vtkmodules.vtkFiltersGeneral import vtkShrinkFilter
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

# create some random points in the unit cube centered at (.5,.5,.5)
#
math = vtkMath()
points = vtkPoints()
i = 0
while i < 25:
    points.InsertPoint(i,math.Random(0,1),math.Random(0,1),math.Random(0,1))
    i = i + 1

profile = vtkPolyData()
profile.SetPoints(points)
# triangulate them
#
del1 = vtkDelaunay3D()
del1.SetInputData(profile)
del1.BoundingTriangulationOn()
del1.SetTolerance(0.01)
del1.SetAlpha(0.2)
del1.BoundingTriangulationOff()
shrink = vtkShrinkFilter()
shrink.SetInputConnection(del1.GetOutputPort())
shrink.SetShrinkFactor(0.9)
map = vtkDataSetMapper()
map.SetInputConnection(shrink.GetOutputPort())
triangulation = vtkActor()
triangulation.SetMapper(map)
triangulation.GetProperty().SetColor(1,0,0)
# Create graphics stuff
#
ren1 = vtkRenderer()
renWin = vtkRenderWindow()
renWin.AddRenderer(ren1)
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
# Add the actors to the renderer, set the background and size
#
ren1.AddActor(triangulation)
ren1.SetBackground(1,1,1)
renWin.SetSize(250,250)
renWin.Render()
cam1 = ren1.GetActiveCamera()
cam1.Zoom(1.5)
# render the image
#
renWin.Render()
# prevent the tk window from showing up then start the event loop
# --- end of script --
