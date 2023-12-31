// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkQuadricLODActor
 * @brief   a specific level-of-detail strategy using the
 * quadric clustering decimation algorithm
 *
 * vtkQuadricLODActor implements a specific strategy for level-of-detail
 * using the vtkQuadricClustering decimation algorithm. It supports only two
 * levels of detail: full resolution and a decimated version. The decimated
 * LOD is generated using a tuned strategy to produce output consistent with
 * the requested interactive frame rate (i.e., the
 * vtkRenderWindowInteractor's DesiredUpdateRate). It also makes use of
 * display lists for performance, and adjusts the vtkQuadricClustering
 * algorithm to take into account the dimensionality of the data (e.g., 2D,
 * x-y surfaces may be binned into n x n x 1 to reduce extra polygons in the
 * z-direction). Finally, the filter may optionally be set in "Static" mode
 * (this works with the vtkMapper::SetStatic() method). `Enabling Static results
 * in a one time execution of the Mapper's pipeline. After that, the pipeline
 * no longer updated (unless manually forced to do so).
 *
 * @warning
 * By default the algorithm is set up to pre-compute the LODs. That is, on
 * the first render (whether a full resolution render or interactive render)
 * the LOD is computed. This behavior can be changed so that the LOD
 * construction is deferred until the first interactive render. Either way,
 * when the LOD is constructed, the user may notice a short pause.
 *
 * @warning
 * This class can be used as a direct replacement for vtkActor. It may also be
 * used as a replacement for vtkFollower's (the ability to track a camera is
 * provided).
 *
 * @sa
 * vtkLODActor vtkQuadricClustering
 */

#ifndef vtkQuadricLODActor_h
#define vtkQuadricLODActor_h

#include "vtkActor.h"
#include "vtkRenderingLODModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class vtkQuadricClustering;
class vtkPolyDataMapper;
class vtkCamera;
class vtkPolyData;

class VTKRENDERINGLOD_EXPORT vtkQuadricLODActor : public vtkActor
{
public:
  /**
   * Creates a vtkQuadricLODActor.
   */
  static vtkQuadricLODActor* New();

  ///@{
  /**
   * Standard class methods.
   */
  vtkTypeMacro(vtkQuadricLODActor, vtkActor);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  ///@}

  ///@{
  /**
   * Specify whether to build the LOD immediately (i.e., on the first render)
   * or to wait until the LOD is requested in a subsequent render. By default,
   * LOD construction is not deferred (DeferLODConstruction is false).
   */
  vtkSetMacro(DeferLODConstruction, vtkTypeBool);
  vtkGetMacro(DeferLODConstruction, vtkTypeBool);
  vtkBooleanMacro(DeferLODConstruction, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Turn on/off a flag to control whether the underlying pipeline is static.
   * If static, this means that the data pipeline executes once and then not
   * again until the user manually modifies this class. By default, Static is
   * off because trying to debug this is tricky, and you should only use it
   * when you know what you are doing.
   */
  vtkSetMacro(Static, vtkTypeBool);
  vtkGetMacro(Static, vtkTypeBool);
  vtkBooleanMacro(Static, vtkTypeBool);
  ///@}

  enum DataConfigurationEnum
  {
    UNKNOWN = 0,
    XLINE,
    YLINE,
    ZLINE,
    XYPLANE,
    XZPLANE,
    YZPLANE,
    XYZVOLUME
  };

  ///@{
  /**
   * Force the binning of the quadric clustering according to application
   * knowledge relative to the dimension of the data. For example, if you
   * know your data lies in a 2D x-y plane, the performance of the quadric
   * clustering algorithm can be greatly improved by indicating this (i.e.,
   * the number of resulting triangles, and the quality of the decimation
   * version is better). Setting this parameter forces the binning to be
   * configured consistent with the dimnesionality of the data, and the
   * collapse dimension ratio is ignored. Specifying the value of
   * DataConfiguration to UNKNOWN (the default value) means that the class
   * will attempt to figure the dimension of the class automatically using
   * the CollapseDimensionRatio ivar.
   */
  vtkSetClampMacro(DataConfiguration, int, UNKNOWN, XYZVOLUME);
  vtkGetMacro(DataConfiguration, int);
  void SetDataConfigurationToUnknown() { this->SetDataConfiguration(UNKNOWN); }
  void SetDataConfigurationToXLine() { this->SetDataConfiguration(XLINE); }
  void SetDataConfigurationToYLine() { this->SetDataConfiguration(YLINE); }
  void SetDataConfigurationToZLine() { this->SetDataConfiguration(ZLINE); }
  void SetDataConfigurationToXYPlane() { this->SetDataConfiguration(XYPLANE); }
  void SetDataConfigurationToYZPlane() { this->SetDataConfiguration(YZPLANE); }
  void SetDataConfigurationToXZPlane() { this->SetDataConfiguration(XZPLANE); }
  void SetDataConfigurationToXYZVolume() { this->SetDataConfiguration(XYZVOLUME); }
  ///@}

  ///@{
  /**
   * If the data configuration is set to UNKNOWN, this class attempts to
   * figure out the dimensionality of the data using CollapseDimensionRatio.
   * This ivar is the ratio of short edge of the input bounding box to its
   * long edge, which is then used to collapse the data dimension (and set the
   * quadric bin size in that direction to one). By default, this value is 0.05.
   */
  vtkSetClampMacro(CollapseDimensionRatio, double, 0.0, 1.0);
  vtkGetMacro(CollapseDimensionRatio, double);
  ///@}

  ///@{
  /**
   * This class will create a vtkQuadricClustering algorithm automatically.
   * However, if you would like to specify the filter to use, or to access it
   * and configure it, these method provide access to the filter.
   */
  void SetLODFilter(vtkQuadricClustering* lodFilter);
  vtkGetObjectMacro(LODFilter, vtkQuadricClustering);
  ///@}

  enum PropTypeEnum
  {
    FOLLOWER = 0,
    ACTOR
  };

  ///@{
  /**
   * Indicate that this actor is actually a follower.
   * By default, the prop type is a vtkActor.
   */
  vtkSetClampMacro(PropType, int, FOLLOWER, ACTOR);
  vtkGetMacro(PropType, int);
  void SetPropTypeToFollower() { this->SetPropType(FOLLOWER); }
  void SetPropTypeToActor() { this->SetPropType(ACTOR); }
  ///@}

  ///@{
  /**
   * Set/Get the camera to follow. This method is only applicable when the
   * prop type is set to a vtkFollower.
   */
  void SetCamera(vtkCamera*);
  vtkGetObjectMacro(Camera, vtkCamera);
  ///@}

  /**
   * This causes the actor to be rendered. Depending on the frame rate request,
   * it will use either a full resolution render or an interactive render (i.e.,
   * it will use the decimated geometry).
   */
  void Render(vtkRenderer*, vtkMapper*) override;

  /**
   * Release any graphics resources that are being consumed by this actor.
   * The parameter window could be used to determine which graphic
   * resources to release.
   */
  void ReleaseGraphicsResources(vtkWindow*) override;

  /**
   * Shallow copy of an LOD actor. Overloads the virtual vtkProp method.
   */
  void ShallowCopy(vtkProp* prop) override;

protected:
  vtkQuadricLODActor();
  ~vtkQuadricLODActor() override;

  // Renders the LOD
  vtkActor* LODActor;
  vtkPolyDataMapper* LODMapper;

  // Keep track of the requested interactive frame rate
  double CachedInteractiveFrameRate;

  // Support various strategies
  vtkQuadricClustering* LODFilter;

  // Specify whether the mapper's should be set in to Static mode.
  vtkTypeBool Static;

  // The dimension of the data
  double CollapseDimensionRatio;
  int DataConfiguration;

  // Control whether this is a follower or regular actor
  int PropType;
  vtkCamera* Camera;

  // Specify to defer construction of the LOD.
  vtkTypeBool DeferLODConstruction;

  // Keep track of building
  vtkTimeStamp BuildTime;

private:
  vtkQuadricLODActor(const vtkQuadricLODActor&) = delete;
  void operator=(const vtkQuadricLODActor&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif
