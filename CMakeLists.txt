project(ITKPetioleToolkit)
itk_module_impl()
  SET(ITK_PETIOLE_LIBRARIES
   ${ITKPetioleToolkit_LIBRARIES}
   )

#add_subdirectory(IO)
add_subdirectory(Utilities)

