// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkTIFFWriter
 * @brief   write out image data as a TIFF file
 *
 * vtkTIFFWriter writes image data as a TIFF data file. Data can be written
 * uncompressed or compressed. Several forms of compression are supported
 * including packed bits, JPEG, deflation, and LZW. (Note: LZW compression
 * is currently under patent in the US and is disabled until the patent
 * expires. However, the mechanism for supporting this compression is available
 * for those with a valid license or to whom the patent does not apply.)
 */

#ifndef vtkTIFFWriter_h
#define vtkTIFFWriter_h

#include "vtkIOImageModule.h" // For export macro
#include "vtkImageWriter.h"

VTK_ABI_NAMESPACE_BEGIN
class VTKIOIMAGE_EXPORT vtkTIFFWriter : public vtkImageWriter
{
public:
  static vtkTIFFWriter* New();
  vtkTypeMacro(vtkTIFFWriter, vtkImageWriter);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  /**
   * The main interface which triggers the writer to start.
   */
  void Write() override;

  enum
  { // Compression types
    NoCompression,
    PackBits,
    JPEG,
    Deflate,
    LZW
  };

  ///@{
  /**
   * Set compression type. Sinze LZW compression is patented outside US, the
   * additional work steps have to be taken in order to use that compression.
   */
  vtkSetClampMacro(Compression, int, NoCompression, LZW);
  vtkGetMacro(Compression, int);
  void SetCompressionToNoCompression() { this->SetCompression(NoCompression); }
  void SetCompressionToPackBits() { this->SetCompression(PackBits); }
  void SetCompressionToJPEG() { this->SetCompression(JPEG); }
  void SetCompressionToDeflate() { this->SetCompression(Deflate); }
  void SetCompressionToLZW() { this->SetCompression(LZW); }
  ///@}

protected:
  vtkTIFFWriter();
  ~vtkTIFFWriter() override = default;

  void WriteFile(ostream* file, vtkImageData* data, int ext[6], int wExt[6]) override;
  void WriteFileHeader(ostream*, vtkImageData*, int wExt[6]) override;
  void WriteFileTrailer(ostream*, vtkImageData*) override;

  void* TIFFPtr;
  int Compression;
  int Width;
  int Height;
  int Pages;
  double XResolution;
  double YResolution;

private:
  vtkTIFFWriter(const vtkTIFFWriter&) = delete;
  void operator=(const vtkTIFFWriter&) = delete;

  template <typename T>
  void WriteVolume(T* buffer);
};

VTK_ABI_NAMESPACE_END
#endif
