#!/usr/bin/env python
from vtkmodules.vtkCommonCore import vtkMath
from vtkmodules.vtkCommonDataModel import (
    vtkPlane,
    vtkSphere,
)
from vtkmodules.vtkCommonSystem import vtkTimerLog
from vtkmodules.vtkFiltersCore import vtkClipPolyData
from vtkmodules.vtkFiltersGeneral import vtkRandomAttributeGenerator
from vtkmodules.vtkFiltersPoints import (
    vtkBoundedPointSource,
    vtkFitImplicitFunction,
    vtkPointDensityFilter,
)
from vtkmodules.vtkRenderingCore import (
    vtkImageSlice,
    vtkImageSliceMapper,
    vtkRenderWindow,
    vtkRenderWindowInteractor,
    vtkRenderer,
)
import vtkmodules.vtkInteractionStyle
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

# The resolution of the density function volume
res = 100

# Parameters for debugging
NPts = 1000000
math = vtkMath()
math.RandomSeed(31415)

# create pipeline
#
points = vtkBoundedPointSource()
points.SetNumberOfPoints(NPts)
points.ProduceRandomScalarsOn()
points.ProduceCellOutputOff()
points.Update()

# Create a sphere implicit function
sphere = vtkSphere()
sphere.SetCenter(0.0,0.1,0.2)
sphere.SetRadius(0.75)

# Extract points within sphere
extract = vtkFitImplicitFunction()
extract.SetInputConnection(points.GetOutputPort())
extract.SetImplicitFunction(sphere)
extract.SetThreshold(0.005)
extract.GenerateVerticesOn()

# Clip out some of the points with a plane; requires vertices
plane = vtkPlane()
plane.SetOrigin(sphere.GetCenter())
plane.SetNormal(1,1,1)

clipper = vtkClipPolyData()
clipper.SetInputConnection(extract.GetOutputPort())
clipper.SetClipFunction(plane);

# Generate density field from points
# Use fixed radius
dens0 = vtkPointDensityFilter()
dens0.SetInputConnection(clipper.GetOutputPort())
dens0.SetSampleDimensions(res,res,res)
dens0.SetDensityEstimateToFixedRadius()
dens0.SetRadius(0.05)
#dens0.SetDensityEstimateToRelativeRadius()
dens0.SetRelativeRadius(2.5)
dens0.SetDensityFormToVolumeNormalized()

# Time execution
timer = vtkTimerLog()
timer.StartTimer()
dens0.Update()
timer.StopTimer()
time = timer.GetElapsedTime()
print("Time to compute density field: {0}".format(time))
vrange = dens0.GetOutput().GetScalarRange()
print(dens0)

map0 = vtkImageSliceMapper()
map0.BorderOn()
map0.SliceAtFocalPointOn()
map0.SliceFacesCameraOn()
map0.SetInputConnection(dens0.GetOutputPort())

slice0 = vtkImageSlice()
slice0.SetMapper(map0)
slice0.GetProperty().SetColorWindow(vrange[1]-vrange[0])
slice0.GetProperty().SetColorLevel(0.5*(vrange[0]+vrange[1]))

# Generate density field from points
# Use relative radius
dens1 = vtkPointDensityFilter()
dens1.SetInputConnection(clipper.GetOutputPort())
dens1.SetSampleDimensions(res,res,res)
#dens1.SetDensityEstimateToFixedRadius()
dens1.SetRadius(0.05)
dens1.SetDensityEstimateToRelativeRadius()
dens1.SetRelativeRadius(2.5)
dens1.SetDensityFormToNumberOfPoints()

# Time execution
timer = vtkTimerLog()
timer.StartTimer()
dens1.Update()
timer.StopTimer()
time = timer.GetElapsedTime()
print("Time to compute density field: {0}".format(time))
vrange = dens1.GetOutput().GetScalarRange()

map1 = vtkImageSliceMapper()
map1.BorderOn()
map1.SliceAtFocalPointOn()
map1.SliceFacesCameraOn()
map1.SetInputConnection(dens1.GetOutputPort())

slice1 = vtkImageSlice()
slice1.SetMapper(map1)
slice1.GetProperty().SetColorWindow(vrange[1]-vrange[0])
slice1.GetProperty().SetColorLevel(0.5*(vrange[0]+vrange[1]))

# Generate density field from points
# Use fixed radius and weighted point density and volume normalized density
# First need to generate some scalar attributes (weights)
weights = vtkRandomAttributeGenerator()
weights.SetInputConnection(clipper.GetOutputPort())
weights.SetMinimumComponentValue(0.25)
weights.SetMaximumComponentValue(1.75)
weights.GenerateAllDataOff()
weights.GeneratePointScalarsOn()

dens2 = vtkPointDensityFilter()
dens2.SetInputConnection(weights.GetOutputPort())
dens2.SetSampleDimensions(res,res,res)
dens2.SetDensityEstimateToFixedRadius()
dens2.SetRadius(0.05)
#dens2.SetDensityEstimateToRelativeRadius()
dens2.SetRelativeRadius(2.5)
dens2.SetDensityFormToVolumeNormalized()
dens2.ScalarWeightingOn()

# Time execution
timer = vtkTimerLog()
timer.StartTimer()
dens2.Update()
timer.StopTimer()
time = timer.GetElapsedTime()
print("Time to compute density field: {0}".format(time))
vrange = dens2.GetOutput().GetScalarRange()

map2 = vtkImageSliceMapper()
map2.BorderOn()
map2.SliceAtFocalPointOn()
map2.SliceFacesCameraOn()
map2.SetInputConnection(dens2.GetOutputPort())

slice2 = vtkImageSlice()
slice2.SetMapper(map2)
slice2.GetProperty().SetColorWindow(vrange[1]-vrange[0])
slice2.GetProperty().SetColorLevel(0.5*(vrange[0]+vrange[1]))

# Generate density field from points
# Use relative radius and weighted point density and npts density
dens3 = vtkPointDensityFilter()
dens3.SetInputConnection(weights.GetOutputPort())
dens3.SetSampleDimensions(res,res,res)
#dens3.SetDensityEstimateToFixedRadius()
dens3.SetRadius(0.05)
dens3.SetDensityEstimateToRelativeRadius()
dens3.SetRelativeRadius(2.5)
dens3.SetDensityFormToNumberOfPoints()
dens3.ScalarWeightingOn()

# Time execution
timer = vtkTimerLog()
timer.StartTimer()
dens3.Update()
timer.StopTimer()
time = timer.GetElapsedTime()
print("Time to compute density field: {0}".format(time))
vrange = dens3.GetOutput().GetScalarRange()

map3 = vtkImageSliceMapper()
map3.BorderOn()
map3.SliceAtFocalPointOn()
map3.SliceFacesCameraOn()
map3.SetInputConnection(dens3.GetOutputPort())

slice3 = vtkImageSlice()
slice3.SetMapper(map3)
slice3.GetProperty().SetColorWindow(vrange[1]-vrange[0])
slice3.GetProperty().SetColorLevel(0.5*(vrange[0]+vrange[1]))

# Create the RenderWindow, Renderer and both Actors
#
ren0 = vtkRenderer()
ren0.SetViewport(0,0,0.5,0.5)
ren1 = vtkRenderer()
ren1.SetViewport(0.5,0,1,0.5)
ren2 = vtkRenderer()
ren2.SetViewport(0,0.5,0.5,1)
ren3 = vtkRenderer()
ren3.SetViewport(0.5,0.5,1,1)

renWin = vtkRenderWindow()
renWin.AddRenderer(ren0)
renWin.AddRenderer(ren1)
renWin.AddRenderer(ren2)
renWin.AddRenderer(ren3)
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

# Add the actors to the renderer, set the background and size
#
ren0.AddActor(slice0)
ren0.SetBackground(0,0,0)
ren1.AddActor(slice1)
ren1.SetBackground(0,0,0)
ren2.AddActor(slice2)
ren2.SetBackground(0,0,0)
ren3.AddActor(slice3)
ren3.SetBackground(0,0,0)

renWin.SetSize(300,300)

cam = ren0.GetActiveCamera()
cam.ParallelProjectionOn()
cam.SetFocalPoint(0,0,0)
cam.SetPosition(0,0,1)
ren0.ResetCamera()

ren1.SetActiveCamera(cam)
ren2.SetActiveCamera(cam)
ren3.SetActiveCamera(cam)

iren.Initialize()

# render the image
#
renWin.Render()

iren.Start()
