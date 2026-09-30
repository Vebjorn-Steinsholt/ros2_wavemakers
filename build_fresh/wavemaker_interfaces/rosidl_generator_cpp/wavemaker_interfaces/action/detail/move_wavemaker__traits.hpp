// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from wavemaker_interfaces:action/MoveWavemaker.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "wavemaker_interfaces/action/move_wavemaker.hpp"


#ifndef WAVEMAKER_INTERFACES__ACTION__DETAIL__MOVE_WAVEMAKER__TRAITS_HPP_
#define WAVEMAKER_INTERFACES__ACTION__DETAIL__MOVE_WAVEMAKER__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "wavemaker_interfaces/action/detail/move_wavemaker__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace wavemaker_interfaces
{

namespace action
{

inline void to_flow_style_yaml(
  const MoveWavemaker_Goal & msg,
  std::ostream & out)
{
  out << "{";
  // member: amplitude
  {
    out << "amplitude: ";
    rosidl_generator_traits::value_to_yaml(msg.amplitude, out);
    out << ", ";
  }

  // member: period
  {
    out << "period: ";
    rosidl_generator_traits::value_to_yaml(msg.period, out);
    out << ", ";
  }

  // member: positions
  {
    if (msg.positions.size() == 0) {
      out << "positions: []";
    } else {
      out << "positions: [";
      size_t pending_items = msg.positions.size();
      for (auto item : msg.positions) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: sample_interval
  {
    out << "sample_interval: ";
    rosidl_generator_traits::value_to_yaml(msg.sample_interval, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveWavemaker_Goal & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: amplitude
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "amplitude: ";
    rosidl_generator_traits::value_to_yaml(msg.amplitude, out);
    out << "\n";
  }

  // member: period
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "period: ";
    rosidl_generator_traits::value_to_yaml(msg.period, out);
    out << "\n";
  }

  // member: positions
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.positions.size() == 0) {
      out << "positions: []\n";
    } else {
      out << "positions:\n";
      for (auto item : msg.positions) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: sample_interval
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "sample_interval: ";
    rosidl_generator_traits::value_to_yaml(msg.sample_interval, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveWavemaker_Goal & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace action

}  // namespace wavemaker_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wavemaker_interfaces::action::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wavemaker_interfaces::action::MoveWavemaker_Goal & msg,
  std::ostream & out, size_t indentation = 0)
{
  wavemaker_interfaces::action::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wavemaker_interfaces::action::to_yaml() instead")]]
inline std::string to_yaml(const wavemaker_interfaces::action::MoveWavemaker_Goal & msg)
{
  return wavemaker_interfaces::action::to_yaml(msg);
}

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_Goal>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_Goal";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_Goal>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_Goal";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_Goal>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_Goal>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<wavemaker_interfaces::action::MoveWavemaker_Goal>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace wavemaker_interfaces
{

namespace action
{

inline void to_flow_style_yaml(
  const MoveWavemaker_Result & msg,
  std::ostream & out)
{
  out << "{";
  // member: success
  {
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << ", ";
  }

  // member: message
  {
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveWavemaker_Result & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: success
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "success: ";
    rosidl_generator_traits::value_to_yaml(msg.success, out);
    out << "\n";
  }

  // member: message
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "message: ";
    rosidl_generator_traits::value_to_yaml(msg.message, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveWavemaker_Result & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace action

}  // namespace wavemaker_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wavemaker_interfaces::action::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wavemaker_interfaces::action::MoveWavemaker_Result & msg,
  std::ostream & out, size_t indentation = 0)
{
  wavemaker_interfaces::action::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wavemaker_interfaces::action::to_yaml() instead")]]
inline std::string to_yaml(const wavemaker_interfaces::action::MoveWavemaker_Result & msg)
{
  return wavemaker_interfaces::action::to_yaml(msg);
}

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_Result>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_Result";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_Result>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_Result";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_Result>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_Result>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<wavemaker_interfaces::action::MoveWavemaker_Result>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace wavemaker_interfaces
{

namespace action
{

inline void to_flow_style_yaml(
  const MoveWavemaker_Feedback & msg,
  std::ostream & out)
{
  out << "{";
  // member: desired_position
  {
    out << "desired_position: ";
    rosidl_generator_traits::value_to_yaml(msg.desired_position, out);
    out << ", ";
  }

  // member: actual_position
  {
    out << "actual_position: ";
    rosidl_generator_traits::value_to_yaml(msg.actual_position, out);
    out << ", ";
  }

  // member: elapsed_time
  {
    out << "elapsed_time: ";
    rosidl_generator_traits::value_to_yaml(msg.elapsed_time, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveWavemaker_Feedback & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: desired_position
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "desired_position: ";
    rosidl_generator_traits::value_to_yaml(msg.desired_position, out);
    out << "\n";
  }

  // member: actual_position
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "actual_position: ";
    rosidl_generator_traits::value_to_yaml(msg.actual_position, out);
    out << "\n";
  }

  // member: elapsed_time
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "elapsed_time: ";
    rosidl_generator_traits::value_to_yaml(msg.elapsed_time, out);
    out << "\n";
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveWavemaker_Feedback & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace action

}  // namespace wavemaker_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wavemaker_interfaces::action::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wavemaker_interfaces::action::MoveWavemaker_Feedback & msg,
  std::ostream & out, size_t indentation = 0)
{
  wavemaker_interfaces::action::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wavemaker_interfaces::action::to_yaml() instead")]]
inline std::string to_yaml(const wavemaker_interfaces::action::MoveWavemaker_Feedback & msg)
{
  return wavemaker_interfaces::action::to_yaml(msg);
}

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_Feedback>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_Feedback";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_Feedback>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_Feedback";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_Feedback>
  : std::integral_constant<bool, true> {};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_Feedback>
  : std::integral_constant<bool, true> {};

template<>
struct is_message<wavemaker_interfaces::action::MoveWavemaker_Feedback>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'goal_id'
#include "unique_identifier_msgs/msg/detail/uuid__traits.hpp"
// Member 'goal'
#include "wavemaker_interfaces/action/detail/move_wavemaker__traits.hpp"

namespace wavemaker_interfaces
{

namespace action
{

inline void to_flow_style_yaml(
  const MoveWavemaker_SendGoal_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: goal_id
  {
    out << "goal_id: ";
    to_flow_style_yaml(msg.goal_id, out);
    out << ", ";
  }

  // member: goal
  {
    out << "goal: ";
    to_flow_style_yaml(msg.goal, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveWavemaker_SendGoal_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: goal_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "goal_id:\n";
    to_block_style_yaml(msg.goal_id, out, indentation + 2);
  }

  // member: goal
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "goal:\n";
    to_block_style_yaml(msg.goal, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveWavemaker_SendGoal_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace action

}  // namespace wavemaker_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wavemaker_interfaces::action::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  wavemaker_interfaces::action::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wavemaker_interfaces::action::to_yaml() instead")]]
inline std::string to_yaml(const wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request & msg)
{
  return wavemaker_interfaces::action::to_yaml(msg);
}

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_SendGoal_Request";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request>
  : std::integral_constant<bool, has_fixed_size<unique_identifier_msgs::msg::UUID>::value && has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_Goal>::value> {};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request>
  : std::integral_constant<bool, has_bounded_size<unique_identifier_msgs::msg::UUID>::value && has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_Goal>::value> {};

template<>
struct is_message<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'stamp'
#include "builtin_interfaces/msg/detail/time__traits.hpp"

namespace wavemaker_interfaces
{

namespace action
{

inline void to_flow_style_yaml(
  const MoveWavemaker_SendGoal_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: accepted
  {
    out << "accepted: ";
    rosidl_generator_traits::value_to_yaml(msg.accepted, out);
    out << ", ";
  }

  // member: stamp
  {
    out << "stamp: ";
    to_flow_style_yaml(msg.stamp, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveWavemaker_SendGoal_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: accepted
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "accepted: ";
    rosidl_generator_traits::value_to_yaml(msg.accepted, out);
    out << "\n";
  }

  // member: stamp
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "stamp:\n";
    to_block_style_yaml(msg.stamp, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveWavemaker_SendGoal_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace action

}  // namespace wavemaker_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wavemaker_interfaces::action::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  wavemaker_interfaces::action::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wavemaker_interfaces::action::to_yaml() instead")]]
inline std::string to_yaml(const wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response & msg)
{
  return wavemaker_interfaces::action::to_yaml(msg);
}

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_SendGoal_Response";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response>
  : std::integral_constant<bool, has_fixed_size<builtin_interfaces::msg::Time>::value> {};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response>
  : std::integral_constant<bool, has_bounded_size<builtin_interfaces::msg::Time>::value> {};

template<>
struct is_message<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'info'
#include "service_msgs/msg/detail/service_event_info__traits.hpp"

namespace wavemaker_interfaces
{

namespace action
{

inline void to_flow_style_yaml(
  const MoveWavemaker_SendGoal_Event & msg,
  std::ostream & out)
{
  out << "{";
  // member: info
  {
    out << "info: ";
    to_flow_style_yaml(msg.info, out);
    out << ", ";
  }

  // member: request
  {
    if (msg.request.size() == 0) {
      out << "request: []";
    } else {
      out << "request: [";
      size_t pending_items = msg.request.size();
      for (auto item : msg.request) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: response
  {
    if (msg.response.size() == 0) {
      out << "response: []";
    } else {
      out << "response: [";
      size_t pending_items = msg.response.size();
      for (auto item : msg.response) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveWavemaker_SendGoal_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: info
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "info:\n";
    to_block_style_yaml(msg.info, out, indentation + 2);
  }

  // member: request
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.request.size() == 0) {
      out << "request: []\n";
    } else {
      out << "request:\n";
      for (auto item : msg.request) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: response
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.response.size() == 0) {
      out << "response: []\n";
    } else {
      out << "response:\n";
      for (auto item : msg.response) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveWavemaker_SendGoal_Event & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace action

}  // namespace wavemaker_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wavemaker_interfaces::action::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  wavemaker_interfaces::action::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wavemaker_interfaces::action::to_yaml() instead")]]
inline std::string to_yaml(const wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event & msg)
{
  return wavemaker_interfaces::action::to_yaml(msg);
}

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_SendGoal_Event";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event>
  : std::integral_constant<bool, has_bounded_size<service_msgs::msg::ServiceEventInfo>::value && has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request>::value && has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response>::value> {};

template<>
struct is_message<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_SendGoal>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_SendGoal";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_SendGoal>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_SendGoal";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal>
  : std::integral_constant<
    bool,
    has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request>::value &&
    has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response>::value
  >
{
};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal>
  : std::integral_constant<
    bool,
    has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request>::value &&
    has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response>::value
  >
{
};

template<>
struct is_service<wavemaker_interfaces::action::MoveWavemaker_SendGoal>
  : std::true_type
{
};

template<>
struct is_service_request<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request>
  : std::true_type
{
};

template<>
struct is_service_response<wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__traits.hpp"

namespace wavemaker_interfaces
{

namespace action
{

inline void to_flow_style_yaml(
  const MoveWavemaker_GetResult_Request & msg,
  std::ostream & out)
{
  out << "{";
  // member: goal_id
  {
    out << "goal_id: ";
    to_flow_style_yaml(msg.goal_id, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveWavemaker_GetResult_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: goal_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "goal_id:\n";
    to_block_style_yaml(msg.goal_id, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveWavemaker_GetResult_Request & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace action

}  // namespace wavemaker_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wavemaker_interfaces::action::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wavemaker_interfaces::action::MoveWavemaker_GetResult_Request & msg,
  std::ostream & out, size_t indentation = 0)
{
  wavemaker_interfaces::action::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wavemaker_interfaces::action::to_yaml() instead")]]
inline std::string to_yaml(const wavemaker_interfaces::action::MoveWavemaker_GetResult_Request & msg)
{
  return wavemaker_interfaces::action::to_yaml(msg);
}

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_GetResult_Request>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_GetResult_Request";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_GetResult_Request>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_GetResult_Request";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Request>
  : std::integral_constant<bool, has_fixed_size<unique_identifier_msgs::msg::UUID>::value> {};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Request>
  : std::integral_constant<bool, has_bounded_size<unique_identifier_msgs::msg::UUID>::value> {};

template<>
struct is_message<wavemaker_interfaces::action::MoveWavemaker_GetResult_Request>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'result'
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__traits.hpp"

namespace wavemaker_interfaces
{

namespace action
{

inline void to_flow_style_yaml(
  const MoveWavemaker_GetResult_Response & msg,
  std::ostream & out)
{
  out << "{";
  // member: status
  {
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
    out << ", ";
  }

  // member: result
  {
    out << "result: ";
    to_flow_style_yaml(msg.result, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveWavemaker_GetResult_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: status
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "status: ";
    rosidl_generator_traits::value_to_yaml(msg.status, out);
    out << "\n";
  }

  // member: result
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "result:\n";
    to_block_style_yaml(msg.result, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveWavemaker_GetResult_Response & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace action

}  // namespace wavemaker_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wavemaker_interfaces::action::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wavemaker_interfaces::action::MoveWavemaker_GetResult_Response & msg,
  std::ostream & out, size_t indentation = 0)
{
  wavemaker_interfaces::action::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wavemaker_interfaces::action::to_yaml() instead")]]
inline std::string to_yaml(const wavemaker_interfaces::action::MoveWavemaker_GetResult_Response & msg)
{
  return wavemaker_interfaces::action::to_yaml(msg);
}

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_GetResult_Response>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_GetResult_Response";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_GetResult_Response>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_GetResult_Response";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Response>
  : std::integral_constant<bool, has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_Result>::value> {};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Response>
  : std::integral_constant<bool, has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_Result>::value> {};

template<>
struct is_message<wavemaker_interfaces::action::MoveWavemaker_GetResult_Response>
  : std::true_type {};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'info'
// already included above
// #include "service_msgs/msg/detail/service_event_info__traits.hpp"

namespace wavemaker_interfaces
{

namespace action
{

inline void to_flow_style_yaml(
  const MoveWavemaker_GetResult_Event & msg,
  std::ostream & out)
{
  out << "{";
  // member: info
  {
    out << "info: ";
    to_flow_style_yaml(msg.info, out);
    out << ", ";
  }

  // member: request
  {
    if (msg.request.size() == 0) {
      out << "request: []";
    } else {
      out << "request: [";
      size_t pending_items = msg.request.size();
      for (auto item : msg.request) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: response
  {
    if (msg.response.size() == 0) {
      out << "response: []";
    } else {
      out << "response: [";
      size_t pending_items = msg.response.size();
      for (auto item : msg.response) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveWavemaker_GetResult_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: info
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "info:\n";
    to_block_style_yaml(msg.info, out, indentation + 2);
  }

  // member: request
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.request.size() == 0) {
      out << "request: []\n";
    } else {
      out << "request:\n";
      for (auto item : msg.request) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }

  // member: response
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.response.size() == 0) {
      out << "response: []\n";
    } else {
      out << "response:\n";
      for (auto item : msg.response) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveWavemaker_GetResult_Event & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace action

}  // namespace wavemaker_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wavemaker_interfaces::action::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wavemaker_interfaces::action::MoveWavemaker_GetResult_Event & msg,
  std::ostream & out, size_t indentation = 0)
{
  wavemaker_interfaces::action::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wavemaker_interfaces::action::to_yaml() instead")]]
inline std::string to_yaml(const wavemaker_interfaces::action::MoveWavemaker_GetResult_Event & msg)
{
  return wavemaker_interfaces::action::to_yaml(msg);
}

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_GetResult_Event>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_GetResult_Event";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_GetResult_Event>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_GetResult_Event";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Event>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Event>
  : std::integral_constant<bool, has_bounded_size<service_msgs::msg::ServiceEventInfo>::value && has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Request>::value && has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Response>::value> {};

template<>
struct is_message<wavemaker_interfaces::action::MoveWavemaker_GetResult_Event>
  : std::true_type {};

}  // namespace rosidl_generator_traits

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_GetResult>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_GetResult";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_GetResult>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_GetResult";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_GetResult>
  : std::integral_constant<
    bool,
    has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Request>::value &&
    has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Response>::value
  >
{
};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_GetResult>
  : std::integral_constant<
    bool,
    has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Request>::value &&
    has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_GetResult_Response>::value
  >
{
};

template<>
struct is_service<wavemaker_interfaces::action::MoveWavemaker_GetResult>
  : std::true_type
{
};

template<>
struct is_service_request<wavemaker_interfaces::action::MoveWavemaker_GetResult_Request>
  : std::true_type
{
};

template<>
struct is_service_response<wavemaker_interfaces::action::MoveWavemaker_GetResult_Response>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits

// Include directives for member types
// Member 'goal_id'
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__traits.hpp"
// Member 'feedback'
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__traits.hpp"

namespace wavemaker_interfaces
{

namespace action
{

inline void to_flow_style_yaml(
  const MoveWavemaker_FeedbackMessage & msg,
  std::ostream & out)
{
  out << "{";
  // member: goal_id
  {
    out << "goal_id: ";
    to_flow_style_yaml(msg.goal_id, out);
    out << ", ";
  }

  // member: feedback
  {
    out << "feedback: ";
    to_flow_style_yaml(msg.feedback, out);
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const MoveWavemaker_FeedbackMessage & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: goal_id
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "goal_id:\n";
    to_block_style_yaml(msg.goal_id, out, indentation + 2);
  }

  // member: feedback
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "feedback:\n";
    to_block_style_yaml(msg.feedback, out, indentation + 2);
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const MoveWavemaker_FeedbackMessage & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace action

}  // namespace wavemaker_interfaces

namespace rosidl_generator_traits
{

[[deprecated("use wavemaker_interfaces::action::to_block_style_yaml() instead")]]
inline void to_yaml(
  const wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage & msg,
  std::ostream & out, size_t indentation = 0)
{
  wavemaker_interfaces::action::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use wavemaker_interfaces::action::to_yaml() instead")]]
inline std::string to_yaml(const wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage & msg)
{
  return wavemaker_interfaces::action::to_yaml(msg);
}

template<>
inline const char * data_type<wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage>()
{
  return "wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage";
}

template<>
inline const char * name<wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage>()
{
  return "wavemaker_interfaces/action/MoveWavemaker_FeedbackMessage";
}

template<>
struct has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage>
  : std::integral_constant<bool, has_fixed_size<unique_identifier_msgs::msg::UUID>::value && has_fixed_size<wavemaker_interfaces::action::MoveWavemaker_Feedback>::value> {};

template<>
struct has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage>
  : std::integral_constant<bool, has_bounded_size<unique_identifier_msgs::msg::UUID>::value && has_bounded_size<wavemaker_interfaces::action::MoveWavemaker_Feedback>::value> {};

template<>
struct is_message<wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage>
  : std::true_type {};

}  // namespace rosidl_generator_traits


namespace rosidl_generator_traits
{

template<>
struct is_action<wavemaker_interfaces::action::MoveWavemaker>
  : std::true_type
{
};

template<>
struct is_action_goal<wavemaker_interfaces::action::MoveWavemaker_Goal>
  : std::true_type
{
};

template<>
struct is_action_result<wavemaker_interfaces::action::MoveWavemaker_Result>
  : std::true_type
{
};

template<>
struct is_action_feedback<wavemaker_interfaces::action::MoveWavemaker_Feedback>
  : std::true_type
{
};

}  // namespace rosidl_generator_traits


#endif  // WAVEMAKER_INTERFACES__ACTION__DETAIL__MOVE_WAVEMAKER__TRAITS_HPP_
