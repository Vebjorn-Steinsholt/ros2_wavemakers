
#[cfg(feature = "serde")]
use serde::{Deserialize, Serialize};


#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_Goal() -> *const std::ffi::c_void;
}

#[link(name = "wavemaker_interfaces__rosidl_generator_c")]
extern "C" {
    fn wavemaker_interfaces__action__MoveWavemaker_Goal__init(msg: *mut MoveWavemaker_Goal) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_Goal>, size: usize) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_Goal>);
    fn wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MoveWavemaker_Goal>, out_seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_Goal>) -> bool;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker_Goal
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
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
    pub positions: rosidl_runtime_rs::Sequence<f64>,


    // This member is not documented.
    #[allow(missing_docs)]
    pub sample_interval: f64,

}



impl Default for MoveWavemaker_Goal {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wavemaker_interfaces__action__MoveWavemaker_Goal__init(&mut msg as *mut _) {
        panic!("Call to wavemaker_interfaces__action__MoveWavemaker_Goal__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MoveWavemaker_Goal {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_Goal {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MoveWavemaker_Goal where Self: Sized {
  const TYPE_NAME: &'static str = "wavemaker_interfaces/action/MoveWavemaker_Goal";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_Goal() }
  }
}


#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_Result() -> *const std::ffi::c_void;
}

#[link(name = "wavemaker_interfaces__rosidl_generator_c")]
extern "C" {
    fn wavemaker_interfaces__action__MoveWavemaker_Result__init(msg: *mut MoveWavemaker_Result) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_Result>, size: usize) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_Result>);
    fn wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MoveWavemaker_Result>, out_seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_Result>) -> bool;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker_Result
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_Result {

    // This member is not documented.
    #[allow(missing_docs)]
    pub success: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub message: rosidl_runtime_rs::String,

}



impl Default for MoveWavemaker_Result {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wavemaker_interfaces__action__MoveWavemaker_Result__init(&mut msg as *mut _) {
        panic!("Call to wavemaker_interfaces__action__MoveWavemaker_Result__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MoveWavemaker_Result {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_Result {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MoveWavemaker_Result where Self: Sized {
  const TYPE_NAME: &'static str = "wavemaker_interfaces/action/MoveWavemaker_Result";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_Result() }
  }
}


#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_Feedback() -> *const std::ffi::c_void;
}

#[link(name = "wavemaker_interfaces__rosidl_generator_c")]
extern "C" {
    fn wavemaker_interfaces__action__MoveWavemaker_Feedback__init(msg: *mut MoveWavemaker_Feedback) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_Feedback>, size: usize) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_Feedback>);
    fn wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MoveWavemaker_Feedback>, out_seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_Feedback>) -> bool;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker_Feedback
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
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
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wavemaker_interfaces__action__MoveWavemaker_Feedback__init(&mut msg as *mut _) {
        panic!("Call to wavemaker_interfaces__action__MoveWavemaker_Feedback__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MoveWavemaker_Feedback {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_Feedback {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MoveWavemaker_Feedback where Self: Sized {
  const TYPE_NAME: &'static str = "wavemaker_interfaces/action/MoveWavemaker_Feedback";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_Feedback() }
  }
}


#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage() -> *const std::ffi::c_void;
}

#[link(name = "wavemaker_interfaces__rosidl_generator_c")]
extern "C" {
    fn wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__init(msg: *mut MoveWavemaker_FeedbackMessage) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_FeedbackMessage>, size: usize) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_FeedbackMessage>);
    fn wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MoveWavemaker_FeedbackMessage>, out_seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_FeedbackMessage>) -> bool;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_FeedbackMessage {

    // This member is not documented.
    #[allow(missing_docs)]
    pub goal_id: unique_identifier_msgs::msg::rmw::UUID,


    // This member is not documented.
    #[allow(missing_docs)]
    pub feedback: super::super::action::rmw::MoveWavemaker_Feedback,

}



impl Default for MoveWavemaker_FeedbackMessage {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__init(&mut msg as *mut _) {
        panic!("Call to wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MoveWavemaker_FeedbackMessage {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_FeedbackMessage {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MoveWavemaker_FeedbackMessage where Self: Sized {
  const TYPE_NAME: &'static str = "wavemaker_interfaces/action/MoveWavemaker_FeedbackMessage";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage() }
  }
}




#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request() -> *const std::ffi::c_void;
}

#[link(name = "wavemaker_interfaces__rosidl_generator_c")]
extern "C" {
    fn wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__init(msg: *mut MoveWavemaker_SendGoal_Request) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_SendGoal_Request>, size: usize) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_SendGoal_Request>);
    fn wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MoveWavemaker_SendGoal_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_SendGoal_Request>) -> bool;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_SendGoal_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub goal_id: unique_identifier_msgs::msg::rmw::UUID,


    // This member is not documented.
    #[allow(missing_docs)]
    pub goal: super::super::action::rmw::MoveWavemaker_Goal,

}



impl Default for MoveWavemaker_SendGoal_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__init(&mut msg as *mut _) {
        panic!("Call to wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MoveWavemaker_SendGoal_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_SendGoal_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MoveWavemaker_SendGoal_Request where Self: Sized {
  const TYPE_NAME: &'static str = "wavemaker_interfaces/action/MoveWavemaker_SendGoal_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request() }
  }
}


#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response() -> *const std::ffi::c_void;
}

#[link(name = "wavemaker_interfaces__rosidl_generator_c")]
extern "C" {
    fn wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__init(msg: *mut MoveWavemaker_SendGoal_Response) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_SendGoal_Response>, size: usize) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_SendGoal_Response>);
    fn wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MoveWavemaker_SendGoal_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_SendGoal_Response>) -> bool;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_SendGoal_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub accepted: bool,


    // This member is not documented.
    #[allow(missing_docs)]
    pub stamp: builtin_interfaces::msg::rmw::Time,

}



impl Default for MoveWavemaker_SendGoal_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__init(&mut msg as *mut _) {
        panic!("Call to wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MoveWavemaker_SendGoal_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_SendGoal_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MoveWavemaker_SendGoal_Response where Self: Sized {
  const TYPE_NAME: &'static str = "wavemaker_interfaces/action/MoveWavemaker_SendGoal_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response() }
  }
}


#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_GetResult_Request() -> *const std::ffi::c_void;
}

#[link(name = "wavemaker_interfaces__rosidl_generator_c")]
extern "C" {
    fn wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__init(msg: *mut MoveWavemaker_GetResult_Request) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_GetResult_Request>, size: usize) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_GetResult_Request>);
    fn wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MoveWavemaker_GetResult_Request>, out_seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_GetResult_Request>) -> bool;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker_GetResult_Request
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_GetResult_Request {

    // This member is not documented.
    #[allow(missing_docs)]
    pub goal_id: unique_identifier_msgs::msg::rmw::UUID,

}



impl Default for MoveWavemaker_GetResult_Request {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__init(&mut msg as *mut _) {
        panic!("Call to wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MoveWavemaker_GetResult_Request {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_GetResult_Request {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MoveWavemaker_GetResult_Request where Self: Sized {
  const TYPE_NAME: &'static str = "wavemaker_interfaces/action/MoveWavemaker_GetResult_Request";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_GetResult_Request() }
  }
}


#[link(name = "wavemaker_interfaces__rosidl_typesupport_c")]
extern "C" {
    fn rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_GetResult_Response() -> *const std::ffi::c_void;
}

#[link(name = "wavemaker_interfaces__rosidl_generator_c")]
extern "C" {
    fn wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__init(msg: *mut MoveWavemaker_GetResult_Response) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__init(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_GetResult_Response>, size: usize) -> bool;
    fn wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__fini(seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_GetResult_Response>);
    fn wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__copy(in_seq: &rosidl_runtime_rs::Sequence<MoveWavemaker_GetResult_Response>, out_seq: *mut rosidl_runtime_rs::Sequence<MoveWavemaker_GetResult_Response>) -> bool;
}

// Corresponds to wavemaker_interfaces__action__MoveWavemaker_GetResult_Response
#[cfg_attr(feature = "serde", derive(Deserialize, Serialize))]


// This struct is not documented.
#[allow(missing_docs)]

#[allow(non_camel_case_types)]
#[repr(C)]
#[derive(Clone, Debug, PartialEq, PartialOrd)]
pub struct MoveWavemaker_GetResult_Response {

    // This member is not documented.
    #[allow(missing_docs)]
    pub status: i8,


    // This member is not documented.
    #[allow(missing_docs)]
    pub result: super::super::action::rmw::MoveWavemaker_Result,

}



impl Default for MoveWavemaker_GetResult_Response {
  fn default() -> Self {
    unsafe {
      let mut msg = std::mem::zeroed();
      if !wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__init(&mut msg as *mut _) {
        panic!("Call to wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__init() failed");
      }
      msg
    }
  }
}

impl rosidl_runtime_rs::SequenceAlloc for MoveWavemaker_GetResult_Response {
  fn sequence_init(seq: &mut rosidl_runtime_rs::Sequence<Self>, size: usize) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__init(seq as *mut _, size) }
  }
  fn sequence_fini(seq: &mut rosidl_runtime_rs::Sequence<Self>) {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__fini(seq as *mut _) }
  }
  fn sequence_copy(in_seq: &rosidl_runtime_rs::Sequence<Self>, out_seq: &mut rosidl_runtime_rs::Sequence<Self>) -> bool {
    // SAFETY: This is safe since the pointer is guaranteed to be valid/initialized.
    unsafe { wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__copy(in_seq, out_seq as *mut _) }
  }
}

impl rosidl_runtime_rs::Message for MoveWavemaker_GetResult_Response {
  type RmwMsg = Self;
  fn into_rmw_message(msg_cow: std::borrow::Cow<'_, Self>) -> std::borrow::Cow<'_, Self::RmwMsg> { msg_cow }
  fn from_rmw_message(msg: Self::RmwMsg) -> Self { msg }
}

impl rosidl_runtime_rs::RmwMessage for MoveWavemaker_GetResult_Response where Self: Sized {
  const TYPE_NAME: &'static str = "wavemaker_interfaces/action/MoveWavemaker_GetResult_Response";
  fn get_type_support() -> *const std::ffi::c_void {
    // SAFETY: No preconditions for this function.
    unsafe { rosidl_typesupport_c__get_message_type_support_handle__wavemaker_interfaces__action__MoveWavemaker_GetResult_Response() }
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


