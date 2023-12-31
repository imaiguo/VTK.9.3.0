#!/usr/bin/env python
from vtkmodules.vtkIOImage import vtkImageReader2Factory
from vtkmodules.vtkInteractionImage import vtkImageViewer
import vtkmodules.vtkInteractionStyle
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()

# Image pipeline
createReader = vtkImageReader2Factory()
reader = createReader.CreateImageReader2FromExtension("tif")
reader.SetFileName(VTK_DATA_ROOT + "/Data/beach.tif")
# "beach.tif" image contains ORIENTATION tag which is
# ORIENTATION_TOPLEFT (row 0 top, col 0 lhs) type. The TIFF
# reader parses this tag and sets the internal TIFF image
# orientation accordingly.  To overwrite this orientation with a vtk
# convention of ORIENTATION_BOTLEFT (row 0 bottom, col 0 lhs ), invoke
# SetOrientationType method with parameter value of 4.
reader.SetOrientationType(4)
viewer = vtkImageViewer()
viewer.SetInputConnection(reader.GetOutputPort())
viewer.SetColorWindow(256)
viewer.SetColorLevel(127.5)
#make interface
viewer.Render()
reader.UnRegister(viewer) # not needed in python
# --- end of script --
