/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDijkstrasDistanceMatrixGraphFilterTest.cxx,v $
  Language:  C++
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include <iostream>
#include "itkDijkstrasGraphTraits.h"
#include "itkGraph.h"
#include "itkDijkstrasUniquePathsGraphFilter.h"

int itkDijkstrasUniquePathsGraphFilterTest( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{

  // Read PolyData as PointSet
  // Can read into mesh, but cells will not be filled, just points
  typedef itk::DijkstrasGraphTraits<float,3>                   GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                          GraphType;
  typedef itk::DijkstrasUniquePathsGraphFilter<GraphType>   FilterType;

  typedef GraphType::NodePointerType NodePointerType;
  typedef GraphType::EdgePointerType EdgePointerType;

  GraphType::Pointer graph = GraphType::New();
  NodePointerType node0, node1, node2, node3, node4, node5, node6, node7, node8, node9, node10;
  node0 = graph->CreateNewNode();
  node1 = graph->CreateNewNode();
  node2 = graph->CreateNewNode();
  node3 = graph->CreateNewNode();
  node4 = graph->CreateNewNode();
  node5 = graph->CreateNewNode();
  node6 = graph->CreateNewNode();
  node7 = graph->CreateNewNode();
  node8 = graph->CreateNewNode();
  node9 = graph->CreateNewNode();
  node10 = graph->CreateNewNode();

  EdgePointerType edge0 = graph->CreateNewEdge();
  edge0->SourceIdentifier = 0;
  edge0->TargetIdentifier = 1;
  edge0->Weight = 7;
  EdgePointerType edge1 = graph->CreateNewEdge();  
  edge1->SourceIdentifier = 0;
  edge1->TargetIdentifier = 2;
  edge1->Weight = 9;
  EdgePointerType edge2 = graph->CreateNewEdge();  
  edge2->SourceIdentifier = 0;
  edge2->TargetIdentifier = 5;
  edge2->Weight = 14;
  EdgePointerType edge3 = graph->CreateNewEdge();  
  edge3->SourceIdentifier = 1;
  edge3->TargetIdentifier = 3;
  edge3->Weight = 15;
  EdgePointerType edge4 = graph->CreateNewEdge();  
  edge4->SourceIdentifier = 1;
  edge4->TargetIdentifier = 2;
  edge4->Weight = 10;
  EdgePointerType edge5 = graph->CreateNewEdge();  
  edge5->SourceIdentifier = 2;
  edge5->TargetIdentifier = 5;
  edge5->Weight = 2;
  EdgePointerType edge6 = graph->CreateNewEdge();  
  edge6->SourceIdentifier = 2;
  edge6->TargetIdentifier = 3;
  edge6->Weight = 11;
  EdgePointerType edge7 = graph->CreateNewEdge();  
  edge7->SourceIdentifier = 5;
  edge7->TargetIdentifier = 4;
  edge7->Weight = 9;
  EdgePointerType edge8 = graph->CreateNewEdge();  
  edge8->SourceIdentifier = 4;
  edge8->TargetIdentifier = 3;
  edge8->Weight = 6;
  edge3->Weight = 15;

  EdgePointerType edge9 = graph->CreateNewEdge();  
  edge9->SourceIdentifier = 1;
  edge9->TargetIdentifier = 6;
  edge9->Weight = 10;
  EdgePointerType edge10 = graph->CreateNewEdge();  
  edge10->SourceIdentifier = 6;
  edge10->TargetIdentifier = 7;
  edge10->Weight = 2;
  EdgePointerType edge11 = graph->CreateNewEdge();  
  edge11->SourceIdentifier = 7;
  edge11->TargetIdentifier = 8;
  edge11->Weight = 11;
  EdgePointerType edge12 = graph->CreateNewEdge();  
  edge12->SourceIdentifier = 7;
  edge12->TargetIdentifier = 9;
  edge12->Weight = 9;
  EdgePointerType edge13 = graph->CreateNewEdge();  
  edge13->SourceIdentifier = 9;
  edge13->TargetIdentifier = 10;
  edge13->Weight = 6;




  FilterType::Pointer dijkstras = FilterType::New();
  dijkstras->SetInput( graph );
  dijkstras->Update();

  std::cout << "Number Of Unique Paths" << std::endl;
  std::cout << dijkstras->GetUniquePathsMatrix() << std::endl;

  return EXIT_SUCCESS;
}
