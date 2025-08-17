macro(register_gcc_flags target_name)
  if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(FATAL_ERROR "register_gcc_flags can only be used with GCC")
  endif()

  # Set C++ and C standards
  set_target_properties(
    ${target_name}
    PROPERTIES
      CXX_STANDARD 23
      CXX_STANDARD_REQUIRED YES
      C_STANDARD 99
      C_STANDARD_REQUIRED YES
  )

  # Determine compile option scope
  get_target_property(type ${target_name} TYPE)
  if(type STREQUAL "INTERFACE_LIBRARY")
    set(opt_scope INTERFACE)
  else()
    set(opt_scope PRIVATE)
  endif()

  # Apply warnings and flags
  target_compile_options(${target_name} ${opt_scope}
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -g
    -Wold-style-cast
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wduplicated-cond
    -Wlogical-op
    -Wconversion
    -Wformat
    -Wformat=2
    -Wimplicit-fallthrough=5
    -Wstrict-overflow
    -Wfloat-equal
    -Wcast-qual
    -fno-common
    # -Wsign-conversion
    -Wshadow
    -Wundef
  )
endmacro()
