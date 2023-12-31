// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class vtkExtractSelectedBlock
 * @brief Extract-Selection filter to extract blocks.
 *
 * vtkExtractSelectedBlock extracts blocks from a composite dataset on input 0
 * using a vtkSelection on input 1.
 *
 * IDs extracted can refer to leaf nodes or non-leaf nodes. When they refer to
 * non-leaf nodes, the entire subtree is extracted.
 *
 * Note: this filter uses `vtkCompositeDataSet::ShallowCopy`, as a result, datasets at
 * leaf nodes are simply passed through, rather than being shallow-copied
 * themselves.
 *
 * @deprecated vtkExtractSelectedBlock is deprecated in VTK 9.2 and will be removed.
 * Use `vtkExtractSelection` instead of `vtkExtractSelectedBlock`.
 *
 * Example using vtkExtractSelectedBlock:
 *
 * vtkNew<vtkExtractSelectedBlock> selFilter;
 * selFilter->SetInputConnection(0, sphereSource->GetOutputPort());
 * selFilter->SetInputConnection(1, selectionSource->GetOutputPort());
 *
 * Example using vtkExtractSelection:
 *
 * vtkNew<vtkExtractSelection> selFilter;
 * selFilter->SetInputConnection(0, sphereSource->GetOutputPort());
 * selFilter->SetInputConnection(1, selectionSource->GetOutputPort());
 */

#ifndef vtkExtractSelectedBlock_h
#define vtkExtractSelectedBlock_h

#include "vtkDeprecation.h" // For VTK_DEPRECATED_IN_9_2_0
#include "vtkExtractSelectionBase.h"
#include "vtkFiltersExtractionModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class VTK_DEPRECATED_IN_9_2_0("Use vtkExtractSelection instead of vtkExtractSelectedBlock.")
  VTKFILTERSEXTRACTION_EXPORT vtkExtractSelectedBlock : public vtkExtractSelectionBase
{
public:
  static vtkExtractSelectedBlock* New();
  vtkTypeMacro(vtkExtractSelectedBlock, vtkExtractSelectionBase);
  void PrintSelf(ostream& os, vtkIndent indent) override;

protected:
  vtkExtractSelectedBlock();
  ~vtkExtractSelectedBlock() override;

  // Generate the output.
  int RequestData(vtkInformation*, vtkInformationVector**, vtkInformationVector*) override;

  /**
   * Sets up empty output dataset
   */
  int RequestDataObject(vtkInformation* request, vtkInformationVector** inputVector,
    vtkInformationVector* outputVector) override;

  int FillInputPortInformation(int port, vtkInformation* info) override;

private:
  vtkExtractSelectedBlock(const vtkExtractSelectedBlock&) = delete;
  void operator=(const vtkExtractSelectedBlock&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif

// VTK-HeaderTest-Exclude: vtkExtractSelectedBlock.h
