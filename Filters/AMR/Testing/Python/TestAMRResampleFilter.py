#!/usr/bin/env python

# This tests vtkAMRResampleFilter

from vtkmodules.vtkFiltersAMR import vtkAMRResampleFilter
from vtkmodules.vtkIOAMR import vtkAMREnzoReader
from vtkmodules.test import Testing
from vtkmodules.util.misc import vtkGetDataRoot

VTK_DATA_ROOT = vtkGetDataRoot()

class TestAMRResampleFilter(Testing.vtkTest):
  def testAMR(self):
    filename= VTK_DATA_ROOT +"/Data/AMR/Enzo/DD0010/moving7_0010.hierarchy"

    reader = vtkAMREnzoReader()
    reader.SetFileName(filename)
    reader.SetMaxLevel(10)
    reader.SetCellArrayStatus("TotalEnergy",1)

    filter = vtkAMRResampleFilter()
    filter.SetMin([0.2,0.2,0])
    filter.SetMax([0.8,0.8,1])
    filter.SetNumberOfSamples([30,30,30])
    filter.SetDemandDrivenMode(1)

    filter.SetInputConnection(reader.GetOutputPort())
    filter.Update()

    out = filter.GetOutputDataObject(0).GetBlock(0)
    self.assertEqual(out.GetNumberOfPoints(),27000)
    data = out.GetPointData().GetArray("TotalEnergy")
    minV = data.GetTuple(0)[0]
    maxV = data.GetTuple(0)[0]
    for i in range(out.GetNumberOfPoints()):
      v = data.GetTuple(i)[0]
      minV = min(v,minV)
      maxV = max(v,maxV)

    noError = abs(maxV*100000 - 201)<1
    self.assertEqual(noError,True)

if __name__ == "__main__":
    Testing.main([(TestAMRResampleFilter, 'test')])
