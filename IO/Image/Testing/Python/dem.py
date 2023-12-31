#!/usr/bin/env python
from vtkmodules.vtkCommonCore import vtkLookupTable
from vtkmodules.vtkCommonExecutionModel import vtkCastToConcrete
from vtkmodules.vtkFiltersCore import (
    vtkElevationFilter,
    vtkPolyDataNormals,
)
from vtkmodules.vtkFiltersGeneral import vtkWarpScalar
from vtkmodules.vtkFiltersGeometry import vtkImageDataGeometryFilter
from vtkmodules.vtkImagingCore import vtkImageShrink3D
from vtkmodules.vtkIOImage import vtkDEMReader
from vtkmodules.vtkRenderingCore import (
    vtkPolyDataMapper,
    vtkRenderWindow,
    vtkRenderWindowInteractor,
    vtkRenderer,
)
from vtkmodules.vtkRenderingLOD import vtkLODActor
import vtkmodules.vtkInteractionStyle
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

# Create the RenderWindow, Renderer and both Actors
#
ren1 = vtkRenderer()
renWin = vtkRenderWindow()
renWin.AddRenderer(ren1)
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

Scale = 5

lut = vtkLookupTable()
lut.SetHueRange(0.6, 0)
lut.SetSaturationRange(1.0, 0)
lut.SetValueRange(0.5, 1.0)

demModel = vtkDEMReader()
demModel.SetFileName(VTK_DATA_ROOT + "/Data/SainteHelens.dem")
demModel.Update()

lo = Scale * demModel.GetElevationBounds()[0]
hi = Scale * demModel.GetElevationBounds()[1]
demActor = vtkLODActor()

# create a pipeline for each lod mapper
lods = ["4", "8", "16"]
for lod in lods:
    exec("shrink" + lod + " = vtkImageShrink3D()")
    eval("shrink" + lod).SetShrinkFactors(int(lod), int(lod), 1)
    eval("shrink" + lod).SetInputConnection(demModel.GetOutputPort())
    eval("shrink" + lod).AveragingOn()

    exec("geom" + lod + " = vtkImageDataGeometryFilter()")
    eval("geom" + lod).SetInputConnection(eval("shrink" + lod).GetOutputPort())
    eval("geom" + lod).ReleaseDataFlagOn()

    exec("warp" + lod + " = vtkWarpScalar()")
    eval("warp" + lod).SetInputConnection(eval("geom" + lod).GetOutputPort())
    eval("warp" + lod).SetNormal(0, 0, 1)
    eval("warp" + lod).UseNormalOn()
    eval("warp" + lod).SetScaleFactor(Scale)
    eval("warp" + lod).ReleaseDataFlagOn()

    exec("elevation" + lod + " = vtkElevationFilter()")
    eval("elevation" + lod).SetInputConnection(
      eval("warp" + lod).GetOutputPort())
    eval("elevation" + lod).SetLowPoint(0, 0, lo)
    eval("elevation" + lod).SetHighPoint(0, 0, hi)
    eval("elevation" + lod).SetScalarRange(lo, hi)
    eval("elevation" + lod).ReleaseDataFlagOn()

    exec("toPoly" + lod + " = vtkCastToConcrete()")
    eval("toPoly" + lod).SetInputConnection(
      eval("elevation" + lod).GetOutputPort())

    exec("normals" + lod + "  = vtkPolyDataNormals()")
    eval("normals" + lod).SetInputConnection(
      eval("toPoly" + lod).GetOutputPort())
    eval("normals" + lod).SetFeatureAngle(60)
    eval("normals" + lod).ConsistencyOff()
    eval("normals" + lod).SplittingOff()
    eval("normals" + lod).ReleaseDataFlagOn()

    exec("demMapper" + lod + "  = vtkPolyDataMapper()")
    eval("demMapper" + lod).SetInputConnection(
      eval("normals" + lod).GetOutputPort())
    eval("demMapper" + lod).SetScalarRange(lo, hi)
    eval("demMapper" + lod).SetLookupTable(lut)
    eval("demMapper" + lod).Update()

    demActor.AddLODMapper(eval("demMapper" + lod))

# Add the actors to the renderer, set the background and size
#
ren1.AddActor(demActor)
ren1.SetBackground(.4, .4, .4)
iren.SetDesiredUpdateRate(1)

def TkCheckAbort(obj=None, event=""):
    if renWin.GetEventPending():
        renWin.SetAbortRender(1)

renWin.AddObserver("AbortCheckEvent", TkCheckAbort)

ren1.GetActiveCamera().SetViewUp(0, 0, 1)
ren1.GetActiveCamera().SetPosition(-99900, -21354, 131801)
ren1.GetActiveCamera().SetFocalPoint(41461, 41461, 2815)
ren1.ResetCamera()
ren1.GetActiveCamera().Dolly(1.2)
ren1.ResetCameraClippingRange()

renWin.Render()

#iren.Start()
