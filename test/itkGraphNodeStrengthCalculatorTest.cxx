/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkGraphTest.cxx,v $
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
#include "itkImageGraphTraits.h"
#include "itkGraph.h"
#include "itkGraphNodeStrengthCalculator.h"
#include "itkMatrixToGraphFilter.h"
#include "itkMatlabLevel5FileReader.h"

int itkGraphNodeStrengthCalculatorTest( int argc, char * argv [] )
{

  // Read PolyData as PointSet
  // Can read into mesh, but cells will not be filled, just points
  typedef itk::ImageGraphTraits<int,3>                        GraphTraitsType;
  typedef itk::Graph<GraphTraitsType>                         GraphType;
  typedef itk::GraphNodeStrengthCalculator<GraphType>         CalculatorType;
  typedef itk::MatrixToGraphFilter<GraphType>                 FilterType;
  typedef itk::MatlabLevel5FileReader<float>                  ReaderType;

  typedef GraphType::NodePointerType NodePointerType;
  typedef GraphType::EdgePointerType EdgePointerType;

  GraphType::Pointer graph;

  if ( argc >= 2 )
    {
    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileName( argv[1] );
    reader->Update();

    FilterType::MatrixType matrix;
    matrix.SetSize( reader->GetVariableDimensions(1)->GetElement(0),
                    reader->GetVariableDimensions(1)->GetElement(1) );

    std::cout << "Data file: " << argv[1] << std::endl;
    std::cout << "Data variable: " << reader->GetVariableName(1) << std::endl;
    std::cout << "Matrix size: " << matrix.Rows() << "x" << matrix.Cols() << std::endl;
    
    matrix.Fill(0);
    unsigned int idx = 0;
    for ( unsigned int i=0; i<reader->GetVariableDimensions(1)->GetElement(0); i++) 
      {
      for ( unsigned int j=0; j<reader->GetVariableDimensions(1)->GetElement(1); j++) 
        {
        matrix(j,i) = reader->GetVariableData(1)->GetElement( idx );
        ++idx;
        }
      }

    for (unsigned int i=0; i<matrix.Cols(); i++)
      {
      std::cout << matrix(0,i) << " ";
      }
    std::cout << std::endl;
    
    FilterType::Pointer filter = FilterType::New();
    filter->SetMatrix( matrix );
    filter->SetIsDirected( true );
    filter->Update();

    graph = filter->GetOutput();
    }
  else
    {
    graph = GraphType::New();
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
    
    }

  graph->SetIncomingAndOutgoingEdges();
  
  CalculatorType::Pointer calc = CalculatorType::New();
  calc->SetGraph( graph );
  
  calc->ComputeUndirectedStrength();
  std::cout << "Undirected: ";
  for (unsigned int i=0; i<graph->GetTotalNumberOfNodes(); i++)
    {
    std::cout << calc->GetStrength( i ) << " ";
    }
  std::cout << std::endl;

  calc->ComputeInStrength();
  std::cout << "In-Strength: ";
  for (unsigned int i=0; i<graph->GetTotalNumberOfNodes(); i++)
    {
    std::cout << calc->GetStrength( i ) << " ";
    }
  std::cout << std::endl;

  calc->ComputeOutStrength();
  std::cout << "Out-Strength: ";
  for (unsigned int i=0; i<graph->GetTotalNumberOfNodes(); i++)
    {
    std::cout << calc->GetStrength( i ) << " ";
    }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}
