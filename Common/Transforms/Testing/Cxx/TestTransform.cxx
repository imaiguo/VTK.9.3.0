// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include <iostream>

#include "vtkSmartPointer.h"
#include "vtkTransform.h"

// forward declare test subroutines
int testUseOfInverse();
int testConcatenationIdentity();

int TestTransform(int, char*[])
{
  int numErrors = 0;

  numErrors += testUseOfInverse();
  numErrors += testConcatenationIdentity();

  return (numErrors > 0) ? 1 : 0;
}

// This is a regression test for a bug where th following code produced
// a segfault.  As long as this code does not produce a segfault,
// consider it to have passed the test.
int testUseOfInverse()
{
  vtkSmartPointer<vtkTransform> trans1 = vtkSmartPointer<vtkTransform>::New();
  vtkSmartPointer<vtkTransform> trans2 = vtkSmartPointer<vtkTransform>::New();
  vtkSmartPointer<vtkTransform> trans3 = vtkSmartPointer<vtkTransform>::New();
  trans1->Identity();
  trans2->Identity();
  trans2->PostMultiply();
  trans3->Identity();
  double a[] = { 3, 4, 5 }, b[3];
  // get inverses for 2 and 3
  vtkSmartPointer<vtkLinearTransform> inv2 = trans2->GetLinearInverse();
  vtkSmartPointer<vtkLinearTransform> inv3 = trans3->GetLinearInverse();
  for (int i = 0; i < 30; i++)
  {
    // make the transform something easy
    trans2->Translate(a);
    trans2->RotateX(4);
    trans2->RotateY(i % 90);
    // transform some stuff
    inv2->TransformVector(a, b);
    inv2->TransformPoint(a, b);
    // build a transform with concatenations including an inverse
    trans2->Identity();
    trans2->Concatenate(trans1);
    trans2->Concatenate(inv3);
    // transform some stuff
    inv2->TransformVector(a, b);
    inv2->TransformPoint(a, b);
    // print i
    trans2->Identity();
    std::cout << "Iteration: " << i << " Reference Count: " << inv3->GetReferenceCount()
              << std::endl;
  }
  return 0;
}

// This is a regression test for a bug where th following code produced
// a segfault.  As long as this code does not produce a segfault,
// consider it to have passed the test.
int testConcatenationIdentity()
{
  vtkSmartPointer<vtkTransform> trans1 = vtkSmartPointer<vtkTransform>::New();
  // Add 2 transforms
  trans1->Scale(2., 2., 2.);
  trans1->Scale(2., 2., 2.);
  trans1->Identity();

  vtkSmartPointer<vtkTransform> trans2 = vtkSmartPointer<vtkTransform>::New();
  trans2->Scale(2., 2., 2.);

  trans1->DeepCopy(trans2);
  return 0;
}
