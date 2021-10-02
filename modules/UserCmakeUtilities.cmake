# A handy function to add the current source directory to a local
# filename. To be used for creating a list of sources.
function(convert_filenames_to_full_paths NAMES)
    unset(tmp_names)
    foreach(name ${${NAMES}})
        list(APPEND tmp_names ${CMAKE_CURRENT_SOURCE_DIR}/${name})
    endforeach()
    set(${NAMES} ${tmp_names} PARENT_SCOPE)
endfunction()
