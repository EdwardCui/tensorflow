/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/debug_log.h"
#include "tensorflow/lite/micro/kernels/kernel_runner.h"
#include "tensorflow/lite/micro/testing/micro_test.h"
#include "tensorflow/lite/micro/testing/test_utils.h"

namespace tflite {
namespace testing {

void TestPackTwoInputsFloat(std::initializer_list<int> input1_dims_data,
                            std::initializer_list<float> input1_data,
                            std::initializer_list<int> input2_dims_data,
                            std::initializer_list<float> input2_data, int axis,
                            std::initializer_list<int> output_dims_data,
                            std::initializer_list<float> expected_output_data,
                            float* output_data) {
  TfLiteIntArray* input1_dims = IntArrayFromInitializer(input1_dims_data);
  TfLiteIntArray* input2_dims = IntArrayFromInitializer(input2_dims_data);
  TfLiteIntArray* output_dims = IntArrayFromInitializer(output_dims_data);
  const int output_dims_count = ElementCount(*output_dims);

  constexpr int input_size = 2;
  constexpr int output_size = 1;
  constexpr int tensors_size = input_size + output_size;
  TfLiteTensor tensors[tensors_size] = {
      CreateFloatTensor(input1_data, input1_dims),
      CreateFloatTensor(input2_data, input2_dims),
      CreateFloatTensor(output_data, output_dims)};

  // Place a unique value in the uninitialized output buffer.
  for (int i = 0; i < output_dims_count; ++i) {
    output_data[i] = 23;
  }

  TfLitePackParams builtin_data = {
      .values_count = 2,
      .axis = axis,
  };
  int inputs_array_data[] = {2, 0, 1};
  TfLiteIntArray* inputs_array = IntArrayFromInts(inputs_array_data);
  int outputs_array_data[] = {1, 2};
  TfLiteIntArray* outputs_array = IntArrayFromInts(outputs_array_data);

  const TfLiteRegistration registration = tflite::ops::micro::Register_PACK();
  micro::KernelRunner runner(
      registration, tensors, tensors_size, inputs_array, outputs_array,
      reinterpret_cast<void*>(&builtin_data), micro_test::reporter);

  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.InitAndPrepare());
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.Invoke());

  for (int i = 0; i < output_dims_count; ++i) {
    TF_LITE_MICRO_EXPECT_NEAR(expected_output_data.begin()[i], output_data[i],
                              1e-5f);
  }
}

void TestPackThreeInputsFloat(std::initializer_list<int> input1_dims_data,
                              std::initializer_list<float> input1_data,
                              std::initializer_list<int> input2_dims_data,
                              std::initializer_list<float> input2_data,
                              std::initializer_list<int> input3_dims_data,
                              std::initializer_list<float> input3_data,
                              int axis,
                              std::initializer_list<int> output_dims_data,
                              std::initializer_list<float> expected_output_data,
                              float* output_data) {
  TfLiteIntArray* input1_dims = IntArrayFromInitializer(input1_dims_data);
  TfLiteIntArray* input2_dims = IntArrayFromInitializer(input2_dims_data);
  TfLiteIntArray* input3_dims = IntArrayFromInitializer(input3_dims_data);
  TfLiteIntArray* output_dims = IntArrayFromInitializer(output_dims_data);
  const int output_dims_count = ElementCount(*output_dims);

  constexpr int input_size = 3;
  constexpr int output_size = 1;
  constexpr int tensors_size = input_size + output_size;
  TfLiteTensor tensors[tensors_size] = {
      CreateFloatTensor(input1_data, input1_dims),
      CreateFloatTensor(input2_data, input2_dims),
      CreateFloatTensor(input3_data, input3_dims),
      CreateFloatTensor(output_data, output_dims)};

  // Place a unique value in the uninitialized output buffer.
  for (int i = 0; i < output_dims_count; ++i) {
    output_data[i] = 23;
  }

  TfLitePackParams builtin_data = {
      .values_count = 3,
      .axis = axis,
  };

  int inputs_array_data[] = {3, 0, 1, 2};
  TfLiteIntArray* inputs_array = IntArrayFromInts(inputs_array_data);
  int outputs_array_data[] = {1, 3};
  TfLiteIntArray* outputs_array = IntArrayFromInts(outputs_array_data);

  const TfLiteRegistration registration = tflite::ops::micro::Register_PACK();
  micro::KernelRunner runner(
      registration, tensors, tensors_size, inputs_array, outputs_array,
      reinterpret_cast<void*>(&builtin_data), micro_test::reporter);

  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.InitAndPrepare());
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.Invoke());

  for (int i = 0; i < output_dims_count; ++i) {
    TF_LITE_MICRO_EXPECT_NEAR(expected_output_data.begin()[i], output_data[i],
                              1e-5f);
  }
}

void TestPackTwoInputsQuantized(
    std::initializer_list<int> input1_dims_data,
    std::initializer_list<uint8_t> input1_data,
    std::initializer_list<int> input2_dims_data,
    std::initializer_list<uint8_t> input2_data, int axis,
    std::initializer_list<int> output_dims_data,
    std::initializer_list<uint8_t> expected_output_data, uint8_t* output_data) {
  TfLiteIntArray* input1_dims = IntArrayFromInitializer(input1_dims_data);
  TfLiteIntArray* input2_dims = IntArrayFromInitializer(input2_dims_data);
  TfLiteIntArray* output_dims = IntArrayFromInitializer(output_dims_data);
  const int output_dims_count = ElementCount(*output_dims);

  constexpr int input_size = 2;
  constexpr int output_size = 1;
  constexpr int tensors_size = input_size + output_size;
  TfLiteTensor tensors[tensors_size] = {
      // CreateQuantizedTensor needs min/max values as input, but these values
      // don't matter as to the functionality of PACK, so just set as 0 and 10.
      CreateQuantizedTensor(input1_data, input1_dims, 0, 10),
      CreateQuantizedTensor(input2_data, input2_dims, 0, 10),
      CreateQuantizedTensor(output_data, output_dims, 0, 10)};

  // Place a unique value in the uninitialized output buffer.
  for (int i = 0; i < output_dims_count; ++i) {
    output_data[i] = 23;
  }

  TfLitePackParams builtin_data = {
      .values_count = 2,
      .axis = axis,
  };

  int inputs_array_data[] = {2, 0, 1};
  TfLiteIntArray* inputs_array = IntArrayFromInts(inputs_array_data);
  int outputs_array_data[] = {1, 2};
  TfLiteIntArray* outputs_array = IntArrayFromInts(outputs_array_data);

  const TfLiteRegistration registration = tflite::ops::micro::Register_PACK();
  micro::KernelRunner runner(
      registration, tensors, tensors_size, inputs_array, outputs_array,
      reinterpret_cast<void*>(&builtin_data), micro_test::reporter);

  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.InitAndPrepare());
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.Invoke());

  for (int i = 0; i < output_dims_count; ++i) {
    TF_LITE_MICRO_EXPECT_EQ(expected_output_data.begin()[i], output_data[i]);
  }
}

void TestPackTwoInputsQuantized32(
    std::initializer_list<int> input1_dims_data,
    std::initializer_list<int32_t> input1_data,
    std::initializer_list<int> input2_dims_data,
    std::initializer_list<int32_t> input2_data, int axis,
    std::initializer_list<int> output_dims_data,
    std::initializer_list<int32_t> expected_output_data, int32_t* output_data) {
  TfLiteIntArray* input1_dims = IntArrayFromInitializer(input1_dims_data);
  TfLiteIntArray* input2_dims = IntArrayFromInitializer(input2_dims_data);
  TfLiteIntArray* output_dims = IntArrayFromInitializer(output_dims_data);
  const int output_dims_count = ElementCount(*output_dims);

  constexpr int input_size = 2;
  constexpr int output_size = 1;
  constexpr int tensors_size = input_size + output_size;
  TfLiteTensor tensors[tensors_size] = {
      CreateQuantized32Tensor(input1_data, input1_dims, 1.0),
      CreateQuantized32Tensor(input2_data, input2_dims, 1.0),
      CreateQuantized32Tensor(output_data, output_dims, 1.0)};

  // Place a unique value in the uninitialized output buffer.
  for (int i = 0; i < output_dims_count; ++i) {
    output_data[i] = 23;
  }

  TfLitePackParams builtin_data = {
      .values_count = 2,
      .axis = axis,
  };

  int inputs_array_data[] = {2, 0, 1};
  TfLiteIntArray* inputs_array = IntArrayFromInts(inputs_array_data);
  int outputs_array_data[] = {1, 2};
  TfLiteIntArray* outputs_array = IntArrayFromInts(outputs_array_data);

  const TfLiteRegistration registration = tflite::ops::micro::Register_PACK();
  micro::KernelRunner runner(
      registration, tensors, tensors_size, inputs_array, outputs_array,
      reinterpret_cast<void*>(&builtin_data), micro_test::reporter);

  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.InitAndPrepare());
  TF_LITE_MICRO_EXPECT_EQ(kTfLiteOk, runner.Invoke());

  for (int i = 0; i < output_dims_count; ++i) {
    TF_LITE_MICRO_EXPECT_EQ(expected_output_data.begin()[i], output_data[i]);
  }
}

}  // namespace testing
}  // namespace tflite

TF_LITE_MICRO_TESTS_BEGIN

TF_LITE_MICRO_TEST(PackFloatThreeInputs) {
  constexpr int output_dims_count = 6;
  float output_data[output_dims_count];
  tflite::testing::TestPackThreeInputsFloat(
      {1, 2},        // Input1 shape
      {1, 4},        // Input1 values
      {1, 2},        // Input2 shape
      {2, 5},        // Input2 values
      {1, 2},        // Input3 shape
      {3, 6},        // Input3 values
      0, {2, 3, 2},  // Output shape
      {
          1, 4, 2, 5, 3, 6  // Output values
      },
      output_data);
}

TF_LITE_MICRO_TEST(PackFloatThreeInputsDifferentAxis) {
  constexpr int output_dims_count = 6;
  float output_data[output_dims_count];
  tflite::testing::TestPackThreeInputsFloat(
      {1, 2},        // Input1 shape
      {1, 4},        // Input1 values
      {1, 2},        // Input2 shape
      {2, 5},        // Input2 values
      {1, 2},        // Input3 shape
      {3, 6},        // Input3 values
      1, {2, 2, 3},  // Output shape
      {
          1, 2, 3, 4, 5, 6  // Output values
      },
      output_data);
}

TF_LITE_MICRO_TEST(PackFloatThreeInputsNegativeAxis) {
  constexpr int output_dims_count = 6;
  float output_data[output_dims_count];
  tflite::testing::TestPackThreeInputsFloat(
      {1, 2},         // Input1 shape
      {1, 4},         // Input1 values
      {1, 2},         // Input2 shape
      {2, 5},         // Input2 values
      {1, 2},         // Input3 shape
      {3, 6},         // Input3 values
      -1, {2, 2, 3},  // Output shape
      {
          1, 2, 3, 4, 5, 6  // Output values
      },
      output_data);
}

TF_LITE_MICRO_TEST(PackFloatMultilDimensions) {
  constexpr int output_dims_count = 12;
  float output_data[output_dims_count];
  tflite::testing::TestPackTwoInputsFloat(
      {2, 2, 3},              // Input1 shape
      {1, 2, 3, 4, 5, 6},     // Input1 values
      {2, 2, 3},              // Input2 shape
      {7, 8, 9, 10, 11, 12},  // Input2 values
      1, {3, 2, 2, 3},        // Output shape
      {
          1, 2, 3, 7, 8, 9, 4, 5, 6, 10, 11, 12  // Output values
      },
      output_data);
}

TF_LITE_MICRO_TEST(PackQuantizedMultilDimensions) {
  constexpr int output_dims_count = 12;
  uint8_t output_data[output_dims_count];
  tflite::testing::TestPackTwoInputsQuantized(
      {2, 2, 3},              // Input1 shape
      {1, 2, 3, 4, 5, 6},     // Input1 values
      {2, 2, 3},              // Input2 shape
      {7, 8, 9, 10, 11, 12},  // Input2 values
      1, {3, 2, 2, 3},        // Output shape
      {
          1, 2, 3, 7, 8, 9, 4, 5, 6, 10, 11, 12  // Output values
      },
      output_data);
}

TF_LITE_MICRO_TEST(PackQuantized32MultilDimensions) {
  constexpr int output_dims_count = 12;
  int32_t output_data[output_dims_count];
  tflite::testing::TestPackTwoInputsQuantized32(
      {2, 2, 3},              // Input1 shape
      {1, 2, 3, 4, 5, 6},     // Input1 values
      {2, 2, 3},              // Input2 shape
      {7, 8, 9, 10, 11, 12},  // Input2 values
      1, {3, 2, 2, 3},        // Output shape
      {
          1, 2, 3, 7, 8, 9, 4, 5, 6, 10, 11, 12  // Output values
      },
      output_data);
}

TF_LITE_MICRO_TESTS_END
