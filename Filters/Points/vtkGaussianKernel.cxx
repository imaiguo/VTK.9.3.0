// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkGaussianKernel.h"
#include "vtkAbstractPointLocator.h"
#include "vtkDataSet.h"
#include "vtkDoubleArray.h"
#include "vtkIdList.h"
#include "vtkMath.h"
#include "vtkMathUtilities.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkGaussianKernel);

//------------------------------------------------------------------------------
vtkGaussianKernel::vtkGaussianKernel()
{
  this->Sharpness = 2.0;
  this->F2 = this->Sharpness / this->Radius;
}

//------------------------------------------------------------------------------
vtkGaussianKernel::~vtkGaussianKernel() = default;

//------------------------------------------------------------------------------
void vtkGaussianKernel::Initialize(vtkAbstractPointLocator* loc, vtkDataSet* ds, vtkPointData* pd)
{
  this->Superclass::Initialize(loc, ds, pd);

  this->F2 = this->Sharpness / this->Radius;
  this->F2 = this->F2 * this->F2;
}

//------------------------------------------------------------------------------
vtkIdType vtkGaussianKernel::ComputeWeights(
  double x[3], vtkIdList* pIds, vtkDoubleArray* prob, vtkDoubleArray* weights)
{
  vtkIdType numPts = pIds->GetNumberOfIds();
  double d2, y[3], sum = 0.0;
  weights->SetNumberOfTuples(numPts);
  double* p = (prob ? prob->GetPointer(0) : nullptr);
  double* w = weights->GetPointer(0);
  double f2 = this->F2;

  for (vtkIdType i = 0; i < numPts; ++i)
  {
    vtkIdType id = pIds->GetId(i);
    this->DataSet->GetPoint(id, y);
    d2 = vtkMath::Distance2BetweenPoints(x, y);

    if (vtkMathUtilities::FuzzyCompare(
          d2, 0.0, std::numeric_limits<double>::epsilon() * 256.0)) // precise hit on existing point
    {
      pIds->SetNumberOfIds(1);
      pIds->SetId(0, id);
      weights->SetNumberOfTuples(1);
      weights->SetValue(0, 1.0);
      return 1;
    }
    else
    {
      w[i] = (p ? p[i] * exp(-f2 * d2) : exp(-f2 * d2));
      sum += w[i];
    }
  } // over all points

  // Normalize
  if (this->NormalizeWeights && sum != 0.0)
  {
    for (vtkIdType i = 0; i < numPts; ++i)
    {
      w[i] /= sum;
    }
  }

  return numPts;
}

//------------------------------------------------------------------------------
void vtkGaussianKernel::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "Sharpness: " << this->GetSharpness() << endl;
}
VTK_ABI_NAMESPACE_END
