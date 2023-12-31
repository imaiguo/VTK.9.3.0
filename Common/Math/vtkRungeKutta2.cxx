// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkRungeKutta2.h"

#include "vtkFunctionSet.h"
#include "vtkObjectFactory.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkRungeKutta2);

vtkRungeKutta2::vtkRungeKutta2() = default;

vtkRungeKutta2::~vtkRungeKutta2() = default;

void vtkRungeKutta2::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

// Calculate next time step
int vtkRungeKutta2::ComputeNextStep(double* xprev, double* dxprev, double* xnext, double t,
  double& delT, double& delTActual, double, double, double, double& error, void* userData)
{
  int i, numDerivs, numVals;

  delTActual = 0.;
  error = 0.0;

  if (!this->FunctionSet)
  {
    vtkErrorMacro("No derivative functions are provided!");
    return NOT_INITIALIZED;
  }

  if (!this->Initialized)
  {
    vtkErrorMacro("Integrator not initialized!");
    return NOT_INITIALIZED;
  }

  numDerivs = this->FunctionSet->GetNumberOfFunctions();
  numVals = numDerivs + 1;
  for (i = 0; i < numVals - 1; i++)
  {
    this->Vals[i] = xprev[i];
  }
  this->Vals[numVals - 1] = t;

  // Obtain the derivatives dx_i at x_i
  if (dxprev)
  {
    for (i = 0; i < numDerivs; i++)
    {
      this->Derivs[i] = dxprev[i];
    }
  }
  else if (!this->FunctionSet->FunctionValues(this->Vals, this->Derivs, userData))
  {
    memcpy(xnext, this->Vals, (numVals - 1) * sizeof(double));
    return OUT_OF_DOMAIN;
  }

  // Half-step
  for (i = 0; i < numVals - 1; i++)
  {
    this->Vals[i] = xprev[i] + delT / 2.0 * this->Derivs[i];
  }
  this->Vals[numVals - 1] = t + delT / 2.0;

  // Obtain the derivatives at x_i + dt/2 * dx_i
  if (!this->FunctionSet->FunctionValues(this->Vals, this->Derivs, userData))
  {
    memcpy(xnext, this->Vals, (numVals - 1) * sizeof(double));
    delTActual = delT / 2.0; // we've only taken half of a time step
    return OUT_OF_DOMAIN;
  }

  // Calculate x_i using improved values of derivatives
  for (i = 0; i < numDerivs; i++)
  {
    xnext[i] = xprev[i] + delT * this->Derivs[i];
  }

  delTActual = delT;

  return 0;
}
VTK_ABI_NAMESPACE_END
