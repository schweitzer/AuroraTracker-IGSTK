#
#  Define the list of source files forming the IGSTK Library
#
# The header files

SET(IGSTK_HEADS
    igstkAuroraTracker.h                    
    igstkCoordinateSystemSetTransformResult.h      
    igstkNDICommandInterpreterClassic.h  
    igstkSerialCommunicationForPosix.h    
    igstkTimeStamp.h
    igstkAuroraTrackerTool.h                
    igstkCoordinateSystemTransformToErrorResult.h  
    igstkNDICommandInterpreter.h         
    igstkSerialCommunicationForWindows.h  
    igstkTracker.h
    igstkBinaryData.h                       
    igstkCoordinateSystemTransformToResult.h       
    igstkNDIErrorEvent.h                 
    igstkSerialCommunication.h            
    igstkTrackerTool.h
    igstkCommunication.h                    
    igstkEvents.h                                  
    igstkNDITracker.h                    
    igstkSerialCommunicationSimulator.h   
    igstkTransformBase.h  
    igstkCoordinateSystemDelegator.h        
    igstkLogger.h                                  
    igstkObject.h                        
    igstkStateMachine.h                   
    igstkTransform.h
    igstkCoordinateSystem.h                 
    igstkMacros.h                                  
    igstkPulseGenerator.h                
    igstkStateMachineInput.h
    igstkCoordinateSystemInterfaceMacros.h  
    igstkNDIClassicTracker.h                       
    igstkRealTimeClock.h                 
    igstkStateMachineState.h

)



# The implementation files

SET(IGSTK_SRCS
    igstkAuroraTracker.cxx              
    igstkCoordinateSystemSetTransformResult.cxx      
    igstkNDICommandInterpreter.cxx  
    igstkSerialCommunicationForPosix.cxx    
    igstkTransformBase.cxx
    igstkAuroraTrackerTool.cxx          
    igstkCoordinateSystemTransformToErrorResult.cxx  
    igstkNDITracker.cxx             
    igstkSerialCommunicationForWindows.cxx  
    igstkTransform.cxx
    igstkBinaryData.cxx                 
    igstkCoordinateSystemTransformToResult.cxx       
    igstkObject.cxx                 
    igstkSerialCommunicationSimulator.cxx
    igstkCommunication.cxx              
    igstkLogger.cxx                                  
    igstkPulseGenerator.cxx         
    igstkTimeStamp.cxx
    igstkCoordinateSystem.cxx           
    igstkNDIClassicTracker.cxx                      
    igstkRealTimeClock.cxx          
    igstkTracker.cxx
    igstkCoordinateSystemDelegator.cxx  
    igstkNDICommandInterpreterClassic.cxx           
    igstkSerialCommunication.cxx   
    igstkTrackerTool.cxx

)




# Adding the IGSTK library

ADD_LIBRARY(IGSTK ${IGSTK_HEADS} ${IGSTK_SRCS})

# Linking with the FLTK, ITK and VTK libraries
TARGET_LINK_LIBRARIES( IGSTK
    ITKCommon
    vtkRendering vtkGraphics vtkHybrid vtkImaging 
    vtkIO vtkFiltering vtkCommon vtksys
    ${EXTRA_LIBS}
)

# Apply user-defined properties to the library target.
IF(IGSTK_LIBRARY_PROPERTIES)
  SET_TARGET_PROPERTIES(IGSTK PROPERTIES
     ${IGSTK_LIBRARY_PROPERTIES}
  )
ENDIF(IGSTK_LIBRARY_PROPERTIES)

IF(IGSTK_USE_FLTK)
  TARGET_LINK_LIBRARIES( IGSTK ${FLTK_LIBRARIES} )
ENDIF(IGSTK_USE_FLTK)

INSTALL(TARGETS IGSTK
   DESTINATION ${IGSTK_INSTALL_LIB_DIR}
)

FILE(GLOB __files1 "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
FILE(GLOB __files2 "${CMAKE_CURRENT_SOURCE_DIR}/*.txx")

INSTALL(FILES ${__files1} ${__files2}
   DESTINATION ${IGSTK_INSTALL_INCLUDE_DIR}
)
