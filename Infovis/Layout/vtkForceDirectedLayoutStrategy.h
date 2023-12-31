// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright 2008 Sandia Corporation
// SPDX-License-Identifier: LicenseRef-BSD-3-Clause-Sandia-USGov
/**
 * @class   vtkForceDirectedLayoutStrategy
 * @brief   a force directed graph layout algorithm
 *
 *
 * Lays out a graph in 2D or 3D using a force-directed algorithm.
 * The user may specify whether to layout the graph randomly initially,
 * the bounds, the number of dimensions (2 or 3), and the cool-down rate.
 *
 * @par Thanks:
 * Thanks to Brian Wylie for adding functionality for allowing this layout
 * to be incremental.
 */

#ifndef vtkForceDirectedLayoutStrategy_h
#define vtkForceDirectedLayoutStrategy_h

#include "vtkGraphLayoutStrategy.h"
#include "vtkInfovisLayoutModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class VTKINFOVISLAYOUT_EXPORT vtkForceDirectedLayoutStrategy : public vtkGraphLayoutStrategy
{
public:
  static vtkForceDirectedLayoutStrategy* New();

  vtkTypeMacro(vtkForceDirectedLayoutStrategy, vtkGraphLayoutStrategy);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  ///@{
  /**
   * Seed the random number generator used to jitter point positions.
   * This has a significant effect on their final positions when
   * the layout is complete.
   */
  vtkSetClampMacro(RandomSeed, int, 0, VTK_INT_MAX);
  vtkGetMacro(RandomSeed, int);
  ///@}

  ///@{
  /**
   * Set / get the region in space in which to place the final graph.
   * The GraphBounds only affects the results if AutomaticBoundsComputation
   * is off.
   */
  vtkSetVector6Macro(GraphBounds, double);
  vtkGetVectorMacro(GraphBounds, double, 6);
  ///@}

  ///@{
  /**
   * Turn on/off automatic graph bounds calculation. If this
   * boolean is off, then the manually specified GraphBounds is used.
   * If on, then the input's bounds us used as the graph bounds.
   */
  vtkSetMacro(AutomaticBoundsComputation, vtkTypeBool);
  vtkGetMacro(AutomaticBoundsComputation, vtkTypeBool);
  vtkBooleanMacro(AutomaticBoundsComputation, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Set/Get the maximum number of iterations to be used.
   * The higher this number, the more iterations through the algorithm
   * is possible, and thus, the more the graph gets modified.
   * The default is '50' for no particular reason
   */
  vtkSetClampMacro(MaxNumberOfIterations, int, 0, VTK_INT_MAX);
  vtkGetMacro(MaxNumberOfIterations, int);
  ///@}

  ///@{
  /**
   * Set/Get the number of iterations per layout.
   * The only use for this ivar is for the application
   * to do visualizations of the layout before it's complete.
   * The default is '50' to match the default 'MaxNumberOfIterations'
   */
  vtkSetClampMacro(IterationsPerLayout, int, 0, VTK_INT_MAX);
  vtkGetMacro(IterationsPerLayout, int);
  ///@}

  ///@{
  /**
   * Set/Get the Cool-down rate.
   * The higher this number is, the longer it will take to "cool-down",
   * and thus, the more the graph will be modified.
   */
  vtkSetClampMacro(CoolDownRate, double, 0.01, VTK_DOUBLE_MAX);
  vtkGetMacro(CoolDownRate, double);
  ///@}

  ///@{
  /**
   * Turn on/off layout of graph in three dimensions. If off, graph
   * layout occurs in two dimensions. By default, three dimensional
   * layout is off.
   */
  vtkSetMacro(ThreeDimensionalLayout, vtkTypeBool);
  vtkGetMacro(ThreeDimensionalLayout, vtkTypeBool);
  vtkBooleanMacro(ThreeDimensionalLayout, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Turn on/off use of random positions within the graph bounds as initial points.
   */
  vtkSetMacro(RandomInitialPoints, vtkTypeBool);
  vtkGetMacro(RandomInitialPoints, vtkTypeBool);
  vtkBooleanMacro(RandomInitialPoints, vtkTypeBool);
  ///@}

  ///@{
  /**
   * Set the initial temperature.  If zero (the default) , the initial temperature
   * will be computed automatically.
   */
  vtkSetClampMacro(InitialTemperature, float, 0.0, VTK_FLOAT_MAX);
  vtkGetMacro(InitialTemperature, float);
  ///@}

  /**
   * This strategy sets up some data structures
   * for faster processing of each Layout() call
   */
  void Initialize() override;

  /**
   * This is the layout method where the graph that was
   * set in SetGraph() is laid out. The method can either
   * entirely layout the graph or iteratively lay out the
   * graph. If you have an iterative layout please implement
   * the IsLayoutComplete() method.
   */
  void Layout() override;

  /**
   * I'm an iterative layout so this method lets the caller
   * know if I'm done laying out the graph
   */
  int IsLayoutComplete() override { return this->LayoutComplete; }

protected:
  vtkForceDirectedLayoutStrategy();
  ~vtkForceDirectedLayoutStrategy() override;

  double GraphBounds[6];
  vtkTypeBool AutomaticBoundsComputation; // Boolean controls automatic bounds calc.
  int MaxNumberOfIterations;              // Maximum number of iterations.
  double CoolDownRate;                    // Cool-down rate.  Note:  Higher # = Slower rate.
  double InitialTemperature;
  vtkTypeBool ThreeDimensionalLayout; // Boolean for a third dimension.
  vtkTypeBool RandomInitialPoints;    // Boolean for having random points
private:
  // A vertex contains a position and a displacement.
  struct vtkLayoutVertex_t
  {
    double x[3];
    double d[3];
  };
  using vtkLayoutVertex = struct vtkLayoutVertex_t;

  // An edge consists of two vertices joined together.
  // This struct acts as a "pointer" to those two vertices.
  struct vtkLayoutEdge_t
  {
    int t;
    int u;
  };
  using vtkLayoutEdge = struct vtkLayoutEdge_t;

  int RandomSeed;
  int IterationsPerLayout;
  int TotalIterations;
  int LayoutComplete;
  double Temp;
  double optDist;
  vtkLayoutVertex* v;
  vtkLayoutEdge* e;

  vtkForceDirectedLayoutStrategy(const vtkForceDirectedLayoutStrategy&) = delete;
  void operator=(const vtkForceDirectedLayoutStrategy&) = delete;
};

VTK_ABI_NAMESPACE_END
#endif
