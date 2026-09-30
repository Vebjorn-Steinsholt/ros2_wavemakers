// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from wavemaker_interfaces:action/MoveWavemaker.idl
// generated code does not contain a copyright notice

// IWYU pragma: private, include "wavemaker_interfaces/action/move_wavemaker.hpp"


#ifndef WAVEMAKER_INTERFACES__ACTION__DETAIL__MOVE_WAVEMAKER__BUILDER_HPP_
#define WAVEMAKER_INTERFACES__ACTION__DETAIL__MOVE_WAVEMAKER__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "wavemaker_interfaces/action/detail/move_wavemaker__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace wavemaker_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveWavemaker_Goal_sample_interval
{
public:
  explicit Init_MoveWavemaker_Goal_sample_interval(::wavemaker_interfaces::action::MoveWavemaker_Goal & msg)
  : msg_(msg)
  {}
  ::wavemaker_interfaces::action::MoveWavemaker_Goal sample_interval(::wavemaker_interfaces::action::MoveWavemaker_Goal::_sample_interval_type arg)
  {
    msg_.sample_interval = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_Goal msg_;
};

class Init_MoveWavemaker_Goal_positions
{
public:
  explicit Init_MoveWavemaker_Goal_positions(::wavemaker_interfaces::action::MoveWavemaker_Goal & msg)
  : msg_(msg)
  {}
  Init_MoveWavemaker_Goal_sample_interval positions(::wavemaker_interfaces::action::MoveWavemaker_Goal::_positions_type arg)
  {
    msg_.positions = std::move(arg);
    return Init_MoveWavemaker_Goal_sample_interval(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_Goal msg_;
};

class Init_MoveWavemaker_Goal_period
{
public:
  explicit Init_MoveWavemaker_Goal_period(::wavemaker_interfaces::action::MoveWavemaker_Goal & msg)
  : msg_(msg)
  {}
  Init_MoveWavemaker_Goal_positions period(::wavemaker_interfaces::action::MoveWavemaker_Goal::_period_type arg)
  {
    msg_.period = std::move(arg);
    return Init_MoveWavemaker_Goal_positions(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_Goal msg_;
};

class Init_MoveWavemaker_Goal_amplitude
{
public:
  Init_MoveWavemaker_Goal_amplitude()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveWavemaker_Goal_period amplitude(::wavemaker_interfaces::action::MoveWavemaker_Goal::_amplitude_type arg)
  {
    msg_.amplitude = std::move(arg);
    return Init_MoveWavemaker_Goal_period(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_Goal msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::wavemaker_interfaces::action::MoveWavemaker_Goal>()
{
  return wavemaker_interfaces::action::builder::Init_MoveWavemaker_Goal_amplitude();
}

}  // namespace wavemaker_interfaces


namespace wavemaker_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveWavemaker_Result_message
{
public:
  explicit Init_MoveWavemaker_Result_message(::wavemaker_interfaces::action::MoveWavemaker_Result & msg)
  : msg_(msg)
  {}
  ::wavemaker_interfaces::action::MoveWavemaker_Result message(::wavemaker_interfaces::action::MoveWavemaker_Result::_message_type arg)
  {
    msg_.message = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_Result msg_;
};

class Init_MoveWavemaker_Result_success
{
public:
  Init_MoveWavemaker_Result_success()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveWavemaker_Result_message success(::wavemaker_interfaces::action::MoveWavemaker_Result::_success_type arg)
  {
    msg_.success = std::move(arg);
    return Init_MoveWavemaker_Result_message(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_Result msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::wavemaker_interfaces::action::MoveWavemaker_Result>()
{
  return wavemaker_interfaces::action::builder::Init_MoveWavemaker_Result_success();
}

}  // namespace wavemaker_interfaces


namespace wavemaker_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveWavemaker_Feedback_elapsed_time
{
public:
  explicit Init_MoveWavemaker_Feedback_elapsed_time(::wavemaker_interfaces::action::MoveWavemaker_Feedback & msg)
  : msg_(msg)
  {}
  ::wavemaker_interfaces::action::MoveWavemaker_Feedback elapsed_time(::wavemaker_interfaces::action::MoveWavemaker_Feedback::_elapsed_time_type arg)
  {
    msg_.elapsed_time = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_Feedback msg_;
};

class Init_MoveWavemaker_Feedback_actual_position
{
public:
  explicit Init_MoveWavemaker_Feedback_actual_position(::wavemaker_interfaces::action::MoveWavemaker_Feedback & msg)
  : msg_(msg)
  {}
  Init_MoveWavemaker_Feedback_elapsed_time actual_position(::wavemaker_interfaces::action::MoveWavemaker_Feedback::_actual_position_type arg)
  {
    msg_.actual_position = std::move(arg);
    return Init_MoveWavemaker_Feedback_elapsed_time(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_Feedback msg_;
};

class Init_MoveWavemaker_Feedback_desired_position
{
public:
  Init_MoveWavemaker_Feedback_desired_position()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveWavemaker_Feedback_actual_position desired_position(::wavemaker_interfaces::action::MoveWavemaker_Feedback::_desired_position_type arg)
  {
    msg_.desired_position = std::move(arg);
    return Init_MoveWavemaker_Feedback_actual_position(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_Feedback msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::wavemaker_interfaces::action::MoveWavemaker_Feedback>()
{
  return wavemaker_interfaces::action::builder::Init_MoveWavemaker_Feedback_desired_position();
}

}  // namespace wavemaker_interfaces


namespace wavemaker_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveWavemaker_SendGoal_Request_goal
{
public:
  explicit Init_MoveWavemaker_SendGoal_Request_goal(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request & msg)
  : msg_(msg)
  {}
  ::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request goal(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request::_goal_type arg)
  {
    msg_.goal = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request msg_;
};

class Init_MoveWavemaker_SendGoal_Request_goal_id
{
public:
  Init_MoveWavemaker_SendGoal_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveWavemaker_SendGoal_Request_goal goal_id(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_MoveWavemaker_SendGoal_Request_goal(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Request>()
{
  return wavemaker_interfaces::action::builder::Init_MoveWavemaker_SendGoal_Request_goal_id();
}

}  // namespace wavemaker_interfaces


namespace wavemaker_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveWavemaker_SendGoal_Response_stamp
{
public:
  explicit Init_MoveWavemaker_SendGoal_Response_stamp(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response & msg)
  : msg_(msg)
  {}
  ::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response stamp(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response::_stamp_type arg)
  {
    msg_.stamp = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response msg_;
};

class Init_MoveWavemaker_SendGoal_Response_accepted
{
public:
  Init_MoveWavemaker_SendGoal_Response_accepted()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveWavemaker_SendGoal_Response_stamp accepted(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response::_accepted_type arg)
  {
    msg_.accepted = std::move(arg);
    return Init_MoveWavemaker_SendGoal_Response_stamp(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Response>()
{
  return wavemaker_interfaces::action::builder::Init_MoveWavemaker_SendGoal_Response_accepted();
}

}  // namespace wavemaker_interfaces


namespace wavemaker_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveWavemaker_SendGoal_Event_response
{
public:
  explicit Init_MoveWavemaker_SendGoal_Event_response(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event & msg)
  : msg_(msg)
  {}
  ::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event response(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event msg_;
};

class Init_MoveWavemaker_SendGoal_Event_request
{
public:
  explicit Init_MoveWavemaker_SendGoal_Event_request(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event & msg)
  : msg_(msg)
  {}
  Init_MoveWavemaker_SendGoal_Event_response request(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_MoveWavemaker_SendGoal_Event_response(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event msg_;
};

class Init_MoveWavemaker_SendGoal_Event_info
{
public:
  Init_MoveWavemaker_SendGoal_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveWavemaker_SendGoal_Event_request info(::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_MoveWavemaker_SendGoal_Event_request(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::wavemaker_interfaces::action::MoveWavemaker_SendGoal_Event>()
{
  return wavemaker_interfaces::action::builder::Init_MoveWavemaker_SendGoal_Event_info();
}

}  // namespace wavemaker_interfaces


namespace wavemaker_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveWavemaker_GetResult_Request_goal_id
{
public:
  Init_MoveWavemaker_GetResult_Request_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::wavemaker_interfaces::action::MoveWavemaker_GetResult_Request goal_id(::wavemaker_interfaces::action::MoveWavemaker_GetResult_Request::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_GetResult_Request msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::wavemaker_interfaces::action::MoveWavemaker_GetResult_Request>()
{
  return wavemaker_interfaces::action::builder::Init_MoveWavemaker_GetResult_Request_goal_id();
}

}  // namespace wavemaker_interfaces


namespace wavemaker_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveWavemaker_GetResult_Response_result
{
public:
  explicit Init_MoveWavemaker_GetResult_Response_result(::wavemaker_interfaces::action::MoveWavemaker_GetResult_Response & msg)
  : msg_(msg)
  {}
  ::wavemaker_interfaces::action::MoveWavemaker_GetResult_Response result(::wavemaker_interfaces::action::MoveWavemaker_GetResult_Response::_result_type arg)
  {
    msg_.result = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_GetResult_Response msg_;
};

class Init_MoveWavemaker_GetResult_Response_status
{
public:
  Init_MoveWavemaker_GetResult_Response_status()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveWavemaker_GetResult_Response_result status(::wavemaker_interfaces::action::MoveWavemaker_GetResult_Response::_status_type arg)
  {
    msg_.status = std::move(arg);
    return Init_MoveWavemaker_GetResult_Response_result(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_GetResult_Response msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::wavemaker_interfaces::action::MoveWavemaker_GetResult_Response>()
{
  return wavemaker_interfaces::action::builder::Init_MoveWavemaker_GetResult_Response_status();
}

}  // namespace wavemaker_interfaces


namespace wavemaker_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveWavemaker_GetResult_Event_response
{
public:
  explicit Init_MoveWavemaker_GetResult_Event_response(::wavemaker_interfaces::action::MoveWavemaker_GetResult_Event & msg)
  : msg_(msg)
  {}
  ::wavemaker_interfaces::action::MoveWavemaker_GetResult_Event response(::wavemaker_interfaces::action::MoveWavemaker_GetResult_Event::_response_type arg)
  {
    msg_.response = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_GetResult_Event msg_;
};

class Init_MoveWavemaker_GetResult_Event_request
{
public:
  explicit Init_MoveWavemaker_GetResult_Event_request(::wavemaker_interfaces::action::MoveWavemaker_GetResult_Event & msg)
  : msg_(msg)
  {}
  Init_MoveWavemaker_GetResult_Event_response request(::wavemaker_interfaces::action::MoveWavemaker_GetResult_Event::_request_type arg)
  {
    msg_.request = std::move(arg);
    return Init_MoveWavemaker_GetResult_Event_response(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_GetResult_Event msg_;
};

class Init_MoveWavemaker_GetResult_Event_info
{
public:
  Init_MoveWavemaker_GetResult_Event_info()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveWavemaker_GetResult_Event_request info(::wavemaker_interfaces::action::MoveWavemaker_GetResult_Event::_info_type arg)
  {
    msg_.info = std::move(arg);
    return Init_MoveWavemaker_GetResult_Event_request(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_GetResult_Event msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::wavemaker_interfaces::action::MoveWavemaker_GetResult_Event>()
{
  return wavemaker_interfaces::action::builder::Init_MoveWavemaker_GetResult_Event_info();
}

}  // namespace wavemaker_interfaces


namespace wavemaker_interfaces
{

namespace action
{

namespace builder
{

class Init_MoveWavemaker_FeedbackMessage_feedback
{
public:
  explicit Init_MoveWavemaker_FeedbackMessage_feedback(::wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage & msg)
  : msg_(msg)
  {}
  ::wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage feedback(::wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage::_feedback_type arg)
  {
    msg_.feedback = std::move(arg);
    return std::move(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage msg_;
};

class Init_MoveWavemaker_FeedbackMessage_goal_id
{
public:
  Init_MoveWavemaker_FeedbackMessage_goal_id()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_MoveWavemaker_FeedbackMessage_feedback goal_id(::wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage::_goal_id_type arg)
  {
    msg_.goal_id = std::move(arg);
    return Init_MoveWavemaker_FeedbackMessage_feedback(msg_);
  }

private:
  ::wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage msg_;
};

}  // namespace builder

}  // namespace action

template<typename MessageType>
auto build();

template<>
inline
auto build<::wavemaker_interfaces::action::MoveWavemaker_FeedbackMessage>()
{
  return wavemaker_interfaces::action::builder::Init_MoveWavemaker_FeedbackMessage_goal_id();
}

}  // namespace wavemaker_interfaces

#endif  // WAVEMAKER_INTERFACES__ACTION__DETAIL__MOVE_WAVEMAKER__BUILDER_HPP_
