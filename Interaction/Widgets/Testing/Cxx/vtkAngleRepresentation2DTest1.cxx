// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkAngleRepresentation2D.h"

#include <cstdlib>
#include <iostream>

#include "WidgetTestingMacros.h"

int vtkAngleRepresentation2DTest1(int, char*[])
{
  vtkSmartPointer<vtkAngleRepresentation2D> node1 =
    vtkSmartPointer<vtkAngleRepresentation2D>::New();

  EXERCISE_BASIC_ANGLE_REPRESENTATION_METHODS(vtkAngleRepresentation2D, node1);

  vtkLeaderActor2D* actor = node1->GetRay1();
  if (actor == nullptr)
  {
    std::cout << "Ray 1 is null." << std::endl;
  }
  actor = node1->GetRay2();
  if (actor == nullptr)
  {
    std::cout << "Ray 2 is null." << std::endl;
  }
  actor = node1->GetArc();
  if (actor == nullptr)
  {
    std::cout << "Arc is null." << std::endl;
  }
  return EXIT_SUCCESS;
}
