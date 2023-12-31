#!/usr/bin/env python
from vtkmodules.vtkCommonCore import (
    vtkIdList,
    vtkPoints,
)
from vtkmodules.vtkCommonDataModel import (
    vtkGenericCell,
    vtkStructuredGrid,
)
from vtkmodules.vtkFiltersGeometry import vtkStructuredGridGeometryFilter
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

# Remove cullers so single vertex will render
ren1 = vtkRenderer()
ren1.GetCullers().RemoveAllItems()
renWin = vtkRenderWindow()
renWin.AddRenderer(ren1)
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
cell = vtkGenericCell()
ptIds = vtkIdList()
# 0D
ZeroDPts = vtkPoints()
ZeroDPts.SetNumberOfPoints(1)
ZeroDPts.SetPoint(0,0,0,0)
ZeroDGrid = vtkStructuredGrid()
ZeroDGrid.SetDimensions(1,1,1)
ZeroDGrid.SetPoints(ZeroDPts)
ZeroDGrid.GetCell(0)
ZeroDGrid.GetCell(0,cell)
ZeroDGrid.GetCellPoints(0,ptIds)
ZeroDGeom = vtkStructuredGridGeometryFilter()
ZeroDGeom.SetInputData(ZeroDGrid)
ZeroDGeom.SetExtent(0,2,0,2,0,2)
ZeroDMapper = vtkPolyDataMapper()
ZeroDMapper.SetInputConnection(ZeroDGeom.GetOutputPort())
ZeroDActor = vtkActor()
ZeroDActor.SetMapper(ZeroDMapper)
ZeroDActor.SetPosition(0,0,0)
ren1.AddActor(ZeroDActor)
# 1D - X
XPts = vtkPoints()
XPts.SetNumberOfPoints(2)
XPts.SetPoint(0,0,0,0)
XPts.SetPoint(1,1,0,0)
XGrid = vtkStructuredGrid()
XGrid.SetDimensions(2,1,1)
XGrid.SetPoints(XPts)
XGrid.GetCell(0)
XGrid.GetCell(0,cell)
XGrid.GetCellPoints(0,ptIds)
XGeom = vtkStructuredGridGeometryFilter()
XGeom.SetInputData(XGrid)
XGeom.SetExtent(0,2,0,2,0,2)
XMapper = vtkPolyDataMapper()
XMapper.SetInputConnection(XGeom.GetOutputPort())
XActor = vtkActor()
XActor.SetMapper(XMapper)
XActor.SetPosition(2,0,0)
ren1.AddActor(XActor)
# 1D - Y
YPts = vtkPoints()
YPts.SetNumberOfPoints(2)
YPts.SetPoint(0,0,0,0)
YPts.SetPoint(1,0,1,0)
YGrid = vtkStructuredGrid()
YGrid.SetDimensions(1,2,1)
YGrid.SetPoints(YPts)
YGrid.GetCell(0)
YGrid.GetCell(0,cell)
YGrid.GetCellPoints(0,ptIds)
YGeom = vtkStructuredGridGeometryFilter()
YGeom.SetInputData(YGrid)
YGeom.SetExtent(0,2,0,2,0,2)
YMapper = vtkPolyDataMapper()
YMapper.SetInputConnection(YGeom.GetOutputPort())
YActor = vtkActor()
YActor.SetMapper(YMapper)
YActor.SetPosition(4,0,0)
ren1.AddActor(YActor)
# 1D - Z
ZPts = vtkPoints()
ZPts.SetNumberOfPoints(2)
ZPts.SetPoint(0,0,0,0)
ZPts.SetPoint(1,0,0,1)
ZGrid = vtkStructuredGrid()
ZGrid.SetDimensions(1,1,2)
ZGrid.SetPoints(ZPts)
ZGrid.GetCell(0)
ZGrid.GetCell(0,cell)
ZGrid.GetCellPoints(0,ptIds)
ZGeom = vtkStructuredGridGeometryFilter()
ZGeom.SetInputData(ZGrid)
ZGeom.SetExtent(0,2,0,2,0,2)
ZMapper = vtkPolyDataMapper()
ZMapper.SetInputConnection(ZGeom.GetOutputPort())
ZActor = vtkActor()
ZActor.SetMapper(ZMapper)
ZActor.SetPosition(6,0,0)
ren1.AddActor(ZActor)
# 2D - XY
XYPts = vtkPoints()
XYPts.SetNumberOfPoints(4)
XYPts.SetPoint(0,0,0,0)
XYPts.SetPoint(1,1,0,0)
XYPts.SetPoint(2,0,1,0)
XYPts.SetPoint(3,1,1,0)
XYGrid = vtkStructuredGrid()
XYGrid.SetDimensions(2,2,1)
XYGrid.SetPoints(XYPts)
XYGrid.GetCell(0)
XYGrid.GetCell(0,cell)
XYGrid.GetCellPoints(0,ptIds)
XYGeom = vtkStructuredGridGeometryFilter()
XYGeom.SetInputData(XYGrid)
XYGeom.SetExtent(0,2,0,2,0,2)
XYMapper = vtkPolyDataMapper()
XYMapper.SetInputConnection(XYGeom.GetOutputPort())
XYActor = vtkActor()
XYActor.SetMapper(XYMapper)
XYActor.SetPosition(0,2,0)
ren1.AddActor(XYActor)
# 2D - YZ
YZPts = vtkPoints()
YZPts.SetNumberOfPoints(4)
YZPts.SetPoint(0,0,0,0)
YZPts.SetPoint(1,0,1,0)
YZPts.SetPoint(2,0,0,1)
YZPts.SetPoint(3,0,1,1)
YZGrid = vtkStructuredGrid()
YZGrid.SetDimensions(1,2,2)
YZGrid.SetPoints(YZPts)
YZGrid.GetCell(0)
YZGrid.GetCell(0,cell)
YZGrid.GetCellPoints(0,ptIds)
YZGeom = vtkStructuredGridGeometryFilter()
YZGeom.SetInputData(YZGrid)
YZGeom.SetExtent(0,2,0,2,0,2)
YZMapper = vtkPolyDataMapper()
YZMapper.SetInputConnection(YZGeom.GetOutputPort())
YZActor = vtkActor()
YZActor.SetMapper(YZMapper)
YZActor.SetPosition(2,2,0)
ren1.AddActor(YZActor)
# 2D - XZ
XZPts = vtkPoints()
XZPts.SetNumberOfPoints(4)
XZPts.SetPoint(0,0,0,0)
XZPts.SetPoint(1,1,0,0)
XZPts.SetPoint(2,0,0,1)
XZPts.SetPoint(3,1,0,1)
XZGrid = vtkStructuredGrid()
XZGrid.SetDimensions(2,1,2)
XZGrid.SetPoints(XZPts)
XZGrid.GetCell(0)
XZGrid.GetCell(0,cell)
XZGrid.GetCellPoints(0,ptIds)
XZGeom = vtkStructuredGridGeometryFilter()
XZGeom.SetInputData(XZGrid)
XZGeom.SetExtent(0,2,0,2,0,2)
XZMapper = vtkPolyDataMapper()
XZMapper.SetInputConnection(XZGeom.GetOutputPort())
XZActor = vtkActor()
XZActor.SetMapper(XZMapper)
XZActor.SetPosition(4,2,0)
ren1.AddActor(XZActor)
# 3D
XYZPts = vtkPoints()
XYZPts.SetNumberOfPoints(8)
XYZPts.SetPoint(0,0,0,0)
XYZPts.SetPoint(1,1,0,0)
XYZPts.SetPoint(2,0,1,0)
XYZPts.SetPoint(3,1,1,0)
XYZPts.SetPoint(4,0,0,1)
XYZPts.SetPoint(5,1,0,1)
XYZPts.SetPoint(6,0,1,1)
XYZPts.SetPoint(7,1,1,1)
XYZGrid = vtkStructuredGrid()
XYZGrid.SetDimensions(2,2,2)
XYZGrid.SetPoints(XYZPts)
XYZGrid.GetCell(0)
XYZGrid.GetCell(0,cell)
XYZGrid.GetCellPoints(0,ptIds)
XYZGeom = vtkStructuredGridGeometryFilter()
XYZGeom.SetInputData(XYZGrid)
XYZGeom.SetExtent(0,2,0,2,0,2)
XYZMapper = vtkPolyDataMapper()
XYZMapper.SetInputConnection(XYZGeom.GetOutputPort())
XYZActor = vtkActor()
XYZActor.SetMapper(XYZMapper)
XYZActor.SetPosition(6,2,0)
ren1.AddActor(XYZActor)
# render the image
#
renWin.SetSize(300,150)
cam1 = ren1.GetActiveCamera()
cam1.SetClippingRange(2.27407,14.9819)
cam1.SetFocalPoint(3.1957,1.74012,0.176603)
cam1.SetPosition(-0.380779,6.13894,5.59404)
cam1.SetViewUp(0.137568,0.811424,-0.568037)
renWin.Render()
iren.Initialize()
# prevent the tk window from showing up then start the event loop
# --- end of script --
