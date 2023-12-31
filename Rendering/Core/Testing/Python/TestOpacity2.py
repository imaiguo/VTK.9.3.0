#!/usr/bin/env python
from vtkmodules.vtkCommonCore import vtkLookupTable
from vtkmodules.vtkFiltersCore import vtkElevationFilter
from vtkmodules.vtkFiltersSources import (
    vtkConeSource,
    vtkTexturedSphereSource,
)
from vtkmodules.vtkIOImage import vtkPNGReader
from vtkmodules.vtkInteractionStyle import vtkInteractorStyleTrackballCamera
from vtkmodules.vtkRenderingCore import (
    vtkActor,
    vtkPolyDataMapper,
    vtkRenderWindow,
    vtkRenderWindowInteractor,
    vtkRenderer,
    vtkTexture,
)
import vtkmodules.vtkInteractionStyle
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

cone = vtkConeSource()
cone.SetHeight(3.0)
cone.SetRadius(1.0)
cone.SetResolution(10)

coneMapper = vtkPolyDataMapper()
coneMapper.SetInputConnection(cone.GetOutputPort())

# Actor for opacity as a property value.
coneActor = vtkActor()
coneActor.SetMapper(coneMapper)
coneActor.GetProperty().SetOpacity(0.5)

# Actor for opacity through LUT.
elevation = vtkElevationFilter()
elevation.SetInputConnection(cone.GetOutputPort())

coneMapper2 = vtkPolyDataMapper()
coneMapper2.SetInputConnection(elevation.GetOutputPort())

lut = vtkLookupTable()
lut.SetAlphaRange(0.9, 0.1)
lut.SetHueRange(0, 0)
lut.SetSaturationRange(1, 1)
lut.SetValueRange(1, 1)

coneMapper2.SetLookupTable(lut)
coneMapper2.SetScalarModeToUsePointData()
coneMapper2.SetScalarVisibility(1)
coneMapper2.InterpolateScalarsBeforeMappingOn()

coneActorLUT = vtkActor()
coneActorLUT.SetMapper(coneMapper2)
coneActorLUT.SetPosition(0.1, 1.0, 0)
coneActorLUT.GetProperty().SetOpacity(0.99)

# Actor for opacity through texture.
reader = vtkPNGReader()
reader.SetFileName(VTK_DATA_ROOT + "/Data/alphachannel.png")
reader.Update()

sphere = vtkTexturedSphereSource()

texture = vtkTexture()
texture.SetInputConnection(reader.GetOutputPort())

coneMapper3 = vtkPolyDataMapper()
coneMapper3.SetInputConnection(sphere.GetOutputPort())

coneActorTexture = vtkActor()
coneActorTexture.SetTexture(texture)
coneActorTexture.SetMapper(coneMapper3)
coneActorTexture.SetPosition(0, -1.0, 0)
coneActorTexture.GetProperty().SetColor(0.5, 0.5, 1)
coneActorTexture.GetProperty().SetOpacity(0.99)

ren1 = vtkRenderer()
ren1.AddActor(coneActor)
ren1.AddActor(coneActorLUT)
ren1.AddActor(coneActorTexture)
ren1.SetBackground(0.1, 0.2, 0.4)
ren1.SetUseDepthPeeling(1)
# 20 layers of translucency
ren1.SetMaximumNumberOfPeels(20)
# 2 out of 1000 pixels
ren1.SetOcclusionRatio(0.002)

renWin = vtkRenderWindow()
renWin.SetMultiSamples(0)
renWin.SetAlphaBitPlanes(1)
renWin.AddRenderer(ren1)

renWin.SetSize(300, 300)

iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
style = vtkInteractorStyleTrackballCamera()
iren.SetInteractorStyle(style)
iren.Initialize()

camera = ren1.GetActiveCamera()
camera.SetPosition(9, -1, 3)
camera.SetViewAngle(30)
camera.SetViewUp(0.05, 0.96, 0.24)
camera.SetFocalPoint(0, 0.25, 0)
ren1.ResetCameraClippingRange()

renWin.Render()

print(ren1.GetLastRenderingUsedDepthPeeling())
if (ren1.GetLastRenderingUsedDepthPeeling()):
    print("depth peeling was used")
else:
    print("depth peeling was not used (alpha blending instead)")

#iren.Start()
