SET(VERSION 3.20)

PROJECT("raytracing")

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

add_executable("first_weekend" 
			main.cpp 
			Color.hpp 
			Vec3.hpp 
			Ray.hpp 
			Hittable.hpp 
			Sphere.hpp
			HittableList.hpp
			Utils.hpp
			Interval.hpp
			Camera.hpp
			Material.hpp
)