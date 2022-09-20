project(flash2raw)

find_package(HDF5 COMPONENTS CXX)
if (HDF5_FOUND)
  add_executable(flash2cells
    flash2cells.cpp
    )
  include_directories(${HDF5_CXX_INCLUDE_DIRS})
  target_link_libraries(flash2cells ${HDF5_LIBRARIES})
endif()
add_executable(cells2raw
  cells2raw.cpp
  )
