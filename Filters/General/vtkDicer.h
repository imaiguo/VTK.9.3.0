// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkDicer
 * @brief   abstract superclass to divide dataset into pieces
 *
 * Subclasses of vtkDicer divides the input dataset into separate
 * pieces.  These pieces can then be operated on by other filters
 * (e.g., vtkThreshold). One application is to break very large
 * polygonal models into pieces and performing viewing and occlusion
 * culling on the pieces. Multiple pieces can also be streamed through
 * the visualization pipeline.
 *
 * To use this filter, you must specify the execution mode of the
 * filter; i.e., set the way that the piece size is controlled (do
 * this by setting the DiceMode ivar). The filter does not change the
 * geometry or topology of the input dataset, rather it generates
 * integer numbers that indicate which piece a particular point
 * belongs to (i.e., it modifies the point and cell attribute
 * data). The integer number can be placed into the output scalar
 * data, or the output field data.
 *
 * @warning
 * The number of pieces generated may not equal the specified number
 * of pieces. Use the method GetNumberOfActualPieces() after filter
 * execution to get the actual number of pieces generated.
 *
 * @sa
 * vtkOBBDicer vtkConnectedDicer vtkSpatialDicer
 */

#ifndef vtkDicer_h
#define vtkDicer_h

#include "vtkDataSetAlgorithm.h"
#include "vtkFiltersGeneralModule.h" // For export macro

#define VTK_DICE_MODE_NUMBER_OF_POINTS 0
#define VTK_DICE_MODE_SPECIFIED_NUMBER 1
#define VTK_DICE_MODE_MEMORY_LIMIT 2

VTK_ABI_NAMESPACE_BEGIN
class VTKFILTERSGENERAL_EXPORT vtkDicer : public vtkDataSetAlgorithm
{
public:
  vtkTypeMacro(vtkDicer, vtkDataSetAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Set/Get the flag which controls whether to generate point scalar
   * data or point field data. If this flag is off, scalar data is
   * generated.  Otherwise, field data is generated. Note that the
   * generated the data are integer numbers indicating which piece a
   * particular point belongs to.
   */
  vtkSetMacro(FieldData, vtkTypeBool);
  vtkGetMacro(FieldData, vtkTypeBool);
  vtkBooleanMacro(FieldData, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Specify the method to determine how many pieces the data should be
   * broken into. By default, the number of points per piece is used.
   */
  vtkSetClampMacro(DiceMode, int, VTK_DICE_MODE_NUMBER_OF_POINTS, VTK_DICE_MODE_MEMORY_LIMIT);
  vtkGetMacro(DiceMode, int);
  void SetDiceModeToNumberOfPointsPerPiece() { this->SetDiceMode(VTK_DICE_MODE_NUMBER_OF_POINTS); }
  void SetDiceModeToSpecifiedNumberOfPieces() { this->SetDiceMode(VTK_DICE_MODE_SPECIFIED_NUMBER); }
  void SetDiceModeToMemoryLimitPerPiece() { this->SetDiceMode(VTK_DICE_MODE_MEMORY_LIMIT); }
  ///@}

  ///@{
  /**
   * Use the following method after the filter has updated to
   * determine the actual number of pieces the data was separated
   * into.
   */
  vtkGetMacro(NumberOfActualPieces, int);
  ///@}

  ///@{
  /**
   * Control piece size based on the maximum number of points per piece.
   * (This ivar has effect only when the DiceMode is set to
   * SetDiceModeToNumberOfPoints().)
   */
  vtkSetClampMacro(NumberOfPointsPerPiece, int, 1000, VTK_INT_MAX);
  vtkGetMacro(NumberOfPointsPerPiece, int);
  ///@}

  ///@{
  /**
   * Set/Get the number of pieces the object is to be separated into.
   * (This ivar has effect only when the DiceMode is set to
   * SetDiceModeToSpecifiedNumber()). Note that the ivar
   * NumberOfPieces is a target - depending on the particulars of the
   * data, more or less number of pieces than the target value may be
   * created.
   */
  vtkSetClampMacro(NumberOfPieces, int, 1, VTK_INT_MAX);
  vtkGetMacro(NumberOfPieces, int);
  ///@}

  ///@{
  /**
   * Control piece size based on a memory limit.  (This ivar has
   * effect only when the DiceMode is set to
   * SetDiceModeToMemoryLimit()). The memory limit should be set in
   * kibibytes (1024 bytes).
   */
  vtkSetClampMacro(MemoryLimit, unsigned long, 100, VTK_INT_MAX);
  vtkGetMacro(MemoryLimit, unsigned long);
  ///@}

protected:
  vtkDicer();
  ~vtkDicer() override = default;

  virtual void UpdatePieceMeasures(vtkDataSet* input);

  int NumberOfPointsPerPiece;
  int NumberOfPieces;
  unsigned long MemoryLimit;
  int NumberOfActualPieces;
  vtkTypeBool FieldData;
  int DiceMode;

private:
  vtkDicer(const vtkDicer&) = delete;
  void operator=(const vtkDicer&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif
