#!/usr/bin/env python
from vtkmodules.vtkCommonCore import vtkPoints
from vtkmodules.vtkCommonDataModel import (
    vtkCellArray,
    vtkPolyData,
)
from vtkmodules.vtkFiltersModeling import vtkContourLoopExtraction
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

# Create the RenderWindow, Renderer
#
ren = vtkRenderer()
renWin = vtkRenderWindow()
renWin.AddRenderer( ren )

iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)

# Create test data. Some simple incomplete loops that touch the boundary.
#
loopData = vtkPolyData()
loopPts = vtkPoints()
loopLines = vtkCellArray()
loopData.SetPoints(loopPts)
loopData.SetLines(loopLines)

loopPts.InsertPoint(0, -1, -2, 0)
loopPts.InsertPoint(1, 1, -2, 0)
loopPts.InsertPoint(2, -1, -1, 0)
loopPts.InsertPoint(3, 1, -1, 0)

loopPts.InsertPoint(4, -2, 0, 0)
loopPts.InsertPoint(5, -1, 0, 0)
loopPts.InsertPoint(6, -1, 0.5, 0)
loopPts.InsertPoint(7, -1, 1, 0)
loopPts.InsertPoint(8, -2, 1, 0)

# Along x-bottom boundary
loopLines.InsertNextCell(2)
loopLines.InsertCellPoint(0)
loopLines.InsertCellPoint(2)

loopLines.InsertNextCell(2)
loopLines.InsertCellPoint(3)
loopLines.InsertCellPoint(1)

loopLines.InsertNextCell(2)
loopLines.InsertCellPoint(2)
loopLines.InsertCellPoint(3)

# Along y-left-side boundary
loopLines.InsertNextCell(3)
loopLines.InsertCellPoint(4)
loopLines.InsertCellPoint(5)
loopLines.InsertCellPoint(6)

loopLines.InsertNextCell(2)
loopLines.InsertCellPoint(6)
loopLines.InsertCellPoint(7)

loopLines.InsertNextCell(2)
loopLines.InsertCellPoint(7)
loopLines.InsertCellPoint(8)

contours = vtkContourLoopExtraction()
contours.SetInputData(loopData)

mapper = vtkPolyDataMapper()
mapper.SetInputConnection(contours.GetOutputPort())
#mapper.SetInputData(loopData)

actor = vtkActor()
actor.SetMapper(mapper)

ren.AddActor(actor)

renWin.Render()
iren.Start()
