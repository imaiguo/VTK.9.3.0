#!/usr/bin/env python
from vtkmodules.vtkCommonCore import vtkLookupTable
from vtkmodules.vtkFiltersCore import vtkContourFilter
from vtkmodules.vtkIOImage import (
    vtkImageReader,
    vtkMetaImageReader,
    vtkMetaImageWriter,
)
from vtkmodules.vtkRenderingCore import (
    vtkActor,
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

Ren1 = vtkRenderer()
Ren1.SetBackground(0.33,0.35,0.43)
renWin = vtkRenderWindow()
renWin.AddRenderer(Ren1)
renWin.SetSize(300,300)
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
reader = vtkImageReader()
reader.SetDataByteOrderToLittleEndian()
reader.SetDataExtent(0,63,0,63,1,93)
reader.SetDataSpacing(3.2,3.2,1.5)
reader.SetFilePrefix(VTK_DATA_ROOT + "/Data/headsq/quarter")
reader.SetDataMask(0x7fff)
reader.Update()
pvTemp200 = vtkMetaImageWriter()
pvTemp200.SetFileName("mhdWriter.mhd")
pvTemp200.SetInputData(reader.GetOutput())
pvTemp200.Write()
pvTemp90 = vtkMetaImageReader()
pvTemp90.SetFileName("mhdWriter.mhd")
pvTemp90.Update()
pvTemp109 = vtkLookupTable()
pvTemp109.SetNumberOfTableValues(256)
pvTemp109.SetHueRange(0.6667,0)
pvTemp109.SetSaturationRange(1,1)
pvTemp109.SetValueRange(1,1)
pvTemp109.SetTableRange(37.3531,260)
pvTemp109.SetVectorComponent(0)
pvTemp109.Build()
pvTemp110 = vtkContourFilter()
pvTemp110.SetInputData(pvTemp90.GetOutput(0))
pvTemp110.SetValue(0,1150)
pvTemp110.SetComputeNormals(1)
pvTemp110.SetComputeGradients(0)
pvTemp110.SetComputeScalars(0)
pvTemp114 = vtkPolyDataMapper()
pvTemp114.SetInputConnection(pvTemp110.GetOutputPort())
pvTemp114.SetScalarRange(0,1)
pvTemp114.UseLookupTableScalarRangeOn()
pvTemp114.SetScalarVisibility(1)
pvTemp114.SetScalarModeToUsePointFieldData()
pvTemp114.SelectColorArray("ImageFile")
pvTemp114.SetLookupTable(pvTemp109)
pvTemp115 = vtkActor()
pvTemp115.SetMapper(pvTemp114)
pvTemp115.GetProperty().SetRepresentationToSurface()
pvTemp115.GetProperty().SetInterpolationToGouraud()
pvTemp115.GetProperty().SetAmbient(0)
pvTemp115.GetProperty().SetDiffuse(1)
pvTemp115.GetProperty().SetSpecular(0)
pvTemp115.GetProperty().SetSpecularPower(1)
pvTemp115.GetProperty().SetSpecularColor(1,1,1)
Ren1.AddActor(pvTemp115)
iren.Initialize()
# prevent the tk window from showing up then start the event loop
# --- end of script --
