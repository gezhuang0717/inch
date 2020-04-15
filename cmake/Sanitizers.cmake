function(enable_sanitizers project_name)

  if(NOT MSVC)
    set(SANITIZERS "")

    option(INCH_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" FALSE)
    if(INCH_ENABLE_SANITIZER_ADDRESS)
      list(APPEND SANITIZERS "address")
    endif()

    option(INCH_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" FALSE)
    if(INCH_ENABLE_SANITIZER_MEMORY)
      list(APPEND SANITIZERS "memory")
    endif()

    option(INCH_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR "Enable undefined behavior sanitizer" FALSE)
    if(INCH_ENABLE_SANITIZER_UNDEFINED_BEHAVIOR)
      list(APPEND SANITIZERS "undefined")
    endif()

    option(INCH_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" FALSE)
    if(INCH_ENABLE_SANITIZER_THREAD)
      list(APPEND SANITIZERS "thread")
    endif()

    list(JOIN SANITIZERS "," LIST_OF_SANITIZERS)
  endif()

  if(NOT "${LIST_OF_SANITIZERS}" STREQUAL "")
    target_compile_options(
      ${project_name}
      INTERFACE
      -fsanitize=${LIST_OF_SANITIZERS}
      )

    target_link_libraries(
      ${project_name}
      INTERFACE
      -fsanitize=${LIST_OF_SANITIZERS}
      )
    message(STATUS "Sanitizer list is: ${LIST_OF_SANITIZERS}")
  else()
    message(STATUS "Not using any sanitizers")
  endif()

endfunction()
