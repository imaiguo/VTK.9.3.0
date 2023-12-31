#!/usr/bin/env python
from vtkmodules.vtkCommonDataModel import (
    vtkPiecewiseFunction,
    vtkPlane,
)
from vtkmodules.vtkCommonTransforms import vtkTransform
from vtkmodules.vtkFiltersSources import vtkConeSource
from vtkmodules.vtkIOImage import vtkVolume16Reader
from vtkmodules.vtkRenderingCore import (
    vtkActor,
    vtkColorTransferFunction,
    vtkDataSetMapper,
    vtkRenderWindow,
    vtkRenderWindowInteractor,
    vtkRenderer,
    vtkVolume,
    vtkVolumeProperty,
)
from vtkmodules.vtkRenderingVolume import (
    vtkFixedPointVolumeRayCastMapper,
    vtkVolumePicker,
)
import vtkmodules.vtkInteractionStyle
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
import vtkmodules.vtkRenderingVolumeOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

# volume render a medical data set
# renderer and interactor
ren = vtkRenderer()
renWin = vtkRenderWindow()
renWin.AddRenderer(ren)
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

# read the volume
v16 = vtkVolume16Reader()
v16.SetDataDimensions(64, 64)
v16.SetImageRange(1, 93)
v16.SetDataByteOrderToLittleEndian()
v16.SetFilePrefix(VTK_DATA_ROOT + "/Data/headsq/quarter")
v16.SetDataSpacing(3.2, 3.2, 1.5)

#---------------------------------------------------------
# set up the volume rendering
volumeMapper = vtkFixedPointVolumeRayCastMapper()
volumeMapper.SetInputConnection(v16.GetOutputPort())
volumeMapper.CroppingOn()
volumeMapper.SetCroppingRegionPlanes(0.0, 141.6, 0.0, 201.6, 0.0, 138.0)

volumeColor = vtkColorTransferFunction()
volumeColor.AddRGBPoint(0, 0.0, 0.0, 0.0)
volumeColor.AddRGBPoint(180, 0.3, 0.1, 0.2)
volumeColor.AddRGBPoint(1000, 1.0, 0.7, 0.6)
volumeColor.AddRGBPoint(2000, 1.0, 1.0, 0.9)

volumeScalarOpacity = vtkPiecewiseFunction()
volumeScalarOpacity.AddPoint(0, 0.0)
volumeScalarOpacity.AddPoint(180, 0.0)
volumeScalarOpacity.AddPoint(1000, 0.2)
volumeScalarOpacity.AddPoint(2000, 0.8)

volumeGradientOpacity = vtkPiecewiseFunction()
volumeGradientOpacity.AddPoint(0, 0.0)
volumeGradientOpacity.AddPoint(90, 0.5)
volumeGradientOpacity.AddPoint(100, 1.0)

volumeProperty = vtkVolumeProperty()
volumeProperty.SetColor(volumeColor)
volumeProperty.SetScalarOpacity(volumeScalarOpacity)
volumeProperty.SetGradientOpacity(volumeGradientOpacity)
volumeProperty.SetInterpolationTypeToLinear()
volumeProperty.ShadeOn()
volumeProperty.SetAmbient(0.6)
volumeProperty.SetDiffuse(0.6)
volumeProperty.SetSpecular(0.1)

volume = vtkVolume()
volume.SetMapper(volumeMapper)
volume.SetProperty(volumeProperty)

#---------------------------------------------------------
# make a transform and some clipping planes
transform = vtkTransform()
transform.RotateWXYZ(-20, 0.0, -0.7, 0.7)

volume.SetUserTransform(transform)

c = volume.GetCenter()

volumeClip = vtkPlane()
volumeClip.SetNormal(0, 1, 0)
volumeClip.SetOrigin(c[0], c[1], c[2])
volumeMapper.AddClippingPlane(volumeClip)

#---------------------------------------------------------
ren.AddViewProp(volume)

camera = ren.GetActiveCamera()
camera.SetFocalPoint(c[0], c[1], c[2])
camera.SetPosition(c[0] + 500, c[1] - 100, c[2] - 100)
camera.SetViewUp(0, 0, -1)

ren.ResetCameraClippingRange()

renWin.Render()
#---------------------------------------------------------
# The cone source points along the x axis
coneSource = vtkConeSource()
coneSource.CappingOn()
coneSource.SetHeight(12)
coneSource.SetRadius(5)
coneSource.SetResolution(31)
coneSource.SetCenter(6, 0, 0)
coneSource.SetDirection(-1, 0, 0)

#---------------------------------------------------------
picker = vtkVolumePicker()
picker.SetTolerance(1e-6)
picker.SetVolumeOpacityIsovalue(0.3)

# A function to point an actor along a vector
def PointCone(actor, nx, ny, nz):
    if nx < 0.0:
        actor.RotateWXYZ(180, 0, 1, 0)
        actor.RotateWXYZ(180, (nx - 1.0) * 0.5, ny * 0.5, nz * 0.5)
    else:
        actor.RotateWXYZ(180, (nx + 1.0) * 0.5, ny * 0.5, nz * 0.5)

# Pick part of the volume that is clipped away
picker.Pick(192, 103, 0, ren)

# puts [picker Print]

p = picker.GetPickPosition()

n = picker.GetPickNormal()

coneActor1 = vtkActor()
coneActor1.PickableOff()

coneMapper1 = vtkDataSetMapper()
coneMapper1.SetInputConnection(coneSource.GetOutputPort())

coneActor1.SetMapper(coneMapper1)
coneActor1.GetProperty().SetColor(1, 0, 0)
coneActor1.GetProperty().BackfaceCullingOn()
coneActor1.SetPosition(p[0], p[1], p[2])

PointCone(coneActor1, n[0], n[1], n[2])

ren.AddViewProp(coneActor1)

# Pick through a cropping plane to some bone
# This should usually be left alone, but is used here to increase coverage
picker.UseVolumeGradientOpacityOn()
picker.Pick(90, 180, 0, ren)

p = picker.GetPickPosition()

n = picker.GetPickNormal()

coneActor2 = vtkActor()
coneActor2.PickableOff()

coneMapper2 = vtkDataSetMapper()
coneMapper2.SetInputConnection(coneSource.GetOutputPort())

coneActor2.SetMapper(coneMapper2)
coneActor2.GetProperty().SetColor(1, 0, 0)
coneActor2.GetProperty().BackfaceCullingOn()
coneActor2.SetPosition(p[0], p[1], p[2])

PointCone(coneActor2, n[0], n[1], n[2])

ren.AddViewProp(coneActor2)

# Pick through a cropping plane to some transparent tissue
# Ignore gradient opacity, since it makes it harder to find isosurface

picker.UseVolumeGradientOpacityOff()
picker.Pick(125, 195, 0, ren)

p = picker.GetPickPosition()

n = picker.GetPickNormal()

coneActor3 = vtkActor()
coneActor3.PickableOff()

coneMapper3 = vtkDataSetMapper()
coneMapper3.SetInputConnection(coneSource.GetOutputPort())

coneActor3.SetMapper(coneMapper3)
coneActor3.GetProperty().SetColor(1, 0, 0)
coneActor3.GetProperty().BackfaceCullingOn()
coneActor3.SetPosition(p[0], p[1], p[2])

PointCone(coneActor3, n[0], n[1], n[2])

ren.AddViewProp(coneActor3)

# Pick through a clipping plane
picker.Pick(150, 160, 0, ren)

p = picker.GetPickPosition()

n = picker.GetPickNormal()

coneActor4 = vtkActor()
coneActor4.PickableOff()

coneMapper4 = vtkDataSetMapper()
coneMapper4.SetInputConnection(coneSource.GetOutputPort())

coneActor4.SetMapper(coneMapper4)
coneActor4.GetProperty().SetColor(1, 0, 0)
coneActor4.GetProperty().BackfaceCullingOn()
coneActor4.SetPosition(p[0], p[1], p[2])

PointCone(coneActor4, n[0], n[1], n[2])

ren.AddViewProp(coneActor4)

# Pick through a cropping plane with PickCroppingPlanesOn
picker.PickCroppingPlanesOn()
picker.Pick(125, 195, 0, ren)

p = picker.GetPickPosition()

n = picker.GetPickNormal()

coneActor5 = vtkActor()
coneActor5.PickableOff()

coneMapper5 = vtkDataSetMapper()
coneMapper5.SetInputConnection(coneSource.GetOutputPort())

coneActor5.SetMapper(coneMapper5)
coneActor5.GetProperty().SetColor(0, 1, 0)
coneActor5.GetProperty().BackfaceCullingOn()
coneActor5.SetPosition(p[0], p[1], p[2])

PointCone(coneActor5, n[0], n[1], n[2])

ren.AddViewProp(coneActor5)

# Pick through a clipping plane with PickCroppingPlanesOn
picker.PickCroppingPlanesOn()

picker.Pick(150, 160, 0, ren)

p = picker.GetPickPosition()

n = picker.GetPickNormal()

coneActor6 = vtkActor()
coneActor6.PickableOff()

coneMapper6 = vtkDataSetMapper()
coneMapper6.SetInputConnection(coneSource.GetOutputPort())

coneActor6.SetMapper(coneMapper6)
coneActor6.GetProperty().SetColor(0, 1, 0)
coneActor6.GetProperty().BackfaceCullingOn()
coneActor6.SetPosition(p[0], p[1], p[2])

PointCone(coneActor6, n[0], n[1], n[2])

ren.AddViewProp(coneActor6)
ren.ResetCameraClippingRange()
renWin.Render()

#---------------------------------------------------------
# test-related code
def TkCheckAbort(obj=None, event=""):
    if renWin.GetEventPending():
        renWin.SetAbortRender(1)

renWin.AddObserver("AbortCheckEvent", TkCheckAbort)

iren.Initialize()
#iren.Start()
