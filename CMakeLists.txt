cmake_minimum_required(VERSION 3.16)
project(draw_pixels_from_scratch)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(DRAW_PIXEL_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/draw_pixel.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/draw_pixel.h
)

add_library(draw_pixel)
target_sources(draw_pixel
 PRIVATE
  ${DRAW_PIXEL_SOURCES}
  )

target_include_directories(draw_pixel
 PRIVATE
 ${DRAW_PIXEL_SOURCES}
)

set(HELPER_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/helper.c
  ${CMAKE_CURRENT_SOURCE_DIR}/include/helper.h
)

add_library(helper)
target_sources(helper
 PRIVATE
  ${HELPER_SOURCES}
  )

target_include_directories(helper
 PRIVATE
 ${HELPER_SOURCES}
)

set(MAIN_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
  )

add_executable(main)
target_sources(main
 PRIVATE
  ${MAIN_SRC}
)

target_link_libraries(main
  draw_pixel
  helper
  m
)

set(2D_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/2d_thinking_exercises.c
  )

add_executable(2d_thinking_exercises)
target_sources(2d_thinking_exercises
 PRIVATE
  ${2D_SRC}
)

set(RASTER_GRAPHICS_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/raster_graphics_mini_excercises.c
  )

add_executable(raster_graphics_mini_excercises)
target_sources(raster_graphics_mini_excercises
 PRIVATE
  ${RASTER_GRAPHICS_SRC}
)
