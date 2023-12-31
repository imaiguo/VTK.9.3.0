// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
// .NAME Test of vtkOBJReader
// .SECTION Description
//

#include "vtkDebugLeaks.h"
#include "vtkOBJReader.h"

#include "vtkCellArray.h"
#include "vtkPointData.h"
#include "vtkSmartPointer.h"
#include "vtkTestUtilities.h"

int TestOBJReaderNormalsTCoords(int argc, char* argv[])
{
  int retVal = 0;

  // Create the reader.
  char* fname =
    vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/obj_with_normals_and_tcoords.obj");
  vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
  reader->SetFileName(fname);
  reader->Update();
  delete[] fname;

  vtkPolyData* data = reader->GetOutput();

  if (!data)
  {
    std::cerr << "Could not read data" << std::endl;
    return 1;
  }
  if (data->GetNumberOfPoints() != 4)
  {
    std::cerr << "Invalid number of points" << std::endl;
    return 1;
  }
  if (data->GetPointData()->GetNumberOfArrays() != 2)
  {
    std::cerr << "Invalid number of arrays" << std::endl;
    return 1;
  }
  if (!data->GetPointData()->HasArray("TCoords"))
  {
    std::cerr << "Could not find TCoords array" << std::endl;
    return 1;
  }
  if (!data->GetPointData()->HasArray("Normals"))
  {
    std::cerr << "Could not find TCoords array" << std::endl;
    return 1;
  }

  return retVal;
}
