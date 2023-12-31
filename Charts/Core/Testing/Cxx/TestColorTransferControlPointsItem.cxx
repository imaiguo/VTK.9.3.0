// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

// Charts includes
#include "vtkColorTransferControlPointsItem.h"
#include "vtkColorTransferFunction.h"
#include "vtkControlPointsItem.h"

// Common includes
#include "vtkIdTypeArray.h"
#include "vtkNew.h"
#include "vtkSmartPointer.h"

// STD includes
#include <iostream>

//------------------------------------------------------------------------------
int TestColorTransferControlPointsItem(int, char*[])
{
  vtkNew<vtkColorTransferFunction> transferFunction;
  transferFunction->AddHSVSegment(50., 0., 1., 1., 85., 0.3333, 1., 1.);
  transferFunction->AddHSVSegment(85., 0.3333, 1., 1., 170., 0.6666, 1., 1.);
  transferFunction->AddHSVSegment(170., 0.6666, 1., 1., 200., 0., 1., 1.);

  vtkNew<vtkColorTransferControlPointsItem> controlPoints;

  controlPoints->SetColorTransferFunction(transferFunction);

  if (controlPoints->GetColorTransferFunction() != transferFunction)
  {
    std::cerr << "vtkColorTransferControlPointsItem::GetColorTransferFunction "
              << "failed, bad pointer: " << controlPoints->GetColorTransferFunction() << std::endl;
    return EXIT_FAILURE;
  }

  if (controlPoints->GetNumberOfPoints() != 4 || controlPoints->GetNumberOfSelectedPoints() != 0)
  {
    std::cerr << "vtkColorTransferControlPointsItem::SetColorTransferFunction "
              << "failed, wrong number of points: " << controlPoints->GetNumberOfPoints() << ", "
              << controlPoints->GetNumberOfSelectedPoints() << std::endl;
    return EXIT_FAILURE;
  }

  vtkNew<vtkIdTypeArray> ids;
  controlPoints->GetControlPointsIds(ids, true);

  if (ids->GetSize() != controlPoints->GetNumberOfPoints() - 2 || ids->GetValue(0) != 1 ||
    ids->GetValue(1) != 2)
  {
    std::cerr << "vtkControlPointsItem::GetControlPointsIds"
              << "failed, bad array: " << ids->GetSize() << ", " << ids->GetValue(0) << ", "
              << ids->GetValue(1) << std::endl;
    return EXIT_FAILURE;
  }

  controlPoints->GetControlPointsIds(ids);

  if (ids->GetSize() != controlPoints->GetNumberOfPoints() || ids->GetValue(0) != 0 ||
    ids->GetValue(1) != 1 || ids->GetValue(2) != 2 || ids->GetValue(3) != 3)
  {
    std::cerr << "vtkControlPointsItem::GetControlPointsIds"
              << "failed, bad array: " << ids->GetSize() << ", " << ids->GetValue(0) << ", "
              << ids->GetValue(1) << ", " << ids->GetValue(2) << ", " << ids->GetValue(3)
              << std::endl;
    return EXIT_FAILURE;
  }

  double bounds[4];
  controlPoints->GetBounds(bounds);
  if (bounds[0] != 50. || bounds[1] != 200. || bounds[2] != 0.5 || bounds[3] != 0.5)
  {
    std::cerr << "vtkColorTransferControlPointsItem::GetBounds"
              << "failed, wrong bounds: " << bounds[0] << ", " << bounds[1] << ", " << bounds[2]
              << ", " << bounds[3] << std::endl;
    return EXIT_FAILURE;
  }

  // MovePoints
  controlPoints->MovePoints(vtkVector2f(1., 0.), ids);

  double point0[4], point1[4], point2[4], point3[4];
  controlPoints->GetControlPoint(0, point0);
  controlPoints->GetControlPoint(1, point1);
  controlPoints->GetControlPoint(2, point2);
  controlPoints->GetControlPoint(3, point3);
  if (point0[0] != 51. || point1[0] != 86. || point2[0] != 171. || point3[0] != 200.)
  {
    std::cerr << "vtkColorTransferControlPointsItem::MovePoints"
              << "failed, wrong pos: " << point0[0] << ", " << point1[0] << ", " << point2[0]
              << ", " << point3[0] << std::endl;
    return EXIT_FAILURE;
  }

  // SpreadPoints
  controlPoints->SpreadPoints(1., ids);

  controlPoints->GetControlPoint(0, point0);
  controlPoints->GetControlPoint(1, point1);
  controlPoints->GetControlPoint(2, point2);
  controlPoints->GetControlPoint(3, point3);
  if (point0[0] != 51. || point1[0] >= 86. || point2[0] <= 171. || point3[0] != 200.)
  {
    std::cerr << "vtkColorTransferControlPointsItem::SpreadPoints(1)"
              << "failed, wrong pos: " << point0[0] << ", " << point1[0] << ", " << point2[0]
              << ", " << point3[0] << std::endl;
    return EXIT_FAILURE;
  }

  controlPoints->SpreadPoints(-1., ids);

  controlPoints->GetControlPoint(0, point0);
  controlPoints->GetControlPoint(1, point1);
  controlPoints->GetControlPoint(2, point2);
  controlPoints->GetControlPoint(3, point3);
  if (point0[0] != 52. || point1[0] <= 86. || point1[0] >= 87. || point2[0] <= 170. ||
    point2[0] >= 171. || point3[0] != 199.)
  {
    std::cerr << "vtkColorTransferControlPointsItem::SpreadPoints(-1)"
              << "failed, wrong pos: " << point0[0] << ", " << point1[0] << ", " << point2[0]
              << ", " << point3[0] << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
