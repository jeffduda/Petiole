/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMatrixToGraphFilter.hxx,v $
  Language:  C++
  Date:      $Date: 2008/11/11 11:51:34 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMatrixToGraphFilter_hxx
#define __itkMatrixToGraphFilter_hxx

#include "vnl/vnl_matlab_read.h"
#include "itkMatrixToGraphFilter.h"
#include "itkByteSwapper.h"
#include "itk_zlib.h"

namespace itk
{

/**
 *
 */
template <class TGraph>
MatrixToGraphFilter<TGraph>
::MatrixToGraphFilter()
{
  this->ProcessObject::SetNumberOfRequiredInputs( 0 );

  GraphPointer output
    = dynamic_cast<GraphType*>( this->MakeOutput( 0 ).GetPointer() );

  this->ProcessObject::SetNumberOfRequiredOutputs( 1 );
  this->ProcessObject::SetNthOutput( 0, output.GetPointer() );

  this->m_IsDirected = false;
}

/**
 *
 */
template <class TGraph>
MatrixToGraphFilter<TGraph>
::~MatrixToGraphFilter()
{
}


/**
 *   Make Ouput
 */
template <class TGraph>
DataObject::Pointer
MatrixToGraphFilter<TGraph>
::MakeOutput( ProcessObject::DataObjectPointerArraySizeType )
{
  GraphPointer  outputGraph = GraphType::New();
  return dynamic_cast< DataObject *>( outputGraph.GetPointer() );
}

template <class TGraph>
typename MatrixToGraphFilter<TGraph>::GraphType *
MatrixToGraphFilter<TGraph>
::GetOutput( void )
{
  return dynamic_cast<GraphType*>
    ( this->ProcessObject::GetOutput( 0 ) );
}


/**
 *
 */
template <class TGraph>
void
MatrixToGraphFilter<TGraph>
::GenerateData()
{

  if ( this->m_Matrix.Cols() < 1 ) 
    {
    itkExceptionMacro( "Matrix is empty" );
    }
  if ( this->m_Matrix.Cols() != this->m_Matrix.Rows() ) 
    {
    itkExceptionMacro( "Matrix must be square" );
    }
  if ( ! this->m_IsDirected )
    {
    for ( unsigned int i=0; i<this->m_Matrix.Cols(); i++ )
      {
      for ( unsigned int j=(i+1); j<this->m_Matrix.Rows(); j++)
        {
        if ( this->m_Matrix(i,j) != this->m_Matrix(j,i) )
          {
          itkExceptionMacro( "Matrix must be symmetric for undirected graph" );
          }
        }
      }
    }

  GraphPointer output = this->GetOutput();
  output->SetIsDirected( this->m_IsDirected );

  for (unsigned int i=0; i<this->m_Matrix.Cols(); i++)
    {
    output->CreateNewNode();
    }
  
  for (unsigned int i=0; i<this->m_Matrix.Cols(); i++)
    {
    unsigned int startJ = 0;
    if ( !this->m_IsDirected )
      {
      startJ = i+1;
      }
    
    for (unsigned int j=startJ; j<this->m_Matrix.Rows(); j++)
      {
      if ( this->m_Matrix(j,i) != 0 )
        {
        output->CreateNewEdge(j,i,this->m_Matrix(j,i));
        //EdgePointerType edge = output->CreateNewEdge(j,i,this->m_Matrix(j,i));
        }

      }
    }


}

/**
 *
 */
template <class TGraph>
void
MatrixToGraphFilter<TGraph>
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
}

/**
 * copy information from first input to all outputs
 * This is a void implementation to prevent the
 * ProcessObject version to be called
 */
template <class TGraph>
void
MatrixToGraphFilter<TGraph>
::GenerateOutputInformation()
{
}


} // end namespace itk

#endif
