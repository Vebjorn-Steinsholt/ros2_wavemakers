// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from wavemaker_interfaces:action/MoveWavemaker.idl
// generated code does not contain a copyright notice
#include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `positions`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
wavemaker_interfaces__action__MoveWavemaker_Goal__init(wavemaker_interfaces__action__MoveWavemaker_Goal * msg)
{
  if (!msg) {
    return false;
  }
  // amplitude
  // period
  // positions
  if (!rosidl_runtime_c__double__Sequence__init(&msg->positions, 0)) {
    wavemaker_interfaces__action__MoveWavemaker_Goal__fini(msg);
    return false;
  }
  // sample_interval
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_Goal__fini(wavemaker_interfaces__action__MoveWavemaker_Goal * msg)
{
  if (!msg) {
    return;
  }
  // amplitude
  // period
  // positions
  rosidl_runtime_c__double__Sequence__fini(&msg->positions);
  // sample_interval
}

bool
wavemaker_interfaces__action__MoveWavemaker_Goal__are_equal(const wavemaker_interfaces__action__MoveWavemaker_Goal * lhs, const wavemaker_interfaces__action__MoveWavemaker_Goal * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // amplitude
  if (lhs->amplitude != rhs->amplitude) {
    return false;
  }
  // period
  if (lhs->period != rhs->period) {
    return false;
  }
  // positions
  if (!rosidl_runtime_c__double__Sequence__are_equal(
      &(lhs->positions), &(rhs->positions)))
  {
    return false;
  }
  // sample_interval
  if (lhs->sample_interval != rhs->sample_interval) {
    return false;
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_Goal__copy(
  const wavemaker_interfaces__action__MoveWavemaker_Goal * input,
  wavemaker_interfaces__action__MoveWavemaker_Goal * output)
{
  if (!input || !output) {
    return false;
  }
  // amplitude
  output->amplitude = input->amplitude;
  // period
  output->period = input->period;
  // positions
  if (!rosidl_runtime_c__double__Sequence__copy(
      &(input->positions), &(output->positions)))
  {
    return false;
  }
  // sample_interval
  output->sample_interval = input->sample_interval;
  return true;
}

wavemaker_interfaces__action__MoveWavemaker_Goal *
wavemaker_interfaces__action__MoveWavemaker_Goal__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_Goal * msg = (wavemaker_interfaces__action__MoveWavemaker_Goal *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_Goal), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wavemaker_interfaces__action__MoveWavemaker_Goal));
  bool success = wavemaker_interfaces__action__MoveWavemaker_Goal__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wavemaker_interfaces__action__MoveWavemaker_Goal__destroy(wavemaker_interfaces__action__MoveWavemaker_Goal * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wavemaker_interfaces__action__MoveWavemaker_Goal__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__init(wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_Goal * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_Goal)) {
      return false;
    }
    data = (wavemaker_interfaces__action__MoveWavemaker_Goal *)allocator.zero_allocate(size, sizeof(wavemaker_interfaces__action__MoveWavemaker_Goal), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wavemaker_interfaces__action__MoveWavemaker_Goal__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wavemaker_interfaces__action__MoveWavemaker_Goal__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__fini(wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wavemaker_interfaces__action__MoveWavemaker_Goal__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence *
wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence * array = (wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__destroy(wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__are_equal(const wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence * lhs, const wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_Goal__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence__copy(
  const wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence * input,
  wavemaker_interfaces__action__MoveWavemaker_Goal__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_Goal)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(wavemaker_interfaces__action__MoveWavemaker_Goal);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wavemaker_interfaces__action__MoveWavemaker_Goal * data =
      (wavemaker_interfaces__action__MoveWavemaker_Goal *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wavemaker_interfaces__action__MoveWavemaker_Goal__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wavemaker_interfaces__action__MoveWavemaker_Goal__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_Goal__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `message`
#include "rosidl_runtime_c/string_functions.h"

bool
wavemaker_interfaces__action__MoveWavemaker_Result__init(wavemaker_interfaces__action__MoveWavemaker_Result * msg)
{
  if (!msg) {
    return false;
  }
  // success
  // message
  if (!rosidl_runtime_c__String__init(&msg->message)) {
    wavemaker_interfaces__action__MoveWavemaker_Result__fini(msg);
    return false;
  }
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_Result__fini(wavemaker_interfaces__action__MoveWavemaker_Result * msg)
{
  if (!msg) {
    return;
  }
  // success
  // message
  rosidl_runtime_c__String__fini(&msg->message);
}

bool
wavemaker_interfaces__action__MoveWavemaker_Result__are_equal(const wavemaker_interfaces__action__MoveWavemaker_Result * lhs, const wavemaker_interfaces__action__MoveWavemaker_Result * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // success
  if (lhs->success != rhs->success) {
    return false;
  }
  // message
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->message), &(rhs->message)))
  {
    return false;
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_Result__copy(
  const wavemaker_interfaces__action__MoveWavemaker_Result * input,
  wavemaker_interfaces__action__MoveWavemaker_Result * output)
{
  if (!input || !output) {
    return false;
  }
  // success
  output->success = input->success;
  // message
  if (!rosidl_runtime_c__String__copy(
      &(input->message), &(output->message)))
  {
    return false;
  }
  return true;
}

wavemaker_interfaces__action__MoveWavemaker_Result *
wavemaker_interfaces__action__MoveWavemaker_Result__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_Result * msg = (wavemaker_interfaces__action__MoveWavemaker_Result *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_Result), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wavemaker_interfaces__action__MoveWavemaker_Result));
  bool success = wavemaker_interfaces__action__MoveWavemaker_Result__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wavemaker_interfaces__action__MoveWavemaker_Result__destroy(wavemaker_interfaces__action__MoveWavemaker_Result * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wavemaker_interfaces__action__MoveWavemaker_Result__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__init(wavemaker_interfaces__action__MoveWavemaker_Result__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_Result * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_Result)) {
      return false;
    }
    data = (wavemaker_interfaces__action__MoveWavemaker_Result *)allocator.zero_allocate(size, sizeof(wavemaker_interfaces__action__MoveWavemaker_Result), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wavemaker_interfaces__action__MoveWavemaker_Result__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wavemaker_interfaces__action__MoveWavemaker_Result__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__fini(wavemaker_interfaces__action__MoveWavemaker_Result__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wavemaker_interfaces__action__MoveWavemaker_Result__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wavemaker_interfaces__action__MoveWavemaker_Result__Sequence *
wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_Result__Sequence * array = (wavemaker_interfaces__action__MoveWavemaker_Result__Sequence *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_Result__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__destroy(wavemaker_interfaces__action__MoveWavemaker_Result__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__are_equal(const wavemaker_interfaces__action__MoveWavemaker_Result__Sequence * lhs, const wavemaker_interfaces__action__MoveWavemaker_Result__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_Result__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_Result__Sequence__copy(
  const wavemaker_interfaces__action__MoveWavemaker_Result__Sequence * input,
  wavemaker_interfaces__action__MoveWavemaker_Result__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_Result)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(wavemaker_interfaces__action__MoveWavemaker_Result);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wavemaker_interfaces__action__MoveWavemaker_Result * data =
      (wavemaker_interfaces__action__MoveWavemaker_Result *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wavemaker_interfaces__action__MoveWavemaker_Result__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wavemaker_interfaces__action__MoveWavemaker_Result__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_Result__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


bool
wavemaker_interfaces__action__MoveWavemaker_Feedback__init(wavemaker_interfaces__action__MoveWavemaker_Feedback * msg)
{
  if (!msg) {
    return false;
  }
  // desired_position
  // actual_position
  // elapsed_time
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_Feedback__fini(wavemaker_interfaces__action__MoveWavemaker_Feedback * msg)
{
  if (!msg) {
    return;
  }
  // desired_position
  // actual_position
  // elapsed_time
}

bool
wavemaker_interfaces__action__MoveWavemaker_Feedback__are_equal(const wavemaker_interfaces__action__MoveWavemaker_Feedback * lhs, const wavemaker_interfaces__action__MoveWavemaker_Feedback * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // desired_position
  if (lhs->desired_position != rhs->desired_position) {
    return false;
  }
  // actual_position
  if (lhs->actual_position != rhs->actual_position) {
    return false;
  }
  // elapsed_time
  if (lhs->elapsed_time != rhs->elapsed_time) {
    return false;
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_Feedback__copy(
  const wavemaker_interfaces__action__MoveWavemaker_Feedback * input,
  wavemaker_interfaces__action__MoveWavemaker_Feedback * output)
{
  if (!input || !output) {
    return false;
  }
  // desired_position
  output->desired_position = input->desired_position;
  // actual_position
  output->actual_position = input->actual_position;
  // elapsed_time
  output->elapsed_time = input->elapsed_time;
  return true;
}

wavemaker_interfaces__action__MoveWavemaker_Feedback *
wavemaker_interfaces__action__MoveWavemaker_Feedback__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_Feedback * msg = (wavemaker_interfaces__action__MoveWavemaker_Feedback *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_Feedback), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wavemaker_interfaces__action__MoveWavemaker_Feedback));
  bool success = wavemaker_interfaces__action__MoveWavemaker_Feedback__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wavemaker_interfaces__action__MoveWavemaker_Feedback__destroy(wavemaker_interfaces__action__MoveWavemaker_Feedback * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wavemaker_interfaces__action__MoveWavemaker_Feedback__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__init(wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_Feedback * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_Feedback)) {
      return false;
    }
    data = (wavemaker_interfaces__action__MoveWavemaker_Feedback *)allocator.zero_allocate(size, sizeof(wavemaker_interfaces__action__MoveWavemaker_Feedback), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wavemaker_interfaces__action__MoveWavemaker_Feedback__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wavemaker_interfaces__action__MoveWavemaker_Feedback__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__fini(wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wavemaker_interfaces__action__MoveWavemaker_Feedback__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence *
wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence * array = (wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__destroy(wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__are_equal(const wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence * lhs, const wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_Feedback__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence__copy(
  const wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence * input,
  wavemaker_interfaces__action__MoveWavemaker_Feedback__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_Feedback)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(wavemaker_interfaces__action__MoveWavemaker_Feedback);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wavemaker_interfaces__action__MoveWavemaker_Feedback * data =
      (wavemaker_interfaces__action__MoveWavemaker_Feedback *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wavemaker_interfaces__action__MoveWavemaker_Feedback__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wavemaker_interfaces__action__MoveWavemaker_Feedback__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_Feedback__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `goal_id`
#include "unique_identifier_msgs/msg/detail/uuid__functions.h"
// Member `goal`
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__init(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__fini(msg);
    return false;
  }
  // goal
  if (!wavemaker_interfaces__action__MoveWavemaker_Goal__init(&msg->goal)) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__fini(msg);
    return false;
  }
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__fini(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
  // goal
  wavemaker_interfaces__action__MoveWavemaker_Goal__fini(&msg->goal);
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__are_equal(const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * lhs, const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__are_equal(
      &(lhs->goal_id), &(rhs->goal_id)))
  {
    return false;
  }
  // goal
  if (!wavemaker_interfaces__action__MoveWavemaker_Goal__are_equal(
      &(lhs->goal), &(rhs->goal)))
  {
    return false;
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__copy(
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * input,
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__copy(
      &(input->goal_id), &(output->goal_id)))
  {
    return false;
  }
  // goal
  if (!wavemaker_interfaces__action__MoveWavemaker_Goal__copy(
      &(input->goal), &(output->goal)))
  {
    return false;
  }
  return true;
}

wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request *
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * msg = (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request));
  bool success = wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__destroy(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__init(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request)) {
      return false;
    }
    data = (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request *)allocator.zero_allocate(size, sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__fini(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence *
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * array = (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__destroy(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__are_equal(const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * lhs, const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__copy(
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * input,
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request * data =
      (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `stamp`
#include "builtin_interfaces/msg/detail/time__functions.h"

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__init(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * msg)
{
  if (!msg) {
    return false;
  }
  // accepted
  // stamp
  if (!builtin_interfaces__msg__Time__init(&msg->stamp)) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__fini(msg);
    return false;
  }
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__fini(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * msg)
{
  if (!msg) {
    return;
  }
  // accepted
  // stamp
  builtin_interfaces__msg__Time__fini(&msg->stamp);
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__are_equal(const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * lhs, const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // accepted
  if (lhs->accepted != rhs->accepted) {
    return false;
  }
  // stamp
  if (!builtin_interfaces__msg__Time__are_equal(
      &(lhs->stamp), &(rhs->stamp)))
  {
    return false;
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__copy(
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * input,
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // accepted
  output->accepted = input->accepted;
  // stamp
  if (!builtin_interfaces__msg__Time__copy(
      &(input->stamp), &(output->stamp)))
  {
    return false;
  }
  return true;
}

wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response *
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * msg = (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response));
  bool success = wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__destroy(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__init(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response)) {
      return false;
    }
    data = (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response *)allocator.zero_allocate(size, sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__fini(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence *
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * array = (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__destroy(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__are_equal(const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * lhs, const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__copy(
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * input,
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response * data =
      (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `info`
#include "service_msgs/msg/detail/service_event_info__functions.h"
// Member `request`
// Member `response`
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__init(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__fini(msg);
    return false;
  }
  // request
  if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__init(&msg->request, 0)) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__fini(msg);
    return false;
  }
  // response
  if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__init(&msg->response, 0)) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__fini(msg);
    return false;
  }
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__fini(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__fini(&msg->request);
  // response
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__fini(&msg->response);
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__are_equal(const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * lhs, const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__are_equal(
      &(lhs->info), &(rhs->info)))
  {
    return false;
  }
  // request
  if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__copy(
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * input,
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * output)
{
  if (!input || !output) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__copy(
      &(input->info), &(output->info)))
  {
    return false;
  }
  // request
  if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event *
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * msg = (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event));
  bool success = wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__destroy(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence__init(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event)) {
      return false;
    }
    data = (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event *)allocator.zero_allocate(size, sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence__fini(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence *
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence * array = (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence__destroy(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence__are_equal(const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence * lhs, const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence__copy(
  const wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence * input,
  wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event * data =
      (wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_SendGoal_Event__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__functions.h"

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__init(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__fini(msg);
    return false;
  }
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__fini(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__are_equal(const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * lhs, const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__are_equal(
      &(lhs->goal_id), &(rhs->goal_id)))
  {
    return false;
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__copy(
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * input,
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__copy(
      &(input->goal_id), &(output->goal_id)))
  {
    return false;
  }
  return true;
}

wavemaker_interfaces__action__MoveWavemaker_GetResult_Request *
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * msg = (wavemaker_interfaces__action__MoveWavemaker_GetResult_Request *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request));
  bool success = wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__destroy(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__init(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request)) {
      return false;
    }
    data = (wavemaker_interfaces__action__MoveWavemaker_GetResult_Request *)allocator.zero_allocate(size, sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__fini(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence *
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * array = (wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__destroy(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__are_equal(const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * lhs, const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__copy(
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * input,
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Request * data =
      (wavemaker_interfaces__action__MoveWavemaker_GetResult_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `result`
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__init(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * msg)
{
  if (!msg) {
    return false;
  }
  // status
  // result
  if (!wavemaker_interfaces__action__MoveWavemaker_Result__init(&msg->result)) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__fini(msg);
    return false;
  }
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__fini(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * msg)
{
  if (!msg) {
    return;
  }
  // status
  // result
  wavemaker_interfaces__action__MoveWavemaker_Result__fini(&msg->result);
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__are_equal(const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * lhs, const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // status
  if (lhs->status != rhs->status) {
    return false;
  }
  // result
  if (!wavemaker_interfaces__action__MoveWavemaker_Result__are_equal(
      &(lhs->result), &(rhs->result)))
  {
    return false;
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__copy(
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * input,
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // status
  output->status = input->status;
  // result
  if (!wavemaker_interfaces__action__MoveWavemaker_Result__copy(
      &(input->result), &(output->result)))
  {
    return false;
  }
  return true;
}

wavemaker_interfaces__action__MoveWavemaker_GetResult_Response *
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * msg = (wavemaker_interfaces__action__MoveWavemaker_GetResult_Response *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response));
  bool success = wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__destroy(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__init(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response)) {
      return false;
    }
    data = (wavemaker_interfaces__action__MoveWavemaker_GetResult_Response *)allocator.zero_allocate(size, sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__fini(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence *
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * array = (wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__destroy(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__are_equal(const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * lhs, const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__copy(
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * input,
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Response * data =
      (wavemaker_interfaces__action__MoveWavemaker_GetResult_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `info`
// already included above
// #include "service_msgs/msg/detail/service_event_info__functions.h"
// Member `request`
// Member `response`
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__init(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * msg)
{
  if (!msg) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__init(&msg->info)) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__fini(msg);
    return false;
  }
  // request
  if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__init(&msg->request, 0)) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__fini(msg);
    return false;
  }
  // response
  if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__init(&msg->response, 0)) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__fini(msg);
    return false;
  }
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__fini(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * msg)
{
  if (!msg) {
    return;
  }
  // info
  service_msgs__msg__ServiceEventInfo__fini(&msg->info);
  // request
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__fini(&msg->request);
  // response
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__fini(&msg->response);
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__are_equal(const wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * lhs, const wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__are_equal(
      &(lhs->info), &(rhs->info)))
  {
    return false;
  }
  // request
  if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__are_equal(
      &(lhs->request), &(rhs->request)))
  {
    return false;
  }
  // response
  if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__are_equal(
      &(lhs->response), &(rhs->response)))
  {
    return false;
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__copy(
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * input,
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * output)
{
  if (!input || !output) {
    return false;
  }
  // info
  if (!service_msgs__msg__ServiceEventInfo__copy(
      &(input->info), &(output->info)))
  {
    return false;
  }
  // request
  if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Request__Sequence__copy(
      &(input->request), &(output->request)))
  {
    return false;
  }
  // response
  if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Response__Sequence__copy(
      &(input->response), &(output->response)))
  {
    return false;
  }
  return true;
}

wavemaker_interfaces__action__MoveWavemaker_GetResult_Event *
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * msg = (wavemaker_interfaces__action__MoveWavemaker_GetResult_Event *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event));
  bool success = wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__destroy(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence__init(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event)) {
      return false;
    }
    data = (wavemaker_interfaces__action__MoveWavemaker_GetResult_Event *)allocator.zero_allocate(size, sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence__fini(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence *
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence * array = (wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence__destroy(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence__are_equal(const wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence * lhs, const wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence__copy(
  const wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence * input,
  wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(wavemaker_interfaces__action__MoveWavemaker_GetResult_Event);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wavemaker_interfaces__action__MoveWavemaker_GetResult_Event * data =
      (wavemaker_interfaces__action__MoveWavemaker_GetResult_Event *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_GetResult_Event__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `goal_id`
// already included above
// #include "unique_identifier_msgs/msg/detail/uuid__functions.h"
// Member `feedback`
// already included above
// #include "wavemaker_interfaces/action/detail/move_wavemaker__functions.h"

bool
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__init(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * msg)
{
  if (!msg) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__init(&msg->goal_id)) {
    wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__fini(msg);
    return false;
  }
  // feedback
  if (!wavemaker_interfaces__action__MoveWavemaker_Feedback__init(&msg->feedback)) {
    wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__fini(msg);
    return false;
  }
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__fini(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * msg)
{
  if (!msg) {
    return;
  }
  // goal_id
  unique_identifier_msgs__msg__UUID__fini(&msg->goal_id);
  // feedback
  wavemaker_interfaces__action__MoveWavemaker_Feedback__fini(&msg->feedback);
}

bool
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__are_equal(const wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * lhs, const wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__are_equal(
      &(lhs->goal_id), &(rhs->goal_id)))
  {
    return false;
  }
  // feedback
  if (!wavemaker_interfaces__action__MoveWavemaker_Feedback__are_equal(
      &(lhs->feedback), &(rhs->feedback)))
  {
    return false;
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__copy(
  const wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * input,
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * output)
{
  if (!input || !output) {
    return false;
  }
  // goal_id
  if (!unique_identifier_msgs__msg__UUID__copy(
      &(input->goal_id), &(output->goal_id)))
  {
    return false;
  }
  // feedback
  if (!wavemaker_interfaces__action__MoveWavemaker_Feedback__copy(
      &(input->feedback), &(output->feedback)))
  {
    return false;
  }
  return true;
}

wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage *
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__create(void)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * msg = (wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage));
  bool success = wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__destroy(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__init(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * data = NULL;

  if (size) {
    if (size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage)) {
      return false;
    }
    data = (wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage *)allocator.zero_allocate(size, sizeof(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__fini(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence *
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence * array = (wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence *)allocator.allocate(sizeof(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__destroy(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__are_equal(const wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence * lhs, const wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence__copy(
  const wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence * input,
  wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    if (input->size > SIZE_MAX / sizeof(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage)) {
      return false;
    }
    const size_t allocation_size =
      input->size * sizeof(wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage * data =
      (wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!wavemaker_interfaces__action__MoveWavemaker_FeedbackMessage__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
