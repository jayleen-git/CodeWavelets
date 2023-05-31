#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <math.h>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * Extends the input vector by adding reflection or constant values at the
 * beginning and end.
 *
 * @param input The input vector to be extended.
 * @param extendLen The length by which the vector should be extended.
 * @param mode The extension mode, which can be "reflection" or "constant".
 * @return The extended vector.
 */
std::vector<double> wextend(const std::vector<double> &input,
                            const int extendLen, const std::string &mode) {

  int extendSize = input.size() + extendLen * 2;
  std::vector<double> extendedinput(extendSize);
  int inputSize = input.size();

  if (input.empty() || extendLen == 0) {
    return input;
  }
  if (input.size() < extendLen) {
    throw std::runtime_error("input size is less than extendLen!");
  }

  if (mode == "sym") {
    // 对称延拓extendLen个点
    for (int i = 0; i < extendLen; ++i) {
      // 向前对称延拓
      extendedinput[i] = input[extendLen - i - 1];
      // 向后对称延拓
      extendedinput[i + extendLen + inputSize] = input[inputSize - i - 1];
    }
    // 复制原始信号
    for (int i = 0; i < inputSize; ++i) {
      extendedinput[i + extendLen] = input[i];
    }
  } else {
    throw std::runtime_error("Mode error!");
  }

  return extendedinput;
}

/**
 * Performs a 1-D convolution of the input vector with the given filter.
 *
 * @param input The input vector.
 * @param filter The filter.
 * @param mode The convolution mode, which can be "full", "same" or "valid".
 * @return The convolution result.
 */
std::vector<double> wconv1(const std::vector<double> &input,
                           const std::vector<double> &wfilters,
                           const std::string &mode) {
  
  if (input.empty() || wfilters.empty()) {
    throw std::runtime_error("input or wfilters is empty!");
  }

  if (mode == "full") {
    int outputSize = input.size() + wfilters.size() - 1;
    std::vector<double> output(outputSize, 0.0);
    // zero padding for input and filters
    std::vector<double> paddedInput(outputSize, 0.0);
    std::vector<double> paddedFilters(outputSize, 0.0);
    // zero padding for input and filters
    std::copy(input.begin(), input.end(), paddedInput.begin());
    std::copy(wfilters.begin(), wfilters.end(), paddedFilters.begin());
    for (int i = 0; i < outputSize; ++i) {
      output[i] = 0.0;
      for (int j = 0; j <= i; ++j) {
        const int filterIndex = i - j;
        output[i] += paddedInput[j] * paddedFilters[filterIndex];
      }
    }
    return output;
  } else if (mode == "same") {
    // later
    return input;
  } else if (mode == "valid") {
    if (input.size() < wfilters.size()) {
      return std::vector<double>();
    }
    int outputSize = input.size() - wfilters.size() + 1;
    std::vector<double> output(outputSize, 0.0);
    for (int i = 0; i < outputSize; ++i) {
      for (size_t j = 0; j < wfilters.size(); ++j) {
        const int filterIndex = wfilters.size() - j - 1;
        output[i] += input[i + j] * wfilters[filterIndex];
      }
    }
    return output;
  } else {
    throw std::runtime_error("Invalid padding mode!");
  }
  return std::vector<double>();
}

/**
 * Downsamples the input vector by keeping every other element starting from the
 * first.
 *
 * @param coeffs The input vector.
 * @param first The first element to keep.
 * @param last The last element to keep.
 * @return The downsampled vector.
 */
std::vector<double> downsample(const std::vector<double> coeffs,
                               const size_t first, const size_t last) {
  if (last < 2 * first) {
    throw std::runtime_error("last must larger than first!");
  }
  std::vector<double> downsampled;

  for (size_t i = first - 1; i < coeffs.size(); i += 2) {
    downsampled.push_back(coeffs[i]);
  }

  return downsampled;
}

/**
 * Performs a 1-D discrete wavelet transform of the input vector.
 *
 * @param signal The input vector.
 * @param wavelet_name The wavelet name, which can be "db5".
 * @param mode The extension mode, which can be "sym".
 * @return The wavelet coefficients.
 */
std::pair<std::vector<double>, std::vector<double>>
dwt(const std::vector<double> &signal, const std::string wavelet_name,
    const std::string mode) {

  std::vector<double> Lo_D;
  std::vector<double> Ho_D;

  if (wavelet_name == "db5") {
    // aquire db5 wavelet by matlab wfilters func
    Lo_D = {0.00333572528500155, -0.0125807519990155, -0.00624149021301171,
            0.0775714938400652,  -0.0322448695850295, -0.242294887066190,
            0.138428145901103,   0.724308528438574,   0.603829269797473,
            0.160102397974125};

    Ho_D = {-0.160102397974125,  0.603829269797473,    -0.724308528438574,
            0.138428145901103,   0.242294887066190,    -0.0322448695850295,
            -0.0775714938400652, -0.00624149021301171, 0.0125807519990155,
            0.00333572528500155};
  } else {
    throw std::runtime_error("Wavelet name must be db5!");
  }

  // define the extend length
  int extendLen = Lo_D.size() - 1;

  std::vector<double> cA(signal.size() + extendLen * 2);
  std::vector<double> cD(signal.size() + extendLen * 2);

  // extend the input signal
  std::vector<double> extendedInput = wextend(signal, extendLen, mode);

  // low pass filter to get the low frequency part
  cA = wconv1(extendedInput, Lo_D, "valid");

  // high pass filter to get the high frequency part
  cD = wconv1(extendedInput, Ho_D, "valid");

  // downsample the coefficients
  std::vector<double> downsampled_cA(cA.size() / 2);
  std::vector<double> downsampled_cD(cD.size() / 2);

  int first = 2;
  int last = signal.size() + extendLen;

  downsampled_cA = downsample(cA, first, last);
  downsampled_cD = downsample(cD, first, last);

  return std::make_pair(downsampled_cA, downsampled_cD);
}

//-------------------------------------------------------------
/**
 * Performs a 1-D discrete wavelet transform of the input vector.
 *
 * @param signal The input vector.
 * @param level The decomposition level.
 * @param wavelet_type The wavelet name, which can be "db5".
 * @return The wavelet coefficients.
 */
std::pair<std::vector<double>, std::vector<double>>
wavelet_decomposition(const std::vector<double> &signal, const size_t level,
                      const std::string wavelet_type) {
  std::vector<double> coeffs(signal.size());
  coeffs = signal;
  std::vector<double> all_cD;
  std::vector<double> cA, cD;
  std::vector<double> list;

  std::pair<std::vector<double>, std::vector<double>> wavedec_set;

  for (size_t i = 0; i < level; ++i) {
    std::pair<std::vector<double>, std::vector<double>> result =
        dwt(coeffs, wavelet_type, "sym");
    cA = result.first;
    cD = result.second;

    // record the length of cD
    list.push_back(cD.size());

    all_cD.insert(all_cD.begin(), cD.begin(), cD.end());
    // cA assign to coeffs,as the input of next level
    coeffs.assign(cA.begin(), cA.end());
  }

  // merge cA and cD
  for (const auto value : all_cD) {
    coeffs.push_back(value);
  }

  return std::make_pair(coeffs, list);
}

//-------------------------------------------------------------
