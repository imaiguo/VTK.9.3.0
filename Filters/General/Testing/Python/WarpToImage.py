#!/usr/bin/env python
from vtkmodules.vtkFiltersGeneral import vtkWarpTo
from vtkmodules.vtkImagingCore import vtkRTAnalyticSource
from vtkmodules.vtkRenderingCore import (
    vtkActor,
    vtkDataSetMapper,
    vtkRenderWindow,
    vtkRenderer,
)
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

# create a rendering window
renWin = vtkRenderWindow()
renWin.SetMultiSamples(0)
renWin.SetSize(200,200)
wavelet = vtkRTAnalyticSource()
wavelet.SetWholeExtent(-100,100,-100,100,0,0)
wavelet.SetCenter(0,0,0)
wavelet.SetMaximum(255)
wavelet.SetStandardDeviation(.5)
wavelet.SetXFreq(60)
wavelet.SetYFreq(30)
wavelet.SetZFreq(40)
wavelet.SetXMag(10)
wavelet.SetYMag(18)
wavelet.SetZMag(5)
wavelet.SetSubsampleRate(1)
warp = vtkWarpTo()
warp.SetInputConnection(wavelet.GetOutputPort())
warp.SetScaleFactor(0)
warp.SetPosition(0,0,0)
mapper = vtkDataSetMapper()
mapper.SetInputConnection(warp.GetOutputPort())
mapper.SetScalarRange(75,290)
actor = vtkActor()
actor.SetMapper(mapper)
renderer = vtkRenderer()
renderer.AddActor(actor)
renderer.ResetCamera()
renWin.AddRenderer(renderer)
warp.SetScaleFactor(0.25)
warp.SetPosition(75,75,0)
warp.Update()
# --- end of script --
