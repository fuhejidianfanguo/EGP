function(egp_collect_source out_var)
  file(
        GLOB_RECURSE sources
        CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
    )
  set(${out_var} ${sources} PARENT_SCOPE)
endfunction()
