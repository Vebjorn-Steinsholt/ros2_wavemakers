
#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};



// Corresponds to wavemaker_interfaces__action__MoveWavemaker_Goal

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_Goal {

    // This member is not documented.
    #[allow(missing_docs)]
    pub amplitude: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub period: f64,

    /// Absolute wavemaker positions in meters, sampled at this fixed interval.
    /// Used when wavemaker_mode_pregenerated is true.
    pub positions: Vec<f64>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub sample_interval: f64,

}



impl Default for MoveWavemaker_Goal {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::action::rmw::MoveWavemaker_Goal::default())
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_Goal {
  type RmwMsg = super::action::rmw::MoveWavemaker_Goal;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        amplitude: msg.amplitude,
        period: msg.period,
        positions: msg.positions.as_slice().into(),
        sample_interval: msg.sample_interval,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      amplitude: msg.amplitude,
      period: msg.period,
        positions: msg.positions.as_slice().into(),
      sample_interval: msg.sample_interval,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      amplitude: msg.amplitude,
      period: msg.period,
      positions: msg.positions.into(),
      sample_interval: msg.sample_interval,
    }
  }
}


// Corresponds to wavemaker_interfaces__action__MoveWavemaker_Result

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_Result {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub message: std::string::String,

}



impl Default for MoveWavemaker_Result {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::action::rmw::MoveWavemaker_Result::default())
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_Result {
  type RmwMsg = super::action::rmw::MoveWavemaker_Result;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        success: msg.success,
        message: msg.message.as_str().into(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      success: msg.success,
        message: msg.message.as_str().into(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      success: msg.success,
      message: msg.message.to_string(),
    }
  }
}


// Corresponds to wavemaker_interfaces__action__MoveWavemaker_Feedback

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_Feedback {

    // This member is not documented.
    #[allow(missing_docs)]
    pub desired_position: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub actual_position: f64,


    // This member is not documented.
    #[allow(missing_docs)]
    pub elapsed_time: f64,

}



impl Default for MoveWavemaker_Feedback {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::action::rmw::MoveWavemaker_Feedback::default())
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_Feedback {
  type RmwMsg = super::action::rmw::MoveWavemaker_Feedback;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        desired_position: msg.desired_position,
        actual_position: msg.actual_position,
        elapsed_time: msg.elapsed_time,
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      desired_position: msg.desired_position,
      actual_position: msg.actual_position,
      elapsed_time: msg.elapsed_time,
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      desired_position: msg.desired_position,
      actual_position: msg.actual_position,
      elapsed_time: msg.elapsed_time,
    }
  }
}


// Corresponds to wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_FeedbackMessage {

    // This member is not documented.
    #[allow(missing_docs)]
    pub goal_id: unique_identifier_msgs::msg::UUID,


    // This member is not documented.
    #[allow(missing_docs)]
    pub feedback: super::action::MoveWavemaker_Feedback,

}



impl Default for MoveWavemaker_FeedbackMessage {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::action::rmw::MoveWavemaker_FeedbackMessage::default())
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_FeedbackMessage {
  type RmwMsg = super::action::rmw::MoveWavemaker_FeedbackMessage;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        goal_id: unique_identifier_msgs::msg::UUID::into_rmw_message(std::borrow::Cow::Owned(msg.goal_id)).into_owned(),
        feedback: super::action::MoveWavemaker_Feedback::into_rmw_message(std::borrow::Cow::Owned(msg.feedback)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        goal_id: unique_identifier_msgs::msg::UUID::into_rmw_message(std::borrow::Cow::Borrowed(&msg.goal_id)).into_owned(),
        feedback: super::action::MoveWavemaker_Feedback::into_rmw_message(std::borrow::Cow::Borrowed(&msg.feedback)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      goal_id: unique_identifier_msgs::msg::UUID::from_rmw_message(msg.goal_id),
      feedback: super::action::MoveWavemaker_Feedback::from_rmw_message(msg.feedback),
    }
  }
}






// Corresponds to wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_SendGoal_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub goal_id: unique_identifier_msgs::msg::UUID,


    // This member is not documented.
    #[allow(missing_docs)]
    pub goal: super::action::MoveWavemaker_Goal,

}



impl Default for MoveWavemaker_SendGoal_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::action::rmw::MoveWavemaker_SendGoal_Request::default())
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_SendGoal_Request {
  type RmwMsg = super::action::rmw::MoveWavemaker_SendGoal_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        goal_id: unique_identifier_msgs::msg::UUID::into_rmw_message(std::borrow::Cow::Owned(msg.goal_id)).into_owned(),
        goal: super::action::MoveWavemaker_Goal::into_rmw_message(std::borrow::Cow::Owned(msg.goal)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        goal_id: unique_identifier_msgs::msg::UUID::into_rmw_message(std::borrow::Cow::Borrowed(&msg.goal_id)).into_owned(),
        goal: super::action::MoveWavemaker_Goal::into_rmw_message(std::borrow::Cow::Borrowed(&msg.goal)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      goal_id: unique_identifier_msgs::msg::UUID::from_rmw_message(msg.goal_id),
      goal: super::action::MoveWavemaker_Goal::from_rmw_message(msg.goal),
    }
  }
}


// Corresponds to wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_SendGoal_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub accepted: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub stamp: builtin_interfaces::msg::Time,

}



impl Default for MoveWavemaker_SendGoal_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::action::rmw::MoveWavemaker_SendGoal_Response::default())
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_SendGoal_Response {
  type RmwMsg = super::action::rmw::MoveWavemaker_SendGoal_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        accepted: msg.accepted,
        stamp: builtin_interfaces::msg::Time::into_rmw_message(std::borrow::Cow::Owned(msg.stamp)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      accepted: msg.accepted,
        stamp: builtin_interfaces::msg::Time::into_rmw_message(std::borrow::Cow::Borrowed(&msg.stamp)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      accepted: msg.accepted,
      stamp: builtin_interfaces::msg::Time::from_rmw_message(msg.stamp),
    }
  }
}


// Corresponds to wavemaker_interfaces__action__MoveWavemaker_GetResult_Request

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_GetResult_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub goal_id: unique_identifier_msgs::msg::UUID,

}



impl Default for MoveWavemaker_GetResult_Request {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::action::rmw::MoveWavemaker_GetResult_Request::default())
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_GetResult_Request {
  type RmwMsg = super::action::rmw::MoveWavemaker_GetResult_Request;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        goal_id: unique_identifier_msgs::msg::UUID::into_rmw_message(std::borrow::Cow::Owned(msg.goal_id)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        goal_id: unique_identifier_msgs::msg::UUID::into_rmw_message(std::borrow::Cow::Borrowed(&msg.goal_id)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      goal_id: unique_identifier_msgs::msg::UUID::from_rmw_message(msg.goal_id),
    }
  }
}


// Corresponds to wavemaker_interfaces__action__MoveWavemaker_GetResult_Response

// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_GetResult_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub status: i8,


    // This member is not documented.
    #[allow(missing_docs)]
    pub result: super::action::MoveWavemaker_Result,

}



impl Default for MoveWavemaker_GetResult_Response {
  fn default() -> Self {
    <Self as rosidl_runtime_rs::Message>::from_rmw_message(super::action::rmw::MoveWavemaker_GetResult_Response::default())
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_GetResult_Response {
  type RmwMsg = super::action::rmw::MoveWavemaker_GetResult_Response;

  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> {
    match msg_cow {
      std::borrow::Cow::Owned(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
        status: msg.status,
        result: super::action::MoveWavemaker_Result::into_rmw_message(std::borrow::Cow::Owned(msg.result)).into_owned(),
      }),
      std::borrow::Cow::Borrowed(msg) => std::borrow::Cow::Owned(Self::RmwMsg {
      status: msg.status,
        result: super::action::MoveWavemaker_Result::into_rmw_message(std::borrow::Cow::Borrowed(&msg.result)).into_owned(),
      })
    }
  }

  fn from_rmw_message(msg: Self::RmwMsg) -> Self {
    Self {
      status: msg.status,
      result: super::action::MoveWavemaker_Result::from_rmw_message(msg.result),
    }
  }
}






#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_SendGoal() -> *const std::ffi::c_void;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker_SendGoal
#[allow(missing_docs, non_camel_case_types)]
pub struct MoveWavemaker_SendGoal;

impl rosidl_runtime_rs::Service for MoveWavemaker_SendGoal {
    type Request = MoveWavemaker_SendGoal_Request;
    type Response = MoveWavemaker_SendGoal_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_SendGoal() }
    }
}




#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_service_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_GetResult() -> *const std::ffi::c_void;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker_GetResult
#[allow(missing_docs, non_camel_case_types)]
pub struct MoveWavemaker_GetResult;

impl rosidl_runtime_rs::Service for MoveWavemaker_GetResult {
    type Request = MoveWavemaker_GetResult_Request;
    type Response = MoveWavemaker_GetResult_Response;

    fn get_type_support() -> *const std::ffi::c_void {
        // SAFETY: No preconditions for this function.
        unsafe { rosidl_typesupport_c__get_service_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_GetResult() }
    }
}






#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_action_type_support_handle__wavemaker_interfaces__action__MoveWavemaker() -> *const std::ffi::c_void;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker
#[allow(missing_docs, non_camel_case_types)]
pub struct MoveWavemaker;

impl rosidl_runtime_rs::Action for MoveWavemaker {
  // --- Associated types for client library users ---
  /// The goal message defined in the action definition.
  type Goal = MoveWavemaker_Goal;

  /// The result message defined in the action definition.
  type Result = MoveWavemaker_Result;

  /// The feedback message defined in the action definition.
  type Feedback = MoveWavemaker_Feedback;

  // --- Associated types for client library implementation ---
  /// The feedback message with generic fields which wraps the feedback message.
  type FeedbackMessage = super::action::MoveWavemaker_FeedbackMessage;

  /// The send_goal service using a wrapped version of the goal message as a request.
  type SendGoalService = super::action::MoveWavemaker_SendGoal;

  /// The generic service to cancel a goal.
  type CancelGoalService = action_msgs::srv::rmw::CancelGoal;

  /// The get_result service using a wrapped version of the result message as a response.
  type GetResultService = super::action::MoveWavemaker_GetResult;

  // --- Methods for client library implementation ---
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_action_type_support_handle__wavemaker_interfaces__action__MoveWavemaker() }
  }

  fn create_goal_request(
    goal_id: &[u8; 16],
    goal: super::action::rmw::MoveWavemaker_Goal,
  ) -> super::action::rmw::MoveWavemaker_SendGoal_Request {
   super::action::rmw::MoveWavemaker_SendGoal_Request {
      goal_id: unique_identifier_msgs::msg::rmw::UUID { uuid: *goal_id },
      goal,
    }
  }

  fn split_goal_request(
    request: super::action::rmw::MoveWavemaker_SendGoal_Request,
  ) -> (
    [u8; 16],
   super::action::rmw::MoveWavemaker_Goal,
  ) {
    (request.goal_id.uuid, request.goal)
  }

  fn create_goal_response(
    accepted: bool,
    stamp: (i32, u32),
  ) -> super::action::rmw::MoveWavemaker_SendGoal_Response {
   super::action::rmw::MoveWavemaker_SendGoal_Response {
      accepted,
      stamp: builtin_interfaces::msg::rmw::Time {
        sec: stamp.0,
        nanosec: stamp.1,
      },
    }
  }

  fn get_goal_response_accepted(
    response: &super::action::rmw::MoveWavemaker_SendGoal_Response,
  ) -> bool {
    response.accepted
  }

  fn get_goal_response_stamp(
    response: &super::action::rmw::MoveWavemaker_SendGoal_Response,
  ) -> (i32, u32) {
    (response.stamp.sec, response.stamp.nanosec)
  }

  fn create_feedback_message(
    goal_id: &[u8; 16],
    feedback: super::action::rmw::MoveWavemaker_Feedback,
  ) -> super::action::rmw::MoveWavemaker_FeedbackMessage {
    let mut message = super::action::rmw::MoveWavemaker_FeedbackMessage::default();
    message.goal_id.uuid = *goal_id;
    message.feedback = feedback;
    message
  }

  fn split_feedback_message(
    feedback: super::action::rmw::MoveWavemaker_FeedbackMessage,
  ) -> (
    [u8; 16],
   super::action::rmw::MoveWavemaker_Feedback,
  ) {
    (feedback.goal_id.uuid, feedback.feedback)
  }

  fn create_result_request(
    goal_id: &[u8; 16],
  ) -> super::action::rmw::MoveWavemaker_GetResult_Request {
   super::action::rmw::MoveWavemaker_GetResult_Request {
      goal_id: unique_identifier_msgs::msg::rmw::UUID { uuid: *goal_id },
    }
  }

  fn get_result_request_uuid(
    request: &super::action::rmw::MoveWavemaker_GetResult_Request,
  ) -> &[u8; 16] {
    &request.goal_id.uuid
  }

  fn create_result_response(
    status: i8,
    result: super::action::rmw::MoveWavemaker_Result,
  ) -> super::action::rmw::MoveWavemaker_GetResult_Response {
   super::action::rmw::MoveWavemaker_GetResult_Response {
      status,
      result,
    }
  }

  fn split_result_response(
    response: super::action::rmw::MoveWavemaker_GetResult_Response
  ) -> (
    i8,
   super::action::rmw::MoveWavemaker_Result,
  ) {
    (response.status, response.result)
  }
}


