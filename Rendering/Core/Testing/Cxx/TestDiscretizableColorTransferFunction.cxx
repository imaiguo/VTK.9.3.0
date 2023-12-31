// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkDiscretizableColorTransferFunction.h"
#include "vtkSmartPointer.h"

//------------------------------------------------------------------------------
int TestDiscretizableColorTransferFunction(int, char*[])
{
  double controlPoints[] = { 0.0, 1.0, 0.0, 0.0, 0.5, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 1.5, 0.0,
    1.0, 1.0 };

  vtkSmartPointer<vtkDiscretizableColorTransferFunction> dctf =
    vtkSmartPointer<vtkDiscretizableColorTransferFunction>::New();
  for (int i = 0; i < 4; ++i)
  {
    double* xrgb = controlPoints + (i * 4);
    dctf->AddRGBPoint(xrgb[0], xrgb[1], xrgb[2], xrgb[3]);
  }

  double* allPoints = dctf->GetDataPointer();

  for (int ptIdx = 0; ptIdx < 4; ++ptIdx) // Iterate over control points
  {
    for (int i = 0; i < 4; ++i)
    {
      if (allPoints[4 * ptIdx + i] != controlPoints[4 * ptIdx + i])
      {
        std::cerr << "GetRGBPoints() failed at control point " << ptIdx << " for component " << i
                  << std::endl;
        std::cerr << "controlPoint: " << controlPoints[4 * ptIdx + i] << ", "
                  << "GetRGBPoints(): " << allPoints[4 * ptIdx + i] << "\n";
        return EXIT_FAILURE;
      }
    }
  }

  return EXIT_SUCCESS;
}
