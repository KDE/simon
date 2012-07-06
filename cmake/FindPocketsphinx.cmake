include(FindPackageHandleStandardArgs)
if(POCKETSPHINX_INCLUDE_DIR AND POCKETSPHINX_LIBRARIES)
set(Pocketsphinx_FIND_QUIETLY TRUE)
endif(POCKETSPHINX_INCLUDE_DIR AND POCKETSPHINX_LIBRARIES)

find_package(Sphinxbase REQUIRED)

FIND_PATH(POCKETSPHINX_INCLUDE_DIR pocketsphinx.h PATHS /usr/include/pocketsphinx/)
find_library(POCKETSPHINX_LIBRARIES pocketsphinx PATHS $ENV{LD_LIBRARY_PATH}/ /usr/lib)
find_package_handle_standard_args(Pocketsphinx DEFAULT_MSG POCKETSPHINX_LIBRARIES POCKETSPHINX_INCLUDE_DIR)
