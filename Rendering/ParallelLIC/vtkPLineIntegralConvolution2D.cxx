// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkPLineIntegralConvolution2D.h"

#include "vtkMPI.h"
#include "vtkObjectFactory.h"
#include "vtkPPainterCommunicator.h"
#include "vtkPainterCommunicator.h"
#include "vtkParallelTimer.h"

//------------------------------------------------------------------------------
VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkPLineIntegralConvolution2D);

//------------------------------------------------------------------------------
vtkPLineIntegralConvolution2D::vtkPLineIntegralConvolution2D()
{
  this->Comm = new vtkPPainterCommunicator;
}

//------------------------------------------------------------------------------
vtkPLineIntegralConvolution2D::~vtkPLineIntegralConvolution2D() = default;

//------------------------------------------------------------------------------
void vtkPLineIntegralConvolution2D::SetCommunicator(vtkPainterCommunicator* comm)
{
  this->Comm->Copy(comm, false);
}

//------------------------------------------------------------------------------
vtkPainterCommunicator* vtkPLineIntegralConvolution2D::GetCommunicator()
{
  return this->Comm;
}

//------------------------------------------------------------------------------
void vtkPLineIntegralConvolution2D::GetGlobalMinMax(
  vtkPainterCommunicator* painterComm, float& min, float& max)
{
  vtkPPainterCommunicator* pPainterComm = dynamic_cast<vtkPPainterCommunicator*>(painterComm);

  if (pPainterComm->GetMPIInitialized())
  {
    MPI_Comm comm = *((MPI_Comm*)pPainterComm->GetCommunicator());

    MPI_Allreduce(MPI_IN_PLACE, &min, 1, MPI_FLOAT, MPI_MIN, comm);

    MPI_Allreduce(MPI_IN_PLACE, &max, 1, MPI_FLOAT, MPI_MAX, comm);
  }
}

//------------------------------------------------------------------------------
void vtkPLineIntegralConvolution2D::StartTimerEvent(const char* event)
{
#if defined(vtkLineIntegralConvolution2DTIME) || defined(vtkSurfaceLICPainterTIME)
  vtkParallelTimer* log = vtkParallelTimer::GetGlobalInstance();
  log->StartEvent(event);
#else
  (void)event;
#endif
}

//------------------------------------------------------------------------------
void vtkPLineIntegralConvolution2D::EndTimerEvent(const char* event)
{
#if defined(vtkLineIntegralConvolution2DTIME) || defined(vtkSurfaceLICPainterTIME)
  vtkParallelTimer* log = vtkParallelTimer::GetGlobalInstance();
  log->EndEvent(event);
#else
  (void)event;
#endif
}

//------------------------------------------------------------------------------
void vtkPLineIntegralConvolution2D::WriteTimerLog(const char* fileName)
{
#ifdef vtkLineIntegralConvolution2DTIME
  std::string fname = fileName ? fileName : "";
  if (fname == this->LogFileName)
  {
    return;
  }
  this->LogFileName = fname;
  if (!fname.empty())
  {
    vtkParallelTimer* log = vtkParallelTimer::GetGlobalInstance();
    log->SetFileName(fname.c_str());
    log->Update();
    log->Write();
  }
#else
  (void)fileName;
#endif
}

//------------------------------------------------------------------------------
void vtkPLineIntegralConvolution2D::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "LogFileName=" << this->LogFileName << endl;
}
VTK_ABI_NAMESPACE_END
