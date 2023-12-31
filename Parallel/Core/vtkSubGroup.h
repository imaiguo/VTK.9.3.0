// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-FileCopyrightText: Copyright (c) Kitware, Inc.
// SPDX-FileCopyrightText: Copyright (c) Sandia Corporation
// SPDX-License-Identifier: BSD-3-Clause
/**
 * @class   vtkSubGroup
 * @brief   scalable collective communication for a
 *      subset of members of a parallel VTK application
 *
 *
 *     This class provides scalable broadcast, reduce, etc. using
 *     only a vtkMultiProcessController. It does not require MPI.
 *     Users are vtkPKdTree and vtkDistributedDataFilter.
 *
 * @attention
 * This class will be deprecated soon.  Instead of using this class, use the
 * collective and subgrouping operations now built into
 * vtkMultiProcessController.  The only reason this class is not deprecated
 * already is because vtkPKdTree relies heavily on this class in ways that are
 * not easy to work around.  Since vtkPKdTree is due for a major overhaul
 * anyway, we are leaving things the way they are for now.
 *
 * @sa
 *      vtkPKdTree vtkDistributedDataFilter
 */

#ifndef vtkSubGroup_h
#define vtkSubGroup_h

#include "vtkObject.h"
#include "vtkParallelCoreModule.h" // For export macro

VTK_ABI_NAMESPACE_BEGIN
class vtkMultiProcessController;
class vtkCommunicator;

class VTKPARALLELCORE_EXPORT vtkSubGroup : public vtkObject
{
public:
  vtkTypeMacro(vtkSubGroup, vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent) override;
  static vtkSubGroup* New();

  // The wrapper gets confused here and falls down.
  enum
  {
    MINOP = 1,
    MAXOP = 2,
    SUMOP = 3
  };

  /**
   * Initialize a communication subgroup for the processes
   * with rank p0 through p1 of the given communicator.  (So
   * vtkSubGroup is limited to working with subgroups that
   * are identified by a contiguous set of rank IDs.)
   * The third argument is the callers rank, which must
   * in the range from p0 through p1.
   */

  int Initialize(int p0, int p1, int me, int tag, vtkCommunicator* c);

  int Gather(int* data, int* to, int length, int root);
  int Gather(char* data, char* to, int length, int root);
  int Gather(float* data, float* to, int length, int root);
#ifdef VTK_USE_64BIT_IDS
  int Gather(vtkIdType* data, vtkIdType* to, int length, int root);
#endif
  int Broadcast(float* data, int length, int root);
  int Broadcast(double* data, int length, int root);
  int Broadcast(int* data, int length, int root);
  int Broadcast(char* data, int length, int root);
#ifdef VTK_USE_64BIT_IDS
  int Broadcast(vtkIdType* data, int length, int root);
#endif
  int ReduceSum(int* data, int* to, int length, int root);
  int ReduceMax(float* data, float* to, int length, int root);
  int ReduceMax(double* data, double* to, int length, int root);
  int ReduceMax(int* data, int* to, int length, int root);
  int ReduceMin(float* data, float* to, int length, int root);
  int ReduceMin(double* data, double* to, int length, int root);
  int ReduceMin(int* data, int* to, int length, int root);

  int AllReduceUniqueList(int* list, int len, int** newList);
  int MergeSortedUnique(int* list1, int len1, int* list2, int len2, int** newList);

  void setGatherPattern(int root, int length);
  int getLocalRank(int processID);

  int Barrier();

  void PrintSubGroup() const;

  static int MakeSortedUnique(int* list, int len, int** newList);

  int tag;

protected:
  vtkSubGroup();
  ~vtkSubGroup() override;

private:
  int computeFanInTargets();
  void restoreRoot(int rootLoc);
  void moveRoot(int rootLoc);
  void setUpRoot(int root);

  int nFrom;
  int nTo;

  int sendId; // gather
  int sendOffset;
  int sendLength;

  int recvId[20];
  int recvOffset[20];
  int recvLength[20];
  int fanInFrom[20]; // reduce, broadcast

  int fanInTo;
  int nSend;
  int nRecv;
  int gatherRoot;
  int gatherLength;

  int* members;
  int nmembers;
  int myLocalRank;

  vtkCommunicator* comm;

  vtkSubGroup(const vtkSubGroup&) = delete;
  void operator=(const vtkSubGroup&) = delete;
};
VTK_ABI_NAMESPACE_END
#endif
