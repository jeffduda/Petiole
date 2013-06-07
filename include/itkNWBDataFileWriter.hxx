/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNWBDataFileWriter.hxx,v $
  Language:  C++
  Date:      $Date: 2009/03/04 23:10:58 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNWBDataFileWriter_hxx
#define __itkNWBDataFileWriter_hxx

#include "itkNWBDataFileWriter.h"
#include "itkImageFileWriter.h"
#include "itkByteSwapper.h"

#include <fstream>

namespace itk {

//
// Constructor
//
template<class TInputGraph>
NWBDataFileWriter<TInputGraph>
::NWBDataFileWriter()
{
  this->m_Input = NULL;
  this->m_FileName = "";
}

//
// Destructor
//
template<class TInputGraph>
NWBDataFileWriter<TInputGraph>
::~NWBDataFileWriter()
{
}

//
// Set the input mesh
//
template<class TInputGraph>
void
NWBDataFileWriter<TInputGraph>
::SetInput(InputGraphType * input)
{
  this->m_Input = input;
}

//
// Write the input mesh to the output file
//
template<class TInputGraph>
void NWBDataFileWriter<TInputGraph>
::Update()
{
  this->GenerateData();
}

//
// Write the input mesh to the output file
//
template<class TInputGraph>
void NWBDataFileWriter<TInputGraph>
::Write()
{
  this->GenerateData();
}

template<class TInputGraph>
void
NWBDataFileWriter<TInputGraph>
::GenerateData()
{
  if( this->m_FileName == "" )
    {
    itkExceptionMacro( "No FileName" );
    return;
    }
  //
  // Read output file
  //
  this->m_File.open( m_FileName.c_str(), std::ofstream::out );

  if( !this->m_File.is_open() )
    {
    itkExceptionMacro( "Unable to open file\n"
        "outputFilename= " << m_FileName );
    return;
    }

  /**
   * Get filename extension
   */
  std::string::size_type pos = this->m_FileName.rfind( "." );
  std::string extension( this->m_FileName, pos+1, this->m_FileName.length()-1 );

  if( extension == "nwb" )
    {
    this->WriteNWBFile();
    }
  else 
    {
    itkExceptionMacro( "Unknown extension: " << extension );
    }
  this->m_File.close();
}


template<class TInputGraph>
void
NWBDataFileWriter<TInputGraph>
::WriteNWBFile()
{
  this->m_File << "# Network Toolbox file" << std::endl;
  this->m_File << "# Written by itkNWBDataFileWriter" << std::endl;
  this->WriteNodes();
  this->WriteEdges();
}

template<class TInputGraph>
void
NWBDataFileWriter<TInputGraph>
::WriteNodes()
{
  this->m_File << "*Nodes " << this->GetInput()->GetTotalNumberOfNodes() << std::endl;
  this->m_File << "id*int\t" << "label*string\t" << "weight*float\t" << "node_type*string" << std::endl;
  for ( unsigned int i=0; i<this->GetInput()->GetTotalNumberOfNodes(); i++ )
    {
    this->m_File << (i+1) << "\t\t" << "\"" << (i+1) << "\"\t\t" 
                 << this->GetInput()->GetNodeWeight(i) << "\t\t"
                 << "\"node\"" << std::endl;
    }
}

template<class TInputGraph>
void
NWBDataFileWriter<TInputGraph>
::WriteEdges()
{
  if ( this->GetInput()->IsDirected() ) 
    {
    this->m_File << "*DirectedEdges " << this->GetInput()->GetTotalNumberOfEdges() << std::endl;
    }
  else 
    {
    this->m_File << "*UndirectedEdges " << this->GetInput()->GetTotalNumberOfEdges() << std::endl;
    }
  this->m_File << "source*int\t" << "target*int\t" << "weight*float\t" << "edge_type*string" << std::endl;
  for ( unsigned int i=0; i<this->GetInput()->GetTotalNumberOfEdges(); i++ )
    {
    this->m_File << this->GetInput()->GetSourceNode(i).Identifier+1 << "\t\t" 
                 << this->GetInput()->GetTargetNode(i).Identifier+1 << "\t\t" 
                 << this->GetInput()->GetEdgeWeight(i) << "\t\t"
                 << "\"edge\"" << std::endl;
    }
}


template<class TInputGraph>
void
NWBDataFileWriter<TInputGraph>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "FileName: " << this->m_FileName << std::endl;
}

} //end of namespace itk

#endif
