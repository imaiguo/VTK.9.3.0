#!/usr/bin/env python
from vtkmodules.vtkFiltersGeneral import vtkBrownianPoints
from vtkmodules.vtkFiltersSources import vtkSphereSource
from vtkmodules.vtkRenderingCore import (
    vtkActor,
    vtkPolyDataMapper,
    vtkRenderWindow,
    vtkRenderWindowInteractor,
    vtkRenderer,
)
import vtkmodules.vtkInteractionStyle
import vtkmodules.vtkRenderingFreeType
import vtkmodules.vtkRenderingOpenGL2
from vtkmodules.util.misc import vtkGetDataRoot
VTK_DATA_ROOT = vtkGetDataRoot()


# tests the support to pass generic vertex attributes to be used in Cg shaders.
xmlMaterial = '''<?xml version="1.0" encoding="UTF-8"?>
<Material name="GenericAttributes1">
  <Shader
    scope="Vertex"
    name="VertexShader"
    location="Inline"
    language="GLSL"
    entry="main">
    in vec3 genAttrVector;
    out vec4 color;

    void main(void)
    {
      gl_Position = gl_ModelViewProjectionMatrix *gl_Vertex;
      color = vec4(normalize(genAttrVector), 1.0);
    }
  </Shader>

  <Shader scope="Fragment" name="FragmentShader" location="Inline"
    language="GLSL" entry="main">
    in vec4 color;
    void main(void)
    {
      gl_FragColor = color;
    }
  </Shader>
</Material>'''

renWin = vtkRenderWindow()
iren = vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
renderer = vtkRenderer()
renWin.AddRenderer(renderer)

src1 = vtkSphereSource()
src1.SetRadius(5)
src1.SetPhiResolution(20)
src1.SetThetaResolution(20)

randomVectors = vtkBrownianPoints()
randomVectors.SetMinimumSpeed(0)
randomVectors.SetMaximumSpeed(1)
randomVectors.SetInputConnection(src1.GetOutputPort())

mapper = vtkPolyDataMapper()
mapper.SetInputConnection(randomVectors.GetOutputPort())

actor = vtkActor()
actor.SetMapper(mapper)

# Load the material. Here, we are loading a material
# defined in the VTK Library. One can also specify
# a filename to a material description xml.
actor.GetProperty().LoadMaterialFromString(xmlMaterial)
# Set red color to show if shading fails.
actor.GetProperty().SetColor(1.0, 0, 0)
# Turn shading on. Otherwise, shaders are not used.
actor.GetProperty().ShadingOn()

# Map PointData.BrownianVectors (all 3 components) to genAttrVector
mapper.MapDataArrayToVertexAttribute("genAttrVector", "BrownianVectors", 0, -1)

renderer.AddActor(actor)
renderer.SetBackground(0.5, 0.5, 0.5)

renWin.Render()

renderer.GetActiveCamera().Azimuth(-50)
renderer.GetActiveCamera().Roll(70)

renWin.Render()

#iren.Start()
