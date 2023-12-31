// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkPHardwareSelector
 * @brief   vtkHardwareSelector useful for parallel
 * rendering.
 *
 * vtkPHardwareSelector is a vtkHardwareSelector that is parallel aware. It
 * relies on the fact that the application is going to use some other mechanism
 * to ensure that renders are synchronized among windows on all processes. The
 * synchronization happens from the root node. When the root node renders, all
 * processes render. Only vtkPHardwareSelector instance on the root node
 * triggers the renders. All other processes, simply listen to the StartEvent
 * fired and beginning of the render to ensure that vtkHardwareSelector's
 * CurrentPass is updated appropriately.
 */

#ifndef vtkPHardwareSelector_h
#define vtkPHardwareSelector_h

#include "vtkOpenGLHardwareSelector.h"
#include "vtkRenderingParallelModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class VTKRENDERINGPARALLEL_EXPORT vtkPHardwareSelector : public vtkOpenGLHardwareSelector
{
public:
  static vtkPHardwareSelector* New();
  vtkTypeMacro(vtkPHardwareSelector, vtkOpenGLHardwareSelector);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Set/Get the is the root process. The root processes
   * is the only processes which has the composited result and hence the only
   * processes that capture buffers and builds selected list ids.
   */
  vtkSetMacro(ProcessIsRoot, bool);
  vtkGetMacro(ProcessIsRoot, bool);
  vtkBooleanMacro(ProcessIsRoot, bool);
  ///@}

  /**
   * Overridden to only allow the superclass implementation on the root node. On
   * all other processes, the updating the internal state of the
   * vtkHardwareSelector as the capturing of buffers progresses is done as a
   * slave to the master render.
   */
  bool CaptureBuffers() override;

protected:
  vtkPHardwareSelector();
  ~vtkPHardwareSelector() override;

  void StartRender();
  void EndRender();

  bool ProcessIsRoot;

private:
  vtkPHardwareSelector(const vtkPHardwareSelector&) = delete;
  void operator=(const vtkPHardwareSelector&) = delete;

  class vtkObserver;
  friend class vtkObserver;
  vtkObserver* Observer;
};

VTK_ABI_NAMESPACE_END
#endif
