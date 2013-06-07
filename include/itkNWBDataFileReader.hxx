/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNWBDataFileReader.hxx,v $
  Language:  C++
  Date:      $Date: 2009/03/13 19:48:16 $
  Version:   $Revision: 1.23 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNWBDataFileReader_hxx
#define __itkNWBDataFileReader_hxx

#include "itkNWBDataFileReader.h"

#include "itkBinaryThresholdImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkLabelContourImageFilter.h"
#include "itkMersenneTwisterRandomVariateGenerator.h"
#include "itkByteSwapper.h"

#include <fstream>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>

namespace itk {

//
// Constructor
//
template<class TOutputMesh>
NWBDataFileReader<TOutputMesh>
::NWBDataFileReader()
{
  typename TOutputMesh::Pointer output = TOutputMesh::New();
  this->ProcessObject::SetNumberOfRequiredOutputs( 1 );
  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );
}

template<class TOutputMesh>
void
NWBDataFileReader<TOutputMesh>
::GenerateData()
{
  if( this->m_FileName == "" )
    {
    itkExceptionMacro( "No input FileName" );
    return;
    }

  // Read input file
  this->m_File.open( m_FileName.c_str(), std::ifstream::in);

  if( !this->m_File.is_open() )
    {
    itkExceptionMacro("Unable to open file\n"
        "inputFilename= " << this->m_FileName );
    return;
    }

  typename OutputMeshType::Pointer outputMesh = this->GetOutput();
  this->m_UndirectedEdges = LineSetType::New();
  this->m_DirectedEdges = LineSetType::New();

  std::string::size_type pos = this->m_FileName.rfind( "." );
  std::string extension( this->m_FileName, pos+1, this->m_FileName.length()-1 );

  if( extension == "nwb" )
    {
    this->ReadNWBFile();
    }
  else 
    {
    this->m_File.close();
    std::cerr << "Invalid file extension" << std::endl;
    return;
    }
 
  this->m_File.close();

}

template<class TOutputMesh>
void
NWBDataFileReader<TOutputMesh>
::ReadNWBFile()
{
  std::string line;
  while ( ! this->m_File.eof() )
    {
    std::getline( this->m_File, line );
    if ( line.find( "*Nodes" ) != std::string::npos )
      {
      this->ReadNodes( line );
      }
    else if ( line.find( "*DirectedEdges" ) != std::string::npos )
      {
      this->ReadEdges( line, true );
      }
    else if ( line.find( "*UndirectedEdges" ) != std::string::npos )
      {
      this->ReadEdges( line, false );
      }
    } 
}

template<class TOutputMesh>
void
NWBDataFileReader<TOutputMesh>
::ReadNodes(std::string headline)
{
  
  typename OutputMeshType::Pointer outputMesh = this->GetOutput();

  std::istringstream instream;
  instream.str( headline );
  std::string nodesFlag;
  int numberOfNodes = 0;
  instream >> nodesFlag >> numberOfNodes;
  if ( numberOfNodes > 0 )
    {
    outputMesh->GetPoints()->Reserve( numberOfNodes );
    }

  std::string line;
  std::getline( this->m_File, line );
  instream.clear();
  instream.str( line );

  std::string sourceStr, labelStr, weightStr, node_typeStr;
  instream >> sourceStr >> labelStr >> weightStr >> node_typeStr;

  PointType point;
  bool readingNodes = true;
  int nodeIdx = 0;

  while( readingNodes )
    {
    std::getline( this->m_File, line );
    instream.clear();
    instream.str( line );
    
    unsigned int idx = 0;
    std::string label, type;
    float weight;
    instream >> idx >> label >> weight >> type;

    if ( idx > 0 )
      {
      outputMesh->SetPoint( nodeIdx, point );
      ++nodeIdx;
      }
    else
      {
      readingNodes = false;
      }

    if ( (numberOfNodes > 0 ) && ( nodeIdx >= numberOfNodes ) )
      {
      readingNodes = false;
      }

    if ( line.find( "*DirectedEdges" ) != std::string::npos )
      {
      readingNodes = false;
      this->ReadEdges( line, true );
      }
    else if ( line.find( "*UndirectedEdges" ) != std::string::npos )
      {
      readingNodes = false;
      this->ReadEdges( line, false );
      }
    }

}

template<class TOutputMesh>
void
NWBDataFileReader<TOutputMesh>
::ReadEdges( std::string headline, bool directed )
{
  typename OutputMeshType::Pointer outputMesh = this->GetOutput();

  if ( ( directed ) && ( this->GetDirectedEdges()->Size() > 0 ) )
    {
    itkExceptionMacro("Only one directed edge list is allowed\n" );
    return;
    }
  else if ( ( ! directed ) && ( this->GetUndirectedEdges()->Size() > 0 ))
    {
    itkExceptionMacro("Only one undirected edge list is allowed\n" );
    return;
    }

  if ( directed ) 
    {
    this->m_DirectedEdges->Initialize();
    }
  else
    {
    this->m_UndirectedEdges->Initialize();
    }

  std::istringstream instream;
  instream.str( headline );
  std::string edgeFlag;
  unsigned int numberOfEdges = 0;
  instream >> edgeFlag >> numberOfEdges;

  std::string line;
  std::getline( this->m_File, line );
  instream.clear();
  instream.str( line );
  
  std::string sourceLbl;
  std::string targetLbl;
  std::string weightLbl;
  std::string typeLbl;

  instream >> sourceLbl >> targetLbl >> weightLbl >> typeLbl;
  
  bool readingEdges = true;
  unsigned int edgeIdx = 0;

  while ( readingEdges )
    {
    std::getline( this->m_File, line );

    if ( ( directed ) && ( line.find( "*UndirectedEdges" ) != std::string::npos ) )
      {
      readingEdges = false;
      this->ReadEdges( line, false );
      }     
    else if ( ( !directed ) && ( line.find( "*DirectedEdges" ) != std::string::npos ) )
      {
      readingEdges = false;
      this->ReadEdges( line, true );
      }
    else 
      {
      instream.clear();
      instream.str( line );
      
      unsigned int source = 0;
      unsigned int target = 0;
      float weight = 0.0;
      std::string type;

      instream >> source >> target >> weight >> type;
   
      if ( ( source > 0 ) && ( target > 0 ) )
        {
        
        LineType polyLine;
        polyLine.SetSize( 2 );
        polyLine[0] = source-1;  
        polyLine[1] = target-1; 
        
        if ( directed ) 
          {
          this->m_DirectedEdges->InsertElement( edgeIdx, polyLine );
          }
        else 
          {
          this->m_UndirectedEdges->InsertElement( edgeIdx, polyLine );
          }
        ++edgeIdx;
        }
      else
        {
        readingEdges = false;
        }

      }

    if ( (numberOfEdges > 0) && (edgeIdx >= numberOfEdges) )
      {
      readingEdges = false;
      }
    
    }


}

template<class TOutputMesh>
void
NWBDataFileReader<TOutputMesh>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "FileName: "
     << this->m_FileName << std::endl;

}

} //end of namespace itk


#endif
