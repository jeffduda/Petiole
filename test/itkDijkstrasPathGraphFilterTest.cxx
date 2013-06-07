/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkDijkstrasPathGraphFilterTest.cxx,v $
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
#include "itkDijkstrasPathGraphFilter.h"

int itkDijkstrasPathGraphFilterTest( int itkNotUsed(argc), char * itkNotUsed(argv) [] )
{

  // Read PolyData as PointSet
  // Can read into mesh, but cells will not be filled, just points
  typedef itk::DijkstrasGraphTraits<float,3>                   GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                          GraphType;
  typedef itk::DijkstrasPathGraphFilter<GraphType>             FilterType;

  typedef GraphType::NodePointerType NodePointerType;
  typedef GraphType::EdgePointerType EdgePointerType;

  GraphType::Pointer graph = GraphType::New();
  NodePointerType node1, node2, node3, node4, node5, node6;
  node1 = graph->CreateNewNode();
  node2 = graph->CreateNewNode();
  node3 = graph->CreateNewNode();
  node4 = graph->CreateNewNode();
  node5 = graph->CreateNewNode();
  node6 = graph->CreateNewNode();

  EdgePointerType edge1 = graph->CreateNewEdge();
  edge1->SourceIdentifier = 0;
  edge1->TargetIdentifier = 1;
  edge1->Weight = 7;
  EdgePointerType edge2 = graph->CreateNewEdge();  
  edge2->SourceIdentifier = 0;
  edge2->TargetIdentifier = 2;
  edge2->Weight = 9;
  EdgePointerType edge3 = graph->CreateNewEdge();  
  edge3->SourceIdentifier = 0;
  edge3->TargetIdentifier = 5;
  edge3->Weight = 14;
  EdgePointerType edge4 = graph->CreateNewEdge();  
  edge4->SourceIdentifier = 1;
  edge4->TargetIdentifier = 3;
  edge4->Weight = 15;
  EdgePointerType edge5 = graph->CreateNewEdge();  
  edge5->SourceIdentifier = 1;
  edge5->TargetIdentifier = 2;
  edge5->Weight = 10;
  EdgePointerType edge6 = graph->CreateNewEdge();  
  edge6->SourceIdentifier = 2;
  edge6->TargetIdentifier = 5;
  edge6->Weight = 2;
  EdgePointerType edge7 = graph->CreateNewEdge();  
  edge7->SourceIdentifier = 2;
  edge7->TargetIdentifier = 3;
  edge7->Weight = 11;
  EdgePointerType edge8 = graph->CreateNewEdge();  
  edge8->SourceIdentifier = 5;
  edge8->TargetIdentifier = 4;
  edge8->Weight = 9;
  EdgePointerType edge9 = graph->CreateNewEdge();  
  edge9->SourceIdentifier = 4;
  edge9->TargetIdentifier = 3;
  edge9->Weight = 6;

  GraphType::RegionType::Pointer sources = GraphType::RegionType::New();
  sources->InsertElement(0,0);

  GraphType::RegionType::Pointer targets = GraphType::RegionType::New();
  targets->InsertElement(0,4);

  FilterType::Pointer dijkstras = FilterType::New();
  dijkstras->SetInput( graph );
  dijkstras->SetSourceNodes( sources );
  dijkstras->SetTargetNodes( targets );
  dijkstras->Update();

  std::cout << "# paths = " << dijkstras->GetTree().size() << std::endl;
  std::cout << "path size = " << dijkstras->GetTree()[0].size() << std::endl;

  for ( unsigned int i=0; i<dijkstras->GetTree()[0].size(); i++)
    {
    std::cout << " " << dijkstras->GetTree()[0][i]->Identifier;
    }
  std::cout << std::endl;

  for ( unsigned int i=0; i<graph->GetTotalNumberOfNodes(); i++)
    {
    std::cout << graph->GetNodePointer(i)->Weight << " ";
    }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
