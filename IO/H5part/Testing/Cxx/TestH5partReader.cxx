// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkH5PartReader.h"
#include "vtkNew.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkTestUtilities.h"

int TestH5partReader(int argc, char* argv[])
{
  char* fname = vtkTestUtilities::ExpandDataFileName(argc, argv, "Data/H5part/sample.h5part");
  vtkNew<vtkH5PartReader> reader;
  reader->SetFileName(fname);
  delete[] fname;

  reader->UpdateInformation();

  if (strcmp(reader->GetXarray(), "x") != 0 || strcmp(reader->GetYarray(), "y") != 0 ||
    strcmp(reader->GetZarray(), "z") != 0)
  {
    cerr << "ERROR: incorrect arrays chosen by default!" << endl;
    return EXIT_FAILURE;
  }

  reader->EnableAll();
  reader->Update();

  auto output = reader->GetOutput();
  if (output->GetNumberOfPoints() != 1000)
  {
    cerr << "ERROR: mismatched point count" << endl;
    return EXIT_FAILURE;
  }

  if (output->GetPointData()->GetNumberOfArrays() != 7)
  {
    cerr << "ERROR: mismatched array count" << endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
