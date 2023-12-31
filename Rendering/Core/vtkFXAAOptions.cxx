// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkFXAAOptions.h"

#include "vtkObjectFactory.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkFXAAOptions);

//------------------------------------------------------------------------------
void vtkFXAAOptions::PrintSelf(std::ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "RelativeContrastThreshold: " << this->RelativeContrastThreshold << "\n";
  os << indent << "HardContrastThreshold: " << this->HardContrastThreshold << "\n";
  os << indent << "SubpixelBlendLimit: " << this->SubpixelBlendLimit << "\n";
  os << indent << "SubpixelContrastThreshold: " << this->SubpixelContrastThreshold << "\n";
  os << indent << "EndpointSearchIterations: " << this->EndpointSearchIterations << "\n";
  os << indent << "UseHighQualityEndpoints: " << this->UseHighQualityEndpoints << "\n";

  os << indent << "DebugOptionValue: ";
  switch (this->DebugOptionValue)
  {
    default:
    case vtkFXAAOptions::FXAA_NO_DEBUG:
      os << "FXAA_NO_DEBUG\n";
      break;
    case vtkFXAAOptions::FXAA_DEBUG_SUBPIXEL_ALIASING:
      os << "FXAA_DEBUG_SUBPIXEL_ALIASING\n";
      break;
    case vtkFXAAOptions::FXAA_DEBUG_EDGE_DIRECTION:
      os << "FXAA_DEBUG_EDGE_DIRECTION\n";
      break;
    case vtkFXAAOptions::FXAA_DEBUG_EDGE_NUM_STEPS:
      os << "FXAA_DEBUG_EDGE_NUM_STEPS\n";
      break;
    case vtkFXAAOptions::FXAA_DEBUG_EDGE_DISTANCE:
      os << "FXAA_DEBUG_EDGE_DISTANCE\n";
      break;
    case vtkFXAAOptions::FXAA_DEBUG_EDGE_SAMPLE_OFFSET:
      os << "FXAA_DEBUG_EDGE_SAMPLE_OFFSET\n";
      break;
    case vtkFXAAOptions::FXAA_DEBUG_ONLY_SUBPIX_AA:
      os << "FXAA_DEBUG_ONLY_SUBPIX_AA\n";
      break;
    case vtkFXAAOptions::FXAA_DEBUG_ONLY_EDGE_AA:
      os << "FXAA_DEBUG_ONLY_EDGE_AA\n";
      break;
  }
}

//------------------------------------------------------------------------------
vtkFXAAOptions::vtkFXAAOptions()
  : RelativeContrastThreshold(1.f / 8.f)
  , HardContrastThreshold(1.f / 16.f)
  , SubpixelBlendLimit(3.f / 4.f)
  , SubpixelContrastThreshold(1.f / 4.f)
  , EndpointSearchIterations(12)
  , UseHighQualityEndpoints(true)
  , DebugOptionValue(vtkFXAAOptions::FXAA_NO_DEBUG)
{
}

//------------------------------------------------------------------------------
vtkFXAAOptions::~vtkFXAAOptions() = default;
VTK_ABI_NAMESPACE_END
