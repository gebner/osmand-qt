find_path(LZO_INCLUDE_DIR lzo/lzo1x.h)
find_library(LZO_LIBRARY NAMES lzo2 liblzo2)

set(LZO_LIBRARIES ${LZO_LIBRARY})
set(LZO_INCLUDE_DIRS ${LZO_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LZO DEFAULT_MSG LZO_LIBRARY LZO_INCLUDE_DIR)

mark_as_advanced(LZO_INCLUDE_DIR LZO_LIBRARY)
