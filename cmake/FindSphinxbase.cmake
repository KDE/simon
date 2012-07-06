include(FindPackageHandleStandardArgs)
if(SPHINXBASE_INCLUDE_DIR AND SPHINXBASE_LIBRARIES)
set(Sphinxbase_FIND_QUIETLY TRUE)
endif(SPHINXBASE_INCLUDE_DIR AND SPHINXBASE_LIBRARIES)

FIND_PATH(SPHINXBASE_INCLUDE_DIR cmd_ln.h PATHS /usr/include/sphinxbase)
find_library(SPHINXBASE_LIBRARIES sphinxad sphinxbase PATHS /usr/lib)
find_package_handle_standard_args(Sphinxbase DEFAULT_MSG SPHINXBASE_LIBRARIES SPHINXBASE_INCLUDE_DIR)
