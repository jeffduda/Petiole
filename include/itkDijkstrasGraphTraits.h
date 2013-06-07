/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDijkstraGraphTraits.h,v $
  Language:  C++
  Date:
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkDijkstrasGraphTraits_h
#define __itkDijkstrasGraphTraits_h

#include "itkImageGraphTraits.h"

namespace itk
{

/**
 * Graph traits class for use with the DijkstrasPathGraphFilter class.
 */

template <typename TWeight = short, unsigned int VImageDimension = 3>
class DijkstrasGraphTraits : public ImageGraphTraits<TWeight, VImageDimension>
{
public:

  typedef DijkstrasGraphTraits                       Self;
  typedef ImageGraphTraits<TWeight, VImageDimension> Superclass;

  typedef TWeight                                 NodeWeightType;
  typedef TWeight                                 EdgeWeightType;
  typedef TWeight                                 ValueType;
  typedef typename Superclass::NodeIdentifierType NodeIdentifierType;
  typedef typename Superclass::EdgeIdentifierType EdgeIdentifierType;
  typedef typename Superclass::EdgeIdentifierContainerType
                                                  EdgeIdentifierContainerType;
  typedef typename Superclass::NodeIdentifierContainerType
                                                  NodeIdentifierContainerType;
  typedef typename Superclass::IndexType          IndexType;
  //typedef typename Superclass::EdgeType           EdgeType;
  //typedef typename Superclass::EdgePointerType    EdgePointerType;

  struct NodeType
    {
    NodeIdentifierType Identifier;
    EdgeIdentifierContainerType IncomingEdges;
    EdgeIdentifierContainerType OutgoingEdges;
    bool IsTarget;
    bool IsSource;
    bool Visited;
    bool IsActive;
    ValueType Weight;
    unsigned int TimeStamp;
    NodeIdentifierContainerType PreviousNodes;  
    };

  struct EdgeType
    {
    EdgeIdentifierType Identifier;
    NodeIdentifierType SourceIdentifier;
    NodeIdentifierType TargetIdentifier;
    EdgeIdentifierType ReverseEdgeIdentifier;
    EdgeWeightType Weight;
    bool IsActive;
    };

  typedef NodeType* NodePointerType;
  typedef EdgeType* EdgePointerType;
};

} // end namespace itk

#endif
