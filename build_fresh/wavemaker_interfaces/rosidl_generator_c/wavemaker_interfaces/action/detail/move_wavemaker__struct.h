// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from wavemaker_interfaces:action/MoveWavemaker.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "wavemaker_interfaces/action/move_wavemaker.h"


#ifndef WAVEMAKER_INTERFACES__ACTION__DETAIL__MOVE_WAVEMAKER__STRUCT_H_
#define WAVEMAKER_INTERFACES__ACTION__DETAIL__MOVE_WAVEMAKER__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'positions'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in action/MoveWavemaker in the package wavemaker_interfaces.
typedef struct wavemaker_interfaces__action__MoveWavemaker_Goal
{
  double amplitude;
  double period;
  /// Absolute wavemaker positions in meters, sampled at this fixed interval.
  /// Used when wavemaker_mode_pregenerated is true.
  rosidl_runtime_c__double__Sequence positions;
  double sample_interval;
} wavemaker_interfaces__action__MoveWavemaker_Goal;

// Struct for a sequence of wavemaker_interfaces__action__MoveWavemaker_Goal.
typedef struct wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence
{
  wavemaker_interfaces__action__MoveWavemaker_Goal * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'message'
#include "rosidl_runtime_c/string.h"

/// Struct defined in action/MoveWavemaker in the package wavemaker_interfaces.
typedef struct wavemaker_interfaces__action__MoveWavemaker_Result
{
  bool success;
  rosidl_runtime_c__String message;
} wavemaker_interfaces__action__MoveWavemaker_Result;

// Struct for a sequence of wavemaker_interfaces__action__MoveWavemaker_Result.
typedef struct wavemaker_interfaces__action__MoveWavemaker_Result__Sequence
{
  wavemaker_interfaces__action__MoveWavemaker_Result * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wavemaker_interfaces__action__MoveWavemaker_Result__Sequence;

// Constants defined in the message

/// Struct defined in action/MoveWavemaker in the package wavemaker_interfaces.
typedef struct wavemaker_interfaces__action__MoveWavemaker_Feedback
{
  double desired_position;
  double actual_position;
  double elapsed_time;
} wavemaker_interfaces__action__MoveWavemaker_Feedback;

// Struct for a sequence of wavemaker_interfaces__action__MoveWavemaker_Feedback.
typedef struct wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence
{
  wavemaker_interfaces__action__MoveWavemaker_Feedback * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
#include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'goal'
#include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"

/// Struct defined in action/MoveWavemaker in the package wavemaker_interfaces.
typedef struct wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
  wavemaker_interfaces__action__MoveWavemaker_Goal goal;
} wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request;

// Struct for a sequence of wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request.
typedef struct wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__struct.h"

/// Struct defined in action/MoveWavemaker in the package wavemaker_interfaces.
typedef struct wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response
{
  bool accepted;
  builtin_interfaces__msg__Time stamp;
} wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response;

// Struct for a sequence of wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response.
typedef struct wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__request__MAX_SIZE = 1
};
// response
enum
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__response__MAX_SIZE = 1
};

/// Struct defined in action/MoveWavemaker in the package wavemaker_interfaces.
typedef struct wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event
{
  service_msgs__msg__ServiceEventInfo info;
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence request;
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence response;
} wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event;

// Struct for a sequence of wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event.
typedef struct wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence
{
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"

/// Struct defined in action/MoveWavemaker in the package wavemaker_interfaces.
typedef struct wavemaker_interfaces__action__MoveWavemaker_GetResult_Request
{
  unique_identifier_msgs__msg__UUID goal_id;
} wavemaker_interfaces__action__MoveWavemaker_GetResult_Request;

// Struct for a sequence of wavemaker_interfaces__action__MoveWavemaker_GetResult_Request.
typedef struct wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'result'
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"

/// Struct defined in action/MoveWavemaker in the package wavemaker_interfaces.
typedef struct wavemaker_interfaces__action__MoveWavemaker_GetResult_Response
{
  int8_t status;
  wavemaker_interfaces__action__MoveWavemaker_Result result;
} wavemaker_interfaces__action__MoveWavemaker_GetResult_Response;

// Struct for a sequence of wavemaker_interfaces__action__MoveWavemaker_GetResult_Response.
typedef struct wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'info'
// already included above
// #include "service_msgs/msg/detail/service_event_info__struct.h"

// constants for array fields with an upper bound
// request
enum
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__request__MAX_SIZE = 1
};
// response
enum
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__response__MAX_SIZE = 1
};

/// Struct defined in action/MoveWavemaker in the package wavemaker_interfaces.
typedef struct wavemaker_interfaces__action__MoveWavemaker_GetResult_Event
{
  service_msgs__msg__ServiceEventInfo info;
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence request;
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence response;
} wavemaker_interfaces__action__MoveWavemaker_GetResult_Event;

// Struct for a sequence of wavemaker_interfaces__action__MoveWavemaker_GetResult_Event.
typedef struct wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence
{
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence;

// Constants defined in the message

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__struct.h"
// Member 'feedback'
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__struct.h"

/// Struct defined in action/MoveWavemaker in the package wavemaker_interfaces.
typedef struct wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage
{
  unique_identifier_msgs__msg__UUID goal_id;
  wavemaker_interfaces__action__MoveWavemaker_Feedback feedback;
} wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage;

// Struct for a sequence of wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage.
typedef struct wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence
{
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // WAVEMAKER_INTERFACES__ACTION__DETAIL__MOVE_WAVEMAKER__STRUCT_H_
