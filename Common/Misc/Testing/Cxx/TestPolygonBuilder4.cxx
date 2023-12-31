// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkIdListCollection.h"
#include "vtkNew.h"
#include "vtkPoints.h"
#include "vtkPolygonBuilder.h"
#include "vtkSmartPointer.h"

int TestPolygonBuilder4(int, char*[])
{

  vtkPolygonBuilder builder;
  vtkNew<vtkIdListCollection> polys;

  vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
  vtkIdType a = points->InsertNextPoint(0, 0, 0);
  vtkIdType b = points->InsertNextPoint(1, 0, 0);
  vtkIdType c = points->InsertNextPoint(0, 1, 0);
  vtkIdType d = points->InsertNextPoint(1, 1, 0);

  // two counter-rotated triangles give unexpected results
#define NTRIANGLES 4
  vtkIdType triangles[NTRIANGLES][3] = { { b, c, a }, { d, c, b }, { c, b, a }, { d, b, c } };

  vtkIdType p[3];
  for (size_t i = 0; i < NTRIANGLES; i++)
  {
    for (size_t j = 0; j < 3; j++)
    {
      p[j] = triangles[i][j];
    }
    builder.InsertTriangle(p);
  }

  builder.GetPolygons(polys);

  vtkIdType expected(1);
  if (polys->GetNumberOfItems() != 1) // and a-b-c-d expected
  {
    vtkGenericWarningMacro(<< "number of items is " << polys->GetNumberOfItems() << " but expected "
                           << expected << endl);
    return EXIT_FAILURE;
  }

  vtkIdList* poly = polys->GetItem(0);
  expected = 4;
  if (poly->GetNumberOfIds() != 4)
  {
    vtkGenericWarningMacro(<< "number of ids is " << poly->GetNumberOfIds() << " but expected "
                           << expected << endl);
    return EXIT_FAILURE;
  }
  poly->Delete();
  polys->RemoveAllItems();

  return EXIT_SUCCESS;
}
