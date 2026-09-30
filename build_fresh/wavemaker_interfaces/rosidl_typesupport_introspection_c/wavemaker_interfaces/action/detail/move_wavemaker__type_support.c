// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from wavemaker_interfaces:action/MoveWavemaker.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
#include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"
#include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"


// Include directives for member types
// Member `positions`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  wavemaker_interfaces__action__MoveWavemaker_Goal__init(message_memory);
}

void wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_fini_function(void * message_memory)
{
  wavemaker_interfaces__action__MoveWavemaker_Goal__fini(message_memory);
}

size_t wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__size_function__MoveWavemaker_Goal__positions(
  const void * untyped_member)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return member->size;
}

const void * wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_Goal__positions(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__double__Sequence * member =
    (const rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void * wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_Goal__positions(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  return &member->data[index];
}

void wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__fetch_function__MoveWavemaker_Goal__positions(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const double * item =
    ((const double *)
    wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_Goal__positions(untyped_member, index));
  double * value =
    (double *)(untyped_value);
  *value = *item;
}

void wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__assign_function__MoveWavemaker_Goal__positions(
  void * untyped_member, size_t index, const void * untyped_value)
{
  double * item =
    ((double *)
    wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_Goal__positions(untyped_member, index));
  const double * value =
    (const double *)(untyped_value);
  *item = *value;
}

bool wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__resize_function__MoveWavemaker_Goal__positions(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__double__Sequence * member =
    (rosidl_runtime_c__double__Sequence *)(untyped_member);
  rosidl_runtime_c__double__Sequence__fini(member);
  return rosidl_runtime_c__double__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_message_member_array[4] = {
  {
    "amplitude",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_Goal, amplitude),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "period",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_Goal, period),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "positions",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_Goal, positions),  // bytes offset in struct
    NULL,  // default value
    wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__size_function__MoveWavemaker_Goal__positions,  // size() function pointer
    wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_Goal__positions,  // get_const(index) function pointer
    wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_Goal__positions,  // get(index) function pointer
    wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__fetch_function__MoveWavemaker_Goal__positions,  // fetch(index, &value) function pointer
    wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__assign_function__MoveWavemaker_Goal__positions,  // assign(index, value) function pointer
    wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__resize_function__MoveWavemaker_Goal__positions  // resize(index) function pointer
  },
  {
    "sample_interval",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_Goal, sample_interval),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_message_members = {
  "wavemaker_interfaces__action",  // message namespace
  "MoveWavemaker_Goal",  // message name
  4,  // number of fields
  sizeof(wavemaker_interfaces__action__MoveWavemaker_Goal),
  false,  // has_any_key_member_
  wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_message_member_array,  // message members
  wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_init_function,  // function to initialize message memory (memory has to be allocated)
  wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_message_type_support_handle = {
  0,
  &wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_message_members,
  get_message_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_Goal__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_Goal__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_Goal__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_Goal)() {
  if (!wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_message_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &wavemaker_interfaces__action__MoveWavemaker_Goal__rosidl_typesupport_introspection_c__MoveWavemaker_Goal_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"


// Include directives for member types
// Member `message`
#include "rosidl_runtime_c/string_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  wavemaker_interfaces__action__MoveWavemaker_Result__init(message_memory);
}

void wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_fini_function(void * message_memory)
{
  wavemaker_interfaces__action__MoveWavemaker_Result__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_message_member_array[2] = {
  {
    "success",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_Result, success),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "message",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_STRING,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_Result, message),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_message_members = {
  "wavemaker_interfaces__action",  // message namespace
  "MoveWavemaker_Result",  // message name
  2,  // number of fields
  sizeof(wavemaker_interfaces__action__MoveWavemaker_Result),
  false,  // has_any_key_member_
  wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_message_member_array,  // message members
  wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_init_function,  // function to initialize message memory (memory has to be allocated)
  wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_message_type_support_handle = {
  0,
  &wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_message_members,
  get_message_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_Result__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_Result__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_Result__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_Result)() {
  if (!wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_message_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &wavemaker_interfaces__action__MoveWavemaker_Result__rosidl_typesupport_introspection_c__MoveWavemaker_Result_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"


#ifdef __cplusplus
extern "C"
{
#endif

void wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  wavemaker_interfaces__action__MoveWavemaker_Feedback__init(message_memory);
}

void wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_fini_function(void * message_memory)
{
  wavemaker_interfaces__action__MoveWavemaker_Feedback__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_message_member_array[3] = {
  {
    "desired_position",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_Feedback, desired_position),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "actual_position",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_Feedback, actual_position),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "elapsed_time",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_DOUBLE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_Feedback, elapsed_time),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_message_members = {
  "wavemaker_interfaces__action",  // message namespace
  "MoveWavemaker_Feedback",  // message name
  3,  // number of fields
  sizeof(wavemaker_interfaces__action__MoveWavemaker_Feedback),
  false,  // has_any_key_member_
  wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_message_member_array,  // message members
  wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_init_function,  // function to initialize message memory (memory has to be allocated)
  wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_message_type_support_handle = {
  0,
  &wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_message_members,
  get_message_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_Feedback__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_Feedback__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_Feedback__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_Feedback)() {
  if (!wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_message_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &wavemaker_interfaces__action__MoveWavemaker_Feedback__rosidl_typesupport_introspection_c__MoveWavemaker_Feedback_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"


// Include directives for member types
// Member `goal_id`
#include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
#include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"
// Member `goal`
#include "wavemaker_interfaces/action/move_wavemaker.h"
// Member `goal`
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__init(message_memory);
}

void wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_fini_function(void * message_memory)
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_member_array[2] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "goal",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request, goal),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_members = {
  "wavemaker_interfaces__action",  // message namespace
  "MoveWavemaker_SendGoal_Request",  // message name
  2,  // number of fields
  sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request),
  false,  // has_any_key_member_
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_member_array,  // message members
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_type_support_handle = {
  0,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_members,
  get_message_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Request)() {
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_Goal)();
  if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/time.h"
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__init(message_memory);
}

void wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_fini_function(void * message_memory)
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_member_array[2] = {
  {
    "accepted",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_BOOLEAN,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response, accepted),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "stamp",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response, stamp),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_members = {
  "wavemaker_interfaces__action",  // message namespace
  "MoveWavemaker_SendGoal_Response",  // message name
  2,  // number of fields
  sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response),
  false,  // has_any_key_member_
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_member_array,  // message members
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_type_support_handle = {
  0,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_members,
  get_message_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Response)() {
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, builtin_interfaces, msg, Time)();
  if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"


// Include directives for member types
// Member `info`
#include "service_msgs/msg/service_event_info.h"
// Member `info`
#include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
// already included above
// #include "wavemaker_interfaces/action/move_wavemaker.h"
// Member `request`
// Member `response`
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__init(message_memory);
}

void wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_fini_function(void * message_memory)
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__fini(message_memory);
}

size_t wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__MoveWavemaker_SendGoal_Event__request(
  const void * untyped_member)
{
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * member =
    (const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_SendGoal_Event__request(
  const void * untyped_member, size_t index)
{
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * member =
    (const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_SendGoal_Event__request(
  void * untyped_member, size_t index)
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * member =
    (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__MoveWavemaker_SendGoal_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * item =
    ((const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request *)
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_SendGoal_Event__request(untyped_member, index));
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * value =
    (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request *)(untyped_value);
  *value = *item;
}

void wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__MoveWavemaker_SendGoal_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * item =
    ((wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request *)
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_SendGoal_Event__request(untyped_member, index));
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * value =
    (const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request *)(untyped_value);
  *item = *value;
}

bool wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__MoveWavemaker_SendGoal_Event__request(
  void * untyped_member, size_t size)
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * member =
    (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence *)(untyped_member);
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__fini(member);
  return wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__init(member, size);
}

size_t wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__MoveWavemaker_SendGoal_Event__response(
  const void * untyped_member)
{
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * member =
    (const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_SendGoal_Event__response(
  const void * untyped_member, size_t index)
{
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * member =
    (const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_SendGoal_Event__response(
  void * untyped_member, size_t index)
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * member =
    (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__MoveWavemaker_SendGoal_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * item =
    ((const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response *)
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_SendGoal_Event__response(untyped_member, index));
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * value =
    (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response *)(untyped_value);
  *value = *item;
}

void wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__MoveWavemaker_SendGoal_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * item =
    ((wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response *)
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_SendGoal_Event__response(untyped_member, index));
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * value =
    (const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response *)(untyped_value);
  *item = *value;
}

bool wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__MoveWavemaker_SendGoal_Event__response(
  void * untyped_member, size_t size)
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * member =
    (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence *)(untyped_member);
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__fini(member);
  return wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event, info),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "request",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event, request),  // bytes offset in struct
    NULL,  // default value
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__MoveWavemaker_SendGoal_Event__request,  // size() function pointer
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_SendGoal_Event__request,  // get_const(index) function pointer
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_SendGoal_Event__request,  // get(index) function pointer
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__MoveWavemaker_SendGoal_Event__request,  // fetch(index, &value) function pointer
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__MoveWavemaker_SendGoal_Event__request,  // assign(index, value) function pointer
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__MoveWavemaker_SendGoal_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event, response),  // bytes offset in struct
    NULL,  // default value
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__size_function__MoveWavemaker_SendGoal_Event__response,  // size() function pointer
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_SendGoal_Event__response,  // get_const(index) function pointer
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_SendGoal_Event__response,  // get(index) function pointer
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__fetch_function__MoveWavemaker_SendGoal_Event__response,  // fetch(index, &value) function pointer
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__assign_function__MoveWavemaker_SendGoal_Event__response,  // assign(index, value) function pointer
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__resize_function__MoveWavemaker_SendGoal_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_members = {
  "wavemaker_interfaces__action",  // message namespace
  "MoveWavemaker_SendGoal_Event",  // message name
  3,  // number of fields
  sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event),
  false,  // has_any_key_member_
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_member_array,  // message members
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_type_support_handle = {
  0,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_members,
  get_message_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Event)() {
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Request)();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Response)();
  if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

#include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_service_members = {
  "wavemaker_interfaces__action",  // service namespace
  "MoveWavemaker_SendGoal",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_type_support_handle,
  NULL,  // response message
  // wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_type_support_handle
  NULL  // event_message
  // wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_type_support_handle
};


static rosidl_service_type_support_t wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_service_type_support_handle = {
  0,
  &wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_service_members,
  get_service_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Request_message_type_support_handle,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Response_message_type_support_handle,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    wavemaker_interfaces,
    action,
    MoveWavemaker_SendGoal
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    wavemaker_interfaces,
    action,
    MoveWavemaker_SendGoal
  ),
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_SendGoal__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal)(void) {
  if (!wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_service_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_SendGoal_Event)()->data;
  }

  return &wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_SendGoal_service_type_support_handle;
}

// already included above
// #include <stddef.h>
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__init(message_memory);
}

void wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_fini_function(void * message_memory)
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_member_array[1] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_members = {
  "wavemaker_interfaces__action",  // message namespace
  "MoveWavemaker_GetResult_Request",  // message name
  1,  // number of fields
  sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request),
  false,  // has_any_key_member_
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_member_array,  // message members
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_init_function,  // function to initialize message memory (memory has to be allocated)
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_type_support_handle = {
  0,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_members,
  get_message_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Request)() {
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"


// Include directives for member types
// Member `result`
// already included above
// #include "wavemaker_interfaces/action/move_wavemaker.h"
// Member `result`
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__init(message_memory);
}

void wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_fini_function(void * message_memory)
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_member_array[2] = {
  {
    "status",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_INT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response, status),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "result",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response, result),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_members = {
  "wavemaker_interfaces__action",  // message namespace
  "MoveWavemaker_GetResult_Response",  // message name
  2,  // number of fields
  sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response),
  false,  // has_any_key_member_
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_member_array,  // message members
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_init_function,  // function to initialize message memory (memory has to be allocated)
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_type_support_handle = {
  0,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_members,
  get_message_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Response)() {
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_Result)();
  if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include <stddef.h>
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"


// Include directives for member types
// Member `info`
// already included above
// #include "service_msgs/msg/service_event_info.h"
// Member `info`
// already included above
// #include "service_msgs/msg/detail/service_event_info__rosidl_typesupport_introspection_c.h"
// Member `request`
// Member `response`
// already included above
// #include "wavemaker_interfaces/action/move_wavemaker.h"
// Member `request`
// Member `response`
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__init(message_memory);
}

void wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_fini_function(void * message_memory)
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__fini(message_memory);
}

size_t wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__size_function__MoveWavemaker_GetResult_Event__request(
  const void * untyped_member)
{
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * member =
    (const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence *)(untyped_member);
  return member->size;
}

const void * wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_GetResult_Event__request(
  const void * untyped_member, size_t index)
{
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * member =
    (const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void * wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_GetResult_Event__request(
  void * untyped_member, size_t index)
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * member =
    (wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence *)(untyped_member);
  return &member->data[index];
}

void wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__MoveWavemaker_GetResult_Event__request(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * item =
    ((const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request *)
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_GetResult_Event__request(untyped_member, index));
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * value =
    (wavemaker_interfaces__action__MoveWavemaker_GetResult_Request *)(untyped_value);
  *value = *item;
}

void wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__MoveWavemaker_GetResult_Event__request(
  void * untyped_member, size_t index, const void * untyped_value)
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * item =
    ((wavemaker_interfaces__action__MoveWavemaker_GetResult_Request *)
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_GetResult_Event__request(untyped_member, index));
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * value =
    (const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request *)(untyped_value);
  *item = *value;
}

bool wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__MoveWavemaker_GetResult_Event__request(
  void * untyped_member, size_t size)
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * member =
    (wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence *)(untyped_member);
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__fini(member);
  return wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__init(member, size);
}

size_t wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__size_function__MoveWavemaker_GetResult_Event__response(
  const void * untyped_member)
{
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * member =
    (const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence *)(untyped_member);
  return member->size;
}

const void * wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_GetResult_Event__response(
  const void * untyped_member, size_t index)
{
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * member =
    (const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void * wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_GetResult_Event__response(
  void * untyped_member, size_t index)
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * member =
    (wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence *)(untyped_member);
  return &member->data[index];
}

void wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__MoveWavemaker_GetResult_Event__response(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * item =
    ((const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response *)
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_GetResult_Event__response(untyped_member, index));
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * value =
    (wavemaker_interfaces__action__MoveWavemaker_GetResult_Response *)(untyped_value);
  *value = *item;
}

void wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__MoveWavemaker_GetResult_Event__response(
  void * untyped_member, size_t index, const void * untyped_value)
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * item =
    ((wavemaker_interfaces__action__MoveWavemaker_GetResult_Response *)
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_GetResult_Event__response(untyped_member, index));
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * value =
    (const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response *)(untyped_value);
  *item = *value;
}

bool wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__MoveWavemaker_GetResult_Event__response(
  void * untyped_member, size_t size)
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * member =
    (wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence *)(untyped_member);
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__fini(member);
  return wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_member_array[3] = {
  {
    "info",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event, info),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "request",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event, request),  // bytes offset in struct
    NULL,  // default value
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__size_function__MoveWavemaker_GetResult_Event__request,  // size() function pointer
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_GetResult_Event__request,  // get_const(index) function pointer
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_GetResult_Event__request,  // get(index) function pointer
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__MoveWavemaker_GetResult_Event__request,  // fetch(index, &value) function pointer
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__MoveWavemaker_GetResult_Event__request,  // assign(index, value) function pointer
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__MoveWavemaker_GetResult_Event__request  // resize(index) function pointer
  },
  {
    "response",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    true,  // is array
    1,  // array size
    true,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event, response),  // bytes offset in struct
    NULL,  // default value
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__size_function__MoveWavemaker_GetResult_Event__response,  // size() function pointer
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_const_function__MoveWavemaker_GetResult_Event__response,  // get_const(index) function pointer
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__get_function__MoveWavemaker_GetResult_Event__response,  // get(index) function pointer
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__fetch_function__MoveWavemaker_GetResult_Event__response,  // fetch(index, &value) function pointer
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__assign_function__MoveWavemaker_GetResult_Event__response,  // assign(index, value) function pointer
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__resize_function__MoveWavemaker_GetResult_Event__response  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_members = {
  "wavemaker_interfaces__action",  // message namespace
  "MoveWavemaker_GetResult_Event",  // message name
  3,  // number of fields
  sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event),
  false,  // has_any_key_member_
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_member_array,  // message members
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_init_function,  // function to initialize message memory (memory has to be allocated)
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_type_support_handle = {
  0,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_members,
  get_message_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Event)() {
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, service_msgs, msg, ServiceEventInfo)();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Request)();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_member_array[2].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Response)();
  if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif

// already included above
// #include "rosidl_runtime_c/service_type_support_struct.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/service_introspection.h"

// this is intentionally not const to allow initialization later to prevent an initialization race
static rosidl_typesupport_introspection_c__ServiceMembers wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_service_members = {
  "wavemaker_interfaces__action",  // service namespace
  "MoveWavemaker_GetResult",  // service name
  // the following fields are initialized below on first access
  NULL,  // request message
  // wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_type_support_handle,
  NULL,  // response message
  // wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_type_support_handle
  NULL  // event_message
  // wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_type_support_handle
};


static rosidl_service_type_support_t wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_service_type_support_handle = {
  0,
  &wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_service_members,
  get_service_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Request_message_type_support_handle,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Response_message_type_support_handle,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_Event_message_type_support_handle,
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_CREATE_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    wavemaker_interfaces,
    action,
    MoveWavemaker_GetResult
  ),
  ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_DESTROY_EVENT_MESSAGE_SYMBOL_NAME(
    rosidl_typesupport_c,
    wavemaker_interfaces,
    action,
    MoveWavemaker_GetResult
  ),
  &wavemaker_interfaces__action__MoveWavemaker_GetResult__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_GetResult__get_type_description_sources,
};

// Forward declaration of message type support functions for service members
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Request)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Response)(void);

const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Event)(void);

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_service_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__SERVICE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult)(void) {
  if (!wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_service_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_service_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  rosidl_typesupport_introspection_c__ServiceMembers * service_members =
    (rosidl_typesupport_introspection_c__ServiceMembers *)wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_service_type_support_handle.data;

  if (!service_members->request_members_) {
    service_members->request_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Request)()->data;
  }
  if (!service_members->response_members_) {
    service_members->response_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Response)()->data;
  }
  if (!service_members->event_members_) {
    service_members->event_members_ =
      (const rosidl_typesupport_introspection_c__MessageMembers *)
      ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_GetResult_Event)()->data;
  }

  return &wavemaker_interfaces__action__detail__move_wavemaker__rosidl_typesupport_introspection_c__MoveWavemaker_GetResult_service_type_support_handle;
}

// already included above
// #include <stddef.h>
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"
// already included above
// #include "wavemaker_interfaces/msg/rosidl_typesupport_introspection_c__visibility_control.h"
// already included above
// #include "rosidl_typesupport_introspection_c/field_types.h"
// already included above
// #include "rosidl_typesupport_introspection_c/identifier.h"
// already included above
// #include "rosidl_typesupport_introspection_c/message_introspection.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/uuid.h"
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__rosidl_typesupport_introspection_c.h"
// Member `feedback`
// already included above
// #include "wavemaker_interfaces/action/move_wavemaker.h"
// Member `feedback`
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__init(message_memory);
}

void wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_fini_function(void * message_memory)
{
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__fini(message_memory);
}

static rosidl_typesupport_introspection_c__MessageMember wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_message_member_array[2] = {
  {
    "goal_id",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage, goal_id),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "feedback",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is key
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage, feedback),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_message_members = {
  "wavemaker_interfaces__action",  // message namespace
  "MoveWavemaker_FeedbackMessage",  // message name
  2,  // number of fields
  sizeof(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage),
  false,  // has_any_key_member_
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_message_member_array,  // message members
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_init_function,  // function to initialize message memory (memory has to be allocated)
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_message_type_support_handle = {
  0,
  &wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_message_members,
  get_message_typesupport_handle_function,
  &wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__get_type_hash,
  &wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__get_type_description,
  &wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__get_type_description_sources,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_wavemaker_interfaces
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_FeedbackMessage)() {
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, unique_identifier_msgs, msg, UUID)();
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, wavemaker_interfaces, action, MoveWavemaker_Feedback)();
  if (!wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_message_type_support_handle.typesupport_identifier) {
    wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__rosidl_typesupport_introspection_c__MoveWavemaker_FeedbackMessage_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
