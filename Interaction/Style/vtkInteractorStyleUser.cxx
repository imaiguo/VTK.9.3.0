// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkInteractorStyleUser.h"
#include "vtkCellPicker.h"
#include "vtkCommand.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindowInteractor.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkInteractorStyleUser);

//------------------------------------------------------------------------------
vtkInteractorStyleUser::vtkInteractorStyleUser()
{
  // Tell the parent class not to handle observers
  // that has to be done here
  this->HandleObserversOff();
  this->LastPos[0] = this->LastPos[1] = 0;
  this->OldPos[0] = this->OldPos[1] = 0;
  this->ShiftKey = 0;
  this->CtrlKey = 0;
  this->Char = '\0';
  this->KeySym = nullptr;
  this->Button = 0;
}

//------------------------------------------------------------------------------
vtkInteractorStyleUser::~vtkInteractorStyleUser() = default;

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "LastPos: (" << this->LastPos[0] << ", " << this->LastPos[1] << ")\n";
  os << indent << "OldPos: (" << this->OldPos[0] << ", " << this->OldPos[1] << ")\n";
  os << indent << "ShiftKey: " << this->ShiftKey << "\n";
  os << indent << "CtrlKey: " << this->CtrlKey << "\n";
  os << indent << "Char: " << this->Char << "\n";
  os << indent << "KeySym: " << (this->KeySym ? this->KeySym : "(null)") << "\n";
  os << indent << "Button: " << this->Button << "\n";
}

//------------------------------------------------------------------------------
// checks for USERINTERACTION state, then defers to the superclass modes
void vtkInteractorStyleUser::OnTimer()
{
  if (this->HasObserver(vtkCommand::TimerEvent))
  {
    this->InvokeEvent(vtkCommand::TimerEvent, &(this->TimerId));
  }

  if (this->State == VTKIS_USERINTERACTION)
  {
    if (this->HasObserver(vtkCommand::UserEvent))
    {
      this->InvokeEvent(vtkCommand::UserEvent, nullptr);
      this->OldPos[0] = this->LastPos[0];
      this->OldPos[1] = this->LastPos[1];
      if (this->UseTimers)
      {
        this->Interactor->ResetTimer(this->TimerId);
      }
    }
  }
  else if (!(this->HasObserver(vtkCommand::MouseMoveEvent) &&
             (this->Button == 0 ||
               (this->HasObserver(vtkCommand::LeftButtonPressEvent) && this->Button == 1) ||
               (this->HasObserver(vtkCommand::MiddleButtonPressEvent) && this->Button == 2) ||
               (this->HasObserver(vtkCommand::RightButtonPressEvent) && this->Button == 3))))
  {
    this->vtkInteractorStyle::OnTimer();
  }
  else if (this->HasObserver(vtkCommand::TimerEvent))
  {
    if (this->UseTimers)
    {
      this->Interactor->ResetTimer(this->TimerId);
    }
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnKeyPress()
{
  if (this->HasObserver(vtkCommand::KeyPressEvent))
  {
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->CtrlKey = this->Interactor->GetControlKey();
    this->KeySym = this->Interactor->GetKeySym();
    this->Char = this->Interactor->GetKeyCode();
    this->InvokeEvent(vtkCommand::KeyPressEvent, nullptr);
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnKeyRelease()
{
  if (this->HasObserver(vtkCommand::KeyReleaseEvent))
  {
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->CtrlKey = this->Interactor->GetControlKey();
    this->KeySym = this->Interactor->GetKeySym();
    this->Char = this->Interactor->GetKeyCode();

    this->InvokeEvent(vtkCommand::KeyReleaseEvent, nullptr);
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnChar()
{
  // otherwise pass the OnChar to the vtkInteractorStyle.
  if (this->HasObserver(vtkCommand::CharEvent))
  {
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->CtrlKey = this->Interactor->GetControlKey();
    this->Char = this->Interactor->GetKeyCode();

    this->InvokeEvent(vtkCommand::CharEvent, nullptr);
  }
  else
  {
    this->vtkInteractorStyle::OnChar();
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnRightButtonDown()
{
  this->Button = 3;

  if (this->HasObserver(vtkCommand::RightButtonPressEvent))
  {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::RightButtonPressEvent, nullptr);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
  }
  else
  {
    this->vtkInteractorStyle::OnRightButtonDown();
  }
}
//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnRightButtonUp()
{
  if (this->HasObserver(vtkCommand::RightButtonReleaseEvent))
  {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::RightButtonReleaseEvent, nullptr);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
  }
  else
  {
    this->vtkInteractorStyle::OnRightButtonUp();
  }

  if (this->Button == 3)
  {
    this->Button = 0;
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnMouseWheelForward()
{
  if (this->HasObserver(vtkCommand::MouseWheelForwardEvent))
  {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::MouseWheelForwardEvent, nullptr);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
  }
  else
  {
    this->vtkInteractorStyle::OnMouseWheelForward();
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnMouseWheelBackward()
{
  if (this->HasObserver(vtkCommand::MouseWheelBackwardEvent))
  {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::MouseWheelBackwardEvent, nullptr);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
  }
  else
  {
    this->vtkInteractorStyle::OnMouseWheelBackward();
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnMiddleButtonDown()
{
  this->Button = 2;

  if (this->HasObserver(vtkCommand::MiddleButtonPressEvent))
  {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::MiddleButtonPressEvent, nullptr);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
  }
  else
  {
    this->vtkInteractorStyle::OnMiddleButtonDown();
  }
}
//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnMiddleButtonUp()
{
  if (this->HasObserver(vtkCommand::MiddleButtonReleaseEvent))
  {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, nullptr);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
  }
  else
  {
    this->vtkInteractorStyle::OnMiddleButtonUp();
  }

  if (this->Button == 2)
  {
    this->Button = 0;
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnLeftButtonDown()
{
  this->Button = 1;

  if (this->HasObserver(vtkCommand::LeftButtonPressEvent))
  {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::LeftButtonPressEvent, nullptr);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
  }
  else
  {
    this->vtkInteractorStyle::OnLeftButtonDown();
  }
}
//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnLeftButtonUp()
{
  if (this->HasObserver(vtkCommand::LeftButtonReleaseEvent))
  {
    int x = this->Interactor->GetEventPosition()[0];
    int y = this->Interactor->GetEventPosition()[1];
    this->CtrlKey = this->Interactor->GetControlKey();
    this->ShiftKey = this->Interactor->GetShiftKey();
    this->LastPos[0] = x;
    this->LastPos[1] = y;
    this->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, nullptr);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
  }
  else
  {
    this->vtkInteractorStyle::OnLeftButtonUp();
  }

  if (this->Button == 1)
  {
    this->Button = 0;
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnMouseMove()
{
  this->vtkInteractorStyle::OnMouseMove();

  int x = this->Interactor->GetEventPosition()[0];
  int y = this->Interactor->GetEventPosition()[1];
  this->LastPos[0] = x;
  this->LastPos[1] = y;
  this->ShiftKey = this->Interactor->GetShiftKey();
  this->CtrlKey = this->Interactor->GetControlKey();

  if (this->HasObserver(vtkCommand::MouseMoveEvent))
  {
    this->InvokeEvent(vtkCommand::MouseMoveEvent, nullptr);
    this->OldPos[0] = x;
    this->OldPos[1] = y;
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnExpose()
{
  if (this->HasObserver(vtkCommand::ExposeEvent))
  {
    this->InvokeEvent(vtkCommand::ExposeEvent, nullptr);
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnConfigure()
{
  if (this->HasObserver(vtkCommand::ConfigureEvent))
  {
    this->InvokeEvent(vtkCommand::ConfigureEvent, nullptr);
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnEnter()
{
  if (this->HasObserver(vtkCommand::EnterEvent))
  {
    this->LastPos[0] = this->Interactor->GetEventPosition()[0];
    this->LastPos[1] = this->Interactor->GetEventPosition()[1];
    this->InvokeEvent(vtkCommand::EnterEvent, nullptr);
  }
}

//------------------------------------------------------------------------------
void vtkInteractorStyleUser::OnLeave()
{
  if (this->HasObserver(vtkCommand::LeaveEvent))
  {
    this->LastPos[0] = this->Interactor->GetEventPosition()[0];
    this->LastPos[1] = this->Interactor->GetEventPosition()[1];
    this->InvokeEvent(vtkCommand::LeaveEvent, nullptr);
  }
}
VTK_ABI_NAMESPACE_END
