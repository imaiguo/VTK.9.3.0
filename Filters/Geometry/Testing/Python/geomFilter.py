#!/usr/bin/env python
from vtkmodules.vtkCommonCore import (
    vtkIntArray,
    vtkPoints,
)
from vtkmodules.vtkCommonDataModel import (
    VTK_LINE,
    VTK_VERTEX,
    vtkSphere,
    vtkUnstructuredGrid,
)
from vtkmodules.vtkFiltersExtraction import vtkExtractGeometry
from vtkmodules.vtkFiltersGeometry import vtkGeometryFilter
from vtkmodules.vtkIOLegacy import vtkRectilinearGridReader
from vtkmodules.vtkIOParallel import vtkMultiBlockPLOT3DReader
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
import sys

# create pipeline - structured grid
#
pl3d = vtkMultiBlockPLOT3DReader()
pl3d.SetXYZFileName(VTK_DATA_ROOT + "/Data/combxyz.bin")
pl3d.SetQFileName(VTK_DATA_ROOT + "/Data/combq.bin")
pl3d.SetScalarFunctionNumber(100)
pl3d.SetVectorFunctionNumber(202)
pl3d.Update()
output = pl3d.GetOutput().GetBlock(0)
gf = vtkGeometryFilter()
gf.SetInputData(output)
gMapper = vtkPolyDataMapper()
gMapper.SetInputConnection(gf.GetOutputPort())
gMapper.SetScalarRange(output.GetScalarRange())
gActor = vtkActor()
gActor.SetMapper(gMapper)
gf2 = vtkGeometryFilter()
gf2.SetInputData(output)
gf2.ExtentClippingOn()
gf2.SetExtent(10,17,-6,6,23,37)
gf2.PointClippingOn()
gf2.SetPointMinimum(0)
gf2.SetPointMaximum(10000)
gf2.CellClippingOn()
gf2.SetCellMinimum(0)
gf2.SetCellMaximum(7500)
g2Mapper = vtkPolyDataMapper()
g2Mapper.SetInputConnection(gf2.GetOutputPort())
g2Mapper.SetScalarRange(output.GetScalarRange())
g2Actor = vtkActor()
g2Actor.SetMapper(g2Mapper)
g2Actor.AddPosition(0,15,0)
# create pipeline - poly data
#
gf3 = vtkGeometryFilter()
gf3.SetInputConnection(gf.GetOutputPort())
g3Mapper = vtkPolyDataMapper()
g3Mapper.SetInputConnection(gf3.GetOutputPort())
g3Mapper.SetScalarRange(output.GetScalarRange())
g3Actor = vtkActor()
g3Actor.SetMapper(g3Mapper)
g3Actor.AddPosition(0,0,15)
gf4 = vtkGeometryFilter()
gf4.SetInputConnection(gf2.GetOutputPort())
gf4.ExtentClippingOn()
gf4.SetExtent(10,17,-6,6,23,37)
gf4.PointClippingOn()
gf4.SetPointMinimum(0)
gf4.SetPointMaximum(10000)
gf4.CellClippingOn()
gf4.SetCellMinimum(0)
gf4.SetCellMaximum(7500)
g4Mapper = vtkPolyDataMapper()
g4Mapper.SetInputConnection(gf4.GetOutputPort())
g4Mapper.SetScalarRange(output.GetScalarRange())
g4Actor = vtkActor()
g4Actor.SetMapper(g4Mapper)
g4Actor.AddPosition(0,15,15)
# create pipeline - unstructured grid
#
s = vtkSphere()
s.SetCenter(output.GetCenter())
s.SetRadius(100.0)
#everything
eg = vtkExtractGeometry()
eg.SetInputData(output)
eg.SetImplicitFunction(s)
gf5 = vtkGeometryFilter()
gf5.SetInputConnection(eg.GetOutputPort())
g5Mapper = vtkPolyDataMapper()
g5Mapper.SetInputConnection(gf5.GetOutputPort())
g5Mapper.SetScalarRange(output.GetScalarRange())
g5Actor = vtkActor()
g5Actor.SetMapper(g5Mapper)
g5Actor.AddPosition(0,0,30)
gf6 = vtkGeometryFilter()
gf6.SetInputConnection(eg.GetOutputPort())
gf6.ExtentClippingOn()
gf6.SetExtent(10,17,-6,6,23,37)
gf6.PointClippingOn()
gf6.SetPointMinimum(0)
gf6.SetPointMaximum(10000)
gf6.CellClippingOn()
gf6.SetCellMinimum(0)
gf6.SetCellMaximum(7500)
g6Mapper = vtkPolyDataMapper()
g6Mapper.SetInputConnection(gf6.GetOutputPort())
g6Mapper.SetScalarRange(output.GetScalarRange())
g6Actor = vtkActor()
g6Actor.SetMapper(g6Mapper)
g6Actor.AddPosition(0,15,30)
# create pipeline - rectilinear grid
#
rgridReader = vtkRectilinearGridReader()
rgridReader.SetFileName(VTK_DATA_ROOT + "/Data/RectGrid2.vtk")
rgridReader.Update()
gf7 = vtkGeometryFilter()
gf7.SetInputConnection(rgridReader.GetOutputPort())
g7Mapper = vtkPolyDataMapper()
g7Mapper.SetInputConnection(gf7.GetOutputPort())
g7Mapper.SetScalarRange(rgridReader.GetOutput().GetScalarRange())
g7Actor = vtkActor()
g7Actor.SetMapper(g7Mapper)
g7Actor.SetScale(3,3,3)
gf8 = vtkGeometryFilter()
gf8.SetInputConnection(rgridReader.GetOutputPort())
gf8.ExtentClippingOn()
gf8.SetExtent(0,1,-2,2,0,4)
gf8.PointClippingOn()
gf8.SetPointMinimum(0)
gf8.SetPointMaximum(10000)
gf8.CellClippingOn()
gf8.SetCellMinimum(0)
gf8.SetCellMaximum(7500)
g8Mapper = vtkPolyDataMapper()
g8Mapper.SetInputConnection(gf8.GetOutputPort())
g8Mapper.SetScalarRange(rgridReader.GetOutput().GetScalarRange())
g8Actor = vtkActor()
g8Actor.SetMapper(g8Mapper)
g8Actor.SetScale(3,3,3)
g8Actor.AddPosition(0,15,0)
# Create the RenderWindow, Renderer and both Actors
#
ren1 = vtkRenderer()
renWin = vtkRenderWindow()
renWin.AddRenderer(ren1)
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
ren1.AddActor(gActor)
ren1.AddActor(g2Actor)
ren1.AddActor(g3Actor)
ren1.AddActor(g4Actor)
ren1.AddActor(g5Actor)
ren1.AddActor(g6Actor)
ren1.AddActor(g7Actor)
ren1.AddActor(g8Actor)
renWin.SetSize(340,550)
cam1 = ren1.GetActiveCamera()
cam1.SetClippingRange(84,174)
cam1.SetFocalPoint(5.22824,6.09412,35.9813)
cam1.SetPosition(100.052,62.875,102.818)
cam1.SetViewUp(-0.307455,-0.464269,0.830617)
iren.Initialize()
# prevent the tk window from showing up then start the event loop

# test that the cell data is properly mapped in the output
ug = vtkUnstructuredGrid()
p = vtkPoints()
p.InsertNextPoint(0, 0, 0)
p.InsertNextPoint(1, 0, 0)
p.InsertNextPoint(2, 0, 0)
p.InsertNextPoint(3, 0, 0)
ug.SetPoints(p)
ug.GetNumberOfPoints()
ug.Allocate(4)
lpts = [0, 1]
ug.InsertNextCell(VTK_LINE, 2, lpts)
vpts = [1]
ug.InsertNextCell(VTK_VERTEX, 1, vpts)
lpts = [2, 3]
ug.InsertNextCell(VTK_LINE, 2, lpts)
vpts = [3]
ug.InsertNextCell(VTK_VERTEX, 1, vpts)
aa = vtkIntArray()
aa.InsertNextValue(0)
aa.InsertNextValue(1)
aa.InsertNextValue(2)
aa.InsertNextValue(3)
aa.SetName('testarray')
ug.GetCellData().AddArray(aa)
gf = vtkGeometryFilter()
gf.SetInputData(ug)
gf.Update()
pd = gf.GetOutput()
oa = pd.GetCellData().GetArray('testarray')

# Check that the ordering of polydata arrays is correct. Verts should come before
# lines.
correctcelldata = [1, 3, 0, 2]
if oa.GetValue(0) != correctcelldata[0] and oa.GetValue(0) != correctcelldata[1]:
    print('Bad celldata of test array')
    sys.exit(1)
if oa.GetValue(1) != correctcelldata[0] and oa.GetValue(1) != correctcelldata[1]:
    print('Bad celldata of test array')
    sys.exit(1)
if oa.GetValue(2) != correctcelldata[2] and oa.GetValue(2) != correctcelldata[3]:
    print('Bad celldata of test array')
    sys.exit(1)
if oa.GetValue(3) != correctcelldata[2] and oa.GetValue(3) != correctcelldata[3]:
    print('Bad celldata of test array')
    sys.exit(1)
# --- end of script --
