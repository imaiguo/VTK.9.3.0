// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

#include "vtkExecutionTimer.h"

#include "vtkAlgorithm.h"
#include "vtkCallbackCommand.h"
#include "vtkObjectFactory.h"
#include "vtkTimerLog.h"
#include <iostream>

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkExecutionTimer);

//------------------------------------------------------------------------------

vtkExecutionTimer::vtkExecutionTimer()
{
  this->Filter = nullptr;
  this->Callback = vtkCallbackCommand::New();
  this->Callback->SetClientData(this);
  this->Callback->SetCallback(vtkExecutionTimer::EventRelay);

  this->CPUStartTime = 0;
  this->CPUEndTime = 0;
  this->ElapsedCPUTime = 0;
  this->WallClockStartTime = 0;
  this->WallClockEndTime = 0;
  this->ElapsedWallClockTime = 0;
}

//------------------------------------------------------------------------------

vtkExecutionTimer::~vtkExecutionTimer()
{
  this->SetFilter(nullptr);
  this->Callback->Delete();
}

//------------------------------------------------------------------------------

void vtkExecutionTimer::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Observed Filter: ";
  if (this->Filter)
  {
    os << "\n";
    this->Filter->PrintSelf(os, indent.GetNextIndent());
  }
  else
  {
    os << "(null)\n";
  }
  os << indent << "Most recent CPU start time: " << this->CPUStartTime << "\n";
  os << indent << "Most recent CPU end time: " << this->CPUStartTime << "\n";
  os << indent << "Most recent CPU elapsed time: " << this->ElapsedCPUTime << "\n";

  os << indent << "Most recent wall clock start time: " << this->WallClockStartTime << "\n";
  os << indent << "Most recent wall clock end time: " << this->WallClockStartTime << "\n";
  os << indent << "Most recent wall clock elapsed time: " << this->WallClockEndTime << "\n";
}

//------------------------------------------------------------------------------

void vtkExecutionTimer::SetFilter(vtkAlgorithm* filter)
{
  if (this->Filter)
  {
    this->Filter->RemoveObserver(this->Callback);
    this->Filter->RemoveObserver(this->Callback);
    this->Filter->UnRegister(this);
    this->Filter = nullptr;
  }

  if (filter)
  {
    this->Filter = filter;
    this->Filter->Register(this);
    this->Filter->AddObserver(vtkCommand::StartEvent, this->Callback);
    this->Filter->AddObserver(vtkCommand::EndEvent, this->Callback);
  }
}

//------------------------------------------------------------------------------

void vtkExecutionTimer::EventRelay(vtkObject* vtkNotUsed(caller), unsigned long eventType,
  void* clientData, void* vtkNotUsed(callData))
{
  vtkExecutionTimer* receiver = static_cast<vtkExecutionTimer*>(clientData);

  if (eventType == vtkCommand::StartEvent)
  {
    receiver->StartTimer();
  }
  else if (eventType == vtkCommand::EndEvent)
  {
    receiver->StopTimer();
  }
  else
  {
    vtkGenericWarningMacro("WARNING: Unknown event type "
      << eventType << " in vtkExecutionTimer::EventRelay.  This shouldn't happen.");
  }
}

//------------------------------------------------------------------------------

void vtkExecutionTimer::StartTimer()
{
  this->CPUEndTime = 0;
  this->ElapsedCPUTime = 0;
  this->WallClockEndTime = 0;
  this->ElapsedWallClockTime = 0;

  this->WallClockStartTime = vtkTimerLog::GetUniversalTime();
  this->CPUStartTime = vtkTimerLog::GetCPUTime();
}

//------------------------------------------------------------------------------

void vtkExecutionTimer::StopTimer()
{
  this->WallClockEndTime = vtkTimerLog::GetUniversalTime();
  this->CPUEndTime = vtkTimerLog::GetCPUTime();

  this->ElapsedCPUTime = this->CPUEndTime - this->CPUStartTime;
  this->ElapsedWallClockTime = this->WallClockEndTime - this->WallClockStartTime;

  this->TimerFinished();
}

//------------------------------------------------------------------------------

void vtkExecutionTimer::TimerFinished()
{
  // Nothing to do here
}
VTK_ABI_NAMESPACE_END
