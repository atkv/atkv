
function(at_prefix PREFIX IN_LIST)
  foreach(IN_FILE ${IN_LIST})
    list(APPEND OUT_LIST ${PREFIX}${IN_FILE})
  endforeach()
endfunction()

# ------------------
# Concatenate header files from a module
# @param IN_FILE file to be appended
# @param OUT_FILE the big joining file
# @param line_to_remove 
# ------------------
function(at_cat IN_FILE OUT_FILE lines_to_remove)
  file(READ ${IN_FILE} CONTENTS)
  # Remove gpl header
  string(REGEX REPLACE "/\\*[^*]*\\*/(.*)" "\\1" CONTENTS2 "${CONTENTS}")
  foreach(line_to_remove ${lines_to_remove})
    string(REGEX REPLACE ${line_to_remove} "" CONTENTS2 "${CONTENTS2}")
  endforeach()
  file(APPEND ${OUT_FILE} "${CONTENTS2}")
endfunction()