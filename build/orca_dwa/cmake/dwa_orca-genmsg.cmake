# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "dwa_orca: 1 messages, 0 services")

set(MSG_I_FLAGS "-Idwa_orca:/home/gao/orca_dwa_ws/src/orca_dwa/msg;-Igeometry_msgs:/opt/ros/noetic/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/noetic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(dwa_orca_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg" NAME_WE)
add_custom_target(_dwa_orca_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "dwa_orca" "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg" "geometry_msgs/Quaternion:geometry_msgs/Pose:geometry_msgs/Point"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(dwa_orca
  "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dwa_orca
)

### Generating Services

### Generating Module File
_generate_module_cpp(dwa_orca
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dwa_orca
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(dwa_orca_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(dwa_orca_generate_messages dwa_orca_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg" NAME_WE)
add_dependencies(dwa_orca_generate_messages_cpp _dwa_orca_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(dwa_orca_gencpp)
add_dependencies(dwa_orca_gencpp dwa_orca_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS dwa_orca_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(dwa_orca
  "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dwa_orca
)

### Generating Services

### Generating Module File
_generate_module_eus(dwa_orca
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dwa_orca
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(dwa_orca_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(dwa_orca_generate_messages dwa_orca_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg" NAME_WE)
add_dependencies(dwa_orca_generate_messages_eus _dwa_orca_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(dwa_orca_geneus)
add_dependencies(dwa_orca_geneus dwa_orca_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS dwa_orca_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(dwa_orca
  "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dwa_orca
)

### Generating Services

### Generating Module File
_generate_module_lisp(dwa_orca
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dwa_orca
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(dwa_orca_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(dwa_orca_generate_messages dwa_orca_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg" NAME_WE)
add_dependencies(dwa_orca_generate_messages_lisp _dwa_orca_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(dwa_orca_genlisp)
add_dependencies(dwa_orca_genlisp dwa_orca_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS dwa_orca_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(dwa_orca
  "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dwa_orca
)

### Generating Services

### Generating Module File
_generate_module_nodejs(dwa_orca
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dwa_orca
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(dwa_orca_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(dwa_orca_generate_messages dwa_orca_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg" NAME_WE)
add_dependencies(dwa_orca_generate_messages_nodejs _dwa_orca_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(dwa_orca_gennodejs)
add_dependencies(dwa_orca_gennodejs dwa_orca_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS dwa_orca_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(dwa_orca
  "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Quaternion.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Pose.msg;/opt/ros/noetic/share/geometry_msgs/cmake/../msg/Point.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dwa_orca
)

### Generating Services

### Generating Module File
_generate_module_py(dwa_orca
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dwa_orca
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(dwa_orca_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(dwa_orca_generate_messages dwa_orca_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/gao/orca_dwa_ws/src/orca_dwa/msg/test.msg" NAME_WE)
add_dependencies(dwa_orca_generate_messages_py _dwa_orca_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(dwa_orca_genpy)
add_dependencies(dwa_orca_genpy dwa_orca_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS dwa_orca_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dwa_orca)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/dwa_orca
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(dwa_orca_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(dwa_orca_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dwa_orca)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/dwa_orca
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(dwa_orca_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(dwa_orca_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dwa_orca)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/dwa_orca
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(dwa_orca_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(dwa_orca_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dwa_orca)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/dwa_orca
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(dwa_orca_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(dwa_orca_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dwa_orca)
  install(CODE "execute_process(COMMAND \"/usr/bin/python3\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dwa_orca\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/dwa_orca
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(dwa_orca_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(dwa_orca_generate_messages_py std_msgs_generate_messages_py)
endif()
