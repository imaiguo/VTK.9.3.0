// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkSimpleReader
 * @brief   Superclass for algorithms that are not time or parallel aware
 *
 */

#ifndef vtkSimpleReader_h
#define vtkSimpleReader_h

#include "vtkCommonExecutionModelModule.h" // For export macro
#include "vtkReaderAlgorithm.h"

#include <string> // needed for std::string in the interface

VTK_ABI_NAMESPACE_BEGIN
struct vtkSimpleReaderInternal;

class VTKCOMMONEXECUTIONMODEL_EXPORT vtkSimpleReader : public vtkReaderAlgorithm
{
public:
  vtkTypeMacro(vtkSimpleReader, vtkReaderAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * Add a filename to be read. Since this superclass handles
   * file series to support time, multiple filenames can be added.
   * Note that the time values are either integers growing sequentially,
   * or are obtained from individual files as supported by the subclass.
   */
  void AddFileName(VTK_FILEPATH const char* fname);

  /**
   * Removes all filenames stored by the reader.
   */
  void ClearFileNames();

  /**
   * Returns the number of filenames stored by the reader.
   */
  int GetNumberOfFileNames() const;

  /**
   * Returns a particular filename stored by the reader.
   */
  VTK_FILEPATH const char* GetFileName(int i) const;

  /**
   * Returns the filename that was last loaded by the reader.
   * This is set internally in ReadMesh()
   */
  VTK_FILEPATH const char* GetCurrentFileName() const;

  ///@{
  /**
   * This is the superclass API overridden by this class
   * to provide time support internally. Subclasses should
   * not normally have to override these methods.
   */
  int ReadTimeDependentMetaData(int timestep, vtkInformation* metadata) override;
  int ReadMetaData(vtkInformation* metadata) override;
  int ReadMesh(int piece, int npieces, int nghosts, int timestep, vtkDataObject* output) override;
  int ReadPoints(int piece, int npieces, int nghosts, int timestep, vtkDataObject* output) override;
  int ReadArrays(int piece, int npieces, int nghosts, int timestep, vtkDataObject* output) override;
  ///@}

  /**
   * A subclass can override this method to provide an actual
   * time value for a given file (this method is called for
   * each filename stored by the reader). If time values is not
   * available, the subclass does not have to override. This
   * will return vtkMath::NaN() if no time value is present
   * in the file.
   */
  virtual double GetTimeValue(VTK_FILEPATH const std::string& fname);

  /**
   * A subclass can override this method to provide meta data
   * specific to a particular file. In order for this method
   * to be called, HasTemporalMetaData has to be set to true.
   */
  virtual int ReadMetaDataSimple(
    VTK_FILEPATH const std::string& /*fname*/, vtkInformation* /*metadata*/)
  {
    return 1;
  }

  /**
   * A method that needs to be override by the subclass to provide
   * the mesh (topology). Note that the filename is passed to this
   * method and should be used by the subclass. The subclass directly
   * adds the structure/topology to the provided data object.
   */
  virtual int ReadMeshSimple(VTK_FILEPATH const std::string& fname, vtkDataObject* output) = 0;

  /**
   * A method that needs to be override by the subclass to provide
   * the point coordinates. Note that the filename is passed to this
   * method and should be used by the subclass. The subclass directly
   * adds the coordinates to the provided data object.
   */
  virtual int ReadPointsSimple(VTK_FILEPATH const std::string& fname, vtkDataObject* output) = 0;

  /**
   * A method that needs to be override by the subclass to provide
   * data arrays. Note that the filename is passed to this
   * method and should be used by the subclass. The subclass directly
   * adds data arrays to the provided data object.
   */
  virtual int ReadArraysSimple(VTK_FILEPATH const std::string& fname, vtkDataObject* output) = 0;

protected:
  vtkSimpleReader();
  ~vtkSimpleReader() override;

  int CurrentFileIndex;
  bool HasTemporalMetaData;

private:
  vtkSimpleReader(const vtkSimpleReader&) = delete;
  void operator=(const vtkSimpleReader&) = delete;

  vtkSimpleReaderInternal* Internal;
};

VTK_ABI_NAMESPACE_END
#endif
