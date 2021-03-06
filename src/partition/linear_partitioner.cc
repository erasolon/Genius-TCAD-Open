// $Id: linear_partitioner.C,v 1.14 2007-10-21 20:48:52 benkirk Exp $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2007  Benjamin S. Kirk, John W. Peterson

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



// C++ Includes   -----------------------------------

// Local Includes -----------------------------------
#include "mesh_base.h"
#include "linear_partitioner.h"
#include "perf_log.h"
#include "elem.h"


// ------------------------------------------------------------
// LinearPartitioner implementation
void LinearPartitioner::_do_partition (MeshBase& mesh, const unsigned int n_pieces)
{
  assert (n_pieces > 0);

  // Check for an easy return
  if (n_pieces == 1)
    {
      this->single_partition (mesh);
      return;
    }

  START_LOG ("partition()", "LinearPartitioner");

  // Create a simple linear partitioning
  for(unsigned int c=0; c<_clusters.size(); ++c)
  {
    Cluster * cluster = _clusters[c];

    const unsigned int n_elem     = cluster->elems.size();
    const unsigned int blksize    = n_elem/n_pieces;

    for (unsigned int n=0; n<n_elem; ++n)
    {
      short int processor_id;
      if ((n/blksize) < n_pieces)
        processor_id = n/blksize;
      else
        processor_id = 0;

      Elem * elem = const_cast<Elem *>(cluster->elems[n]);
      elem->processor_id() = processor_id;
    }
  }

  STOP_LOG ("partition()", "LinearPartitioner");

}
