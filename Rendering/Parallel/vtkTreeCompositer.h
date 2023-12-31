// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause

// This software and ancillary information known as vtk_ext (and
// herein called "SOFTWARE") is made available under the terms
// described below.  The SOFTWARE has been approved for release with
// associated LA_CC Number 99-44, granted by Los Alamos National
// Laboratory in July 1999.
//
// Unless otherwise indicated, this SOFTWARE has been authored by an
// employee or employees of the University of California, operator of
// the Los Alamos National Laboratory under Contract No. W-7405-ENG-36
// with the United States Department of Energy.
//
// The United States Government has rights to use, reproduce, and
// distribute this SOFTWARE.  The public may copy, distribute, prepare
// derivative works and publicly display this SOFTWARE without charge,
// provided that this Notice and any statement of authorship are
// reproduced on all copies.
//
// Neither the U. S. Government, the University of California, nor the
// Advanced Computing Laboratory makes any warranty, either express or
// implied, nor assumes any liability or responsibility for the use of
// this SOFTWARE.
//
// If SOFTWARE is modified to produce derivative works, such modified
// SOFTWARE should be clearly marked, so as not to confuse it with the
// version available from Los Alamos National Laboratory.

/**
 * @class   vtkTreeCompositer
 * @brief   Implements tree based compositing.
 *
 *
 * vtkTreeCompositer operates in multiple processes.  Each compositer has
 * a render window.  They use a vtkMultiProcessController to communicate
 * the color and depth buffer to process 0's render window.
 * It will not handle transparency well.
 *
 * @sa
 * vtkCompositeManager
 */

#ifndef vtkTreeCompositer_h
#define vtkTreeCompositer_h

#include "vtkCompositer.h"
#include "vtkRenderingParallelModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class VTKRENDERINGPARALLEL_EXPORT vtkTreeCompositer : public vtkCompositer
{
public:
  static vtkTreeCompositer* New();
  vtkTypeMacro(vtkTreeCompositer, vtkCompositer);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  void CompositeBuffer(
    vtkDataArray* pBuf, vtkFloatArray* zBuf, vtkDataArray* pTmp, vtkFloatArray* zTmp) override;

protected:
  vtkTreeCompositer();
  ~vtkTreeCompositer() override;

private:
  vtkTreeCompositer(const vtkTreeCompositer&) = delete;
  void operator=(const vtkTreeCompositer&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif
