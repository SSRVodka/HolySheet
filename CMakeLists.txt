cmake_minimum_required(VERSION 3.21)
project(HolySheet)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE release)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -ggdb")

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

set(main_src
  main.cpp
  src/calc.cpp
  src/Cell.cpp
  src/HolySheet.cpp
  src/findDialog.cpp
  src/goToCellDialog.cpp
  src/sortDialog.cpp
  src/MainWindow.cpp
  icon.rc
)

set(main_headers
  include/calc.h
  include/Cell.h
  include/linStack.h
)

set(moc_headers
  include/HolySheet.h
  include/findDialog.h
  include/goToCellDialog.h
  include/sortDialog.h
  include/MainWindow.h
)

set(form_src
  ui/goToCellDialog.ui
  ui/sortDialog.ui
)

set(qcc_qrc
  resources.qrc
)

qt5_wrap_cpp(mocd_h ${moc_headers})
qt5_wrap_ui(formd_src ${form_src})
qt5_add_resources(qccd_qrc ${qcc_qrc})

add_executable(
    ${PROJECT_NAME}
    WIN32
      ${main_src}
      ${mocd_h}
      ${formd_src}
      ${qccd_qrc}
      ${moc_headers}
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
