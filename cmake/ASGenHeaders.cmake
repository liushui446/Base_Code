# platform-specific config file
configure_file("${AS_SOURCE_DIR}/cmake/templates/asconfig.h.in" "${AS_CONFIG_FILE_INCLUDE_DIR}/asconfig.h")
configure_file("${AS_SOURCE_DIR}/cmake/templates/asconfig.h.in" "${AS_CONFIG_FILE_INCLUDE_DIR}/as/asconfig.h")
install(FILES "${AS_CONFIG_FILE_INCLUDE_DIR}/asconfig.h" DESTINATION ${AS_INCLUDE_INSTALL_PATH}/as COMPONENT dev)

# platform-specific config file
as_compiler_optimization_fill_cpu_config()
configure_file("${AS_SOURCE_DIR}/cmake/templates/as_cpu_config.h.in" "${AS_CONFIG_FILE_INCLUDE_DIR}/as_cpu_config.h")

# ----------------------------------------------------------------------------
#  as_modules.hpp based on actual modules list
# ----------------------------------------------------------------------------
set(AS_MODULE_DEFINITIONS_CONFIGMAKE "")

set(AS_MOD_LIST ${AS_MODULES_PUBLIC})
as_list_sort(AS_MOD_LIST)
foreach(m ${AS_MOD_LIST})
  string(TOUPPER "${m}" m)
  set(AS_MODULE_DEFINITIONS_CONFIGMAKE "${AS_MODULE_DEFINITIONS_CONFIGMAKE}#define HAVE_${m}\n")
endforeach()

set(AS_MODULE_DEFINITIONS_CONFIGMAKE "${AS_MODULE_DEFINITIONS_CONFIGMAKE}\n")

#set(AS_MOD_LIST ${AS_MODULES_DISABLED_USER} ${AS_MODULES_DISABLED_AUTO} ${AS_MODULES_DISABLED_FORCE})
#as_list_sort(AS_MOD_LIST)
#foreach(m ${AS_MOD_LIST})
#  string(TOUPPER "${m}" m)
#  set(AS_MODULE_DEFINITIONS_CONFIGMAKE "${AS_MODULE_DEFINITIONS_CONFIGMAKE}#undef HAVE_${m}\n")
#endforeach()

configure_file("${AS_SOURCE_DIR}/cmake/templates/as_modules.hpp.in" "${AS_CONFIG_FILE_INCLUDE_DIR}/as/as_modules.hpp")
install(FILES "${AS_CONFIG_FILE_INCLUDE_DIR}/as/as_modules.hpp" DESTINATION ${AS_INCLUDE_INSTALL_PATH}/as COMPONENT dev)
