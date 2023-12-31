// SPDX-FileCopyrightText: Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
// SPDX-License-Identifier: BSD-3-Clause
#include "vtkSocketCollection.h"

#include "vtkCollectionIterator.h"
#include "vtkObjectFactory.h"
#include "vtkSocket.h"

VTK_ABI_NAMESPACE_BEGIN
vtkStandardNewMacro(vtkSocketCollection);
//------------------------------------------------------------------------------
vtkSocketCollection::vtkSocketCollection()
{
  this->SelectedSocket = nullptr;
}

//------------------------------------------------------------------------------
vtkSocketCollection::~vtkSocketCollection() = default;

//------------------------------------------------------------------------------
void vtkSocketCollection::AddItem(vtkSocket* soc)
{
  this->Superclass::AddItem(soc);
}

//------------------------------------------------------------------------------
int vtkSocketCollection::SelectSockets(unsigned long msec /*=0*/)
{
  // clear last selected socket.
  this->SelectedSocket = nullptr;

  int max = this->GetNumberOfItems();
  if (max <= 0)
  {
    vtkErrorMacro("No sockets to select.");
    return -1;
  }

  int* socket_indices = new int[max];
  int* sockets_to_select = new int[max];
  int no_of_sockets = 0;

  vtkCollectionIterator* iter = this->NewIterator();

  int index = 0;
  for (iter->InitTraversal(); !iter->IsDoneWithTraversal(); iter->GoToNextItem(), index++)
  {
    vtkSocket* soc = vtkSocket::SafeDownCast(iter->GetCurrentObject());
    if (!soc->GetConnected())
    {
      // skip not-connected sockets.
      continue;
    }
    int sockfd = soc->GetSocketDescriptor();
    sockets_to_select[no_of_sockets] = sockfd;
    socket_indices[no_of_sockets] = index;
    no_of_sockets++;
  }

  if (no_of_sockets == 0)
  {
    vtkErrorMacro("No alive sockets!");
    delete[] sockets_to_select;
    delete[] socket_indices;
    return -1;
  }
  int res = vtkSocket::SelectSockets(sockets_to_select, no_of_sockets, msec, &index);
  int actual_index = -1;
  if (index != -1)
  {
    actual_index = socket_indices[index];
  }

  iter->Delete();
  delete[] sockets_to_select;
  delete[] socket_indices;

  if (res <= 0 || index == -1)
  {
    return res;
  }

  this->SelectedSocket = vtkSocket::SafeDownCast(this->GetItemAsObject(actual_index));
  return 1;
}

//------------------------------------------------------------------------------
void vtkSocketCollection::RemoveItem(vtkObject* a)
{
  if (this->SelectedSocket && this->SelectedSocket == a)
  {
    this->SelectedSocket = nullptr;
  }
  this->Superclass::RemoveItem(a);
}

//------------------------------------------------------------------------------
void vtkSocketCollection::RemoveItem(int i)
{
  if (this->SelectedSocket && this->GetItemAsObject(i) == this->SelectedSocket)
  {
    this->SelectedSocket = nullptr;
  }
  this->Superclass::RemoveItem(i);
}

//------------------------------------------------------------------------------
void vtkSocketCollection::ReplaceItem(int i, vtkObject* a)
{
  if (this->SelectedSocket && this->GetItemAsObject(i) == this->SelectedSocket)
  {
    this->SelectedSocket = nullptr;
  }
  this->Superclass::ReplaceItem(i, a);
}

//------------------------------------------------------------------------------
void vtkSocketCollection::RemoveAllItems()
{
  this->SelectedSocket = nullptr;
  this->Superclass::RemoveAllItems();
}

//------------------------------------------------------------------------------
void vtkSocketCollection::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}
VTK_ABI_NAMESPACE_END
