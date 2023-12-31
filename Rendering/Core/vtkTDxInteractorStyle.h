// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkTDxInteractorStyle
 * @brief   provide 3DConnexion device event-driven interface to the rendering window
 *
 *
 * vtkTDxInteractorStyle is an abstract class defining an event-driven
 * interface to support 3DConnexion device events send by
 * vtkRenderWindowInteractor.
 * vtkRenderWindowInteractor forwards events in a platform independent form to
 * vtkInteractorStyle which can then delegate some processing to
 * vtkTDxInteractorStyle.
 *
 * @sa
 * vtkInteractorStyle vtkRenderWindowInteractor
 * vtkTDxInteractorStyleCamera
 */

#ifndef vtkTDxInteractorStyle_h
#define vtkTDxInteractorStyle_h

#include "vtkObject.h"
#include "vtkRenderingCoreModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class vtkTDxMotionEventInfo;
class vtkRenderer;
class vtkTDxInteractorStyleSettings;

class VTKRENDERINGCORE_EXPORT vtkTDxInteractorStyle : public vtkObject
{
public:
  vtkTypeMacro(vtkTDxInteractorStyle, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Action on motion event. Default implementation is empty.
   * \pre: motionInfo_exist: motionInfo!=0
   */
  virtual void OnMotionEvent(vtkTDxMotionEventInfo* motionInfo);

  /**
   * Action on button pressed event. Default implementation is empty.
   */
  virtual void OnButtonPressedEvent(int button);

  /**
   * Action on button released event. Default implementation is empty.
   */
  virtual void OnButtonReleasedEvent(int button);

  /**
   * Dispatch the events TDxMotionEvent, TDxButtonPressEvent and
   * TDxButtonReleaseEvent to OnMotionEvent(), OnButtonPressedEvent() and
   * OnButtonReleasedEvent() respectively.
   * It is called by the vtkInteractorStyle.
   * This method is virtual for convenient but you should really override
   * the On*Event() methods only.
   * \pre renderer can be null.
   */
  virtual void ProcessEvent(vtkRenderer* renderer, unsigned long event, void* calldata);

  ///@{
  /**
   * 3Dconnexion device settings. (sensitivity, individual axis filters).
   * Initial object is not null.
   */
  vtkGetObjectMacro(Settings, vtkTDxInteractorStyleSettings);
  virtual void SetSettings(vtkTDxInteractorStyleSettings* settings);
  ///@}

protected:
  vtkTDxInteractorStyle();
  ~vtkTDxInteractorStyle() override;

  vtkTDxInteractorStyleSettings* Settings;

  vtkRenderer* Renderer;

private:
  vtkTDxInteractorStyle(const vtkTDxInteractorStyle&) = delete;
  void operator=(const vtkTDxInteractorStyle&) = delete;
};
VTK_ABI_NAMESPACE_END
#endif
