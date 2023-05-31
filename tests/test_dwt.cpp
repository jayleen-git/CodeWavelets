#include <iostream>
#include <string>
#include <vector>
#define CATCH_CONFIG_MAIN // 告诉Catch2生成main函数
// #include "catch_amalgamated.hpp" // Include the Catch2 header
#include "../dwt.h"
#include <catch.hpp>
#include <random>
#include <utility>

TEST_CASE("test wextend func", "[wextend]") {
  // Test case 1
  SECTION("Symmetric mode") {
    std::vector<double> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int extendLen = 5;
    std::string mode = "sym";
    std::vector<double> expectedOutput = {5, 4, 3, 2, 1,  1,  2, 3, 4, 5,
                                          6, 7, 8, 9, 10, 10, 9, 8, 7, 6};
    std::vector<double> output = wextend(input, extendLen, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("Invalid mode") {
    std::vector<double> input = {1, 2, 3};
    int extendLen = 2;
    std::string mode = "invalid_mode";
    REQUIRE_THROWS_AS(wextend(input, extendLen, mode),
                      const std::runtime_error &);
  }

  SECTION("Symmetric mode") {
    std::vector<double> input = {1, 2, 3};
    int extendLen = 2;
    std::string mode = "sym";
    std::vector<double> expectedOutput = {2, 1, 1, 2, 3, 3, 2};
    std::vector<double> output = wextend(input, extendLen, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("Test extenLen = 0") {
    std::vector<double> input = {1, 2, 3};
    int extendLen = 0;
    std::string mode = "sym";
    std::vector<double> expectedOutput = {1, 2, 3};
    std::vector<double> output = wextend(input, extendLen, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("Test input empty") {
    std::vector<double> input = {};
    int extendLen = 2;
    std::string mode = "sym";
    std::vector<double> expectedOutput = {};
    std::vector<double> output = wextend(input, extendLen, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("Test input = {0}") {
    std::vector<double> input = {0};
    int extendLen = 2;
    std::string mode = "sym";
    REQUIRE_THROWS_AS(wextend(input, extendLen, mode),
                      const std::runtime_error &);
  }

  SECTION("Test input same number") {
    std::vector<double> input = {1, 1, 1, 1, 1};
    int extendLen = 2;
    std::string mode = "sym";
    std::vector<double> expectedOutput = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    std::vector<double> output = wextend(input, extendLen, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("Test input < extendLen") {
    std::vector<double> input = {1, 2};
    int extendLen = 5;
    std::string mode = "sym";
    REQUIRE_THROWS_AS(wextend(input, extendLen, mode),
                      const std::runtime_error &);
  }
}

TEST_CASE("test wconv1 func", "[wconv1]") {
  std::string mode = "full";
  // Test case 1
  SECTION("Original signal and filter") {
    std::vector<double> input = {1, 2, 3};
    std::vector<double> wfilters = {1, 2, 3};
    std::vector<double> expectedOutput = {1, 4, 10, 12, 9};
    std::vector<double> output = wconv1(input, wfilters, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("Sysmetric extension ") {
    std::vector<double> input = {1, 2, 3};
    std::vector<double> wfilters = {1, 2, 3};
    std::vector<double> extendedInput = wextend(input, 2, "sym");
    std::vector<double> expectedOutput = {2, 5, 9, 7, 10, 15, 17, 13, 6};
    std::vector<double> output = wconv1(extendedInput, wfilters, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("wfilter = NULL") {
    std::vector<double> input = {1, 2, 3};
    std::vector<double> wfilters = {};
    // 使用引用方式捕获异常对象
    REQUIRE_THROWS_AS(wconv1(input, wfilters, mode),
                      const std::runtime_error &);
  }

  SECTION("input = NULL") {
    std::vector<double> input = {};
    std::vector<double> wfilters = {1, 2, 3};
    REQUIRE_THROWS_AS(wconv1(input, wfilters, mode),
                      const std::runtime_error &);
  }

  SECTION("wfilter = {0}") {
    std::vector<double> input = {1, 2, 3};
    std::vector<double> wfilters = {0};
    std::vector<double> expectedOutput = {0, 0, 0};
    std::vector<double> output = wconv1(input, wfilters, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("wfilter.size() > input.size()") {
    std::vector<double> input = {1, 2, 3};
    std::vector<double> wfilters = {1, 2, 3, 4};
    std::vector<double> expectedOutput = {1, 4, 10, 16, 17, 12};
    std::vector<double> output = wconv1(input, wfilters, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("wfilter.size() < input.size()") {
    std::vector<double> input = {1, 2, 3, 4, 5};
    std::vector<double> wfilters = {1, 2, 3};
    std::vector<double> expectedOutput = {1, 4, 10, 16, 22, 22, 15};
    std::vector<double> output = wconv1(input, wfilters, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("test valid mode --input.size() = wfilters.size()") {
    std::vector<double> input = {1, 2, 3};
    std::vector<double> wfilters = {1, 2, 3};
    std::string mode = "valid";
    std::vector<double> expectedOutput = {10};
    std::vector<double> output = wconv1(input, wfilters, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("test valid mode -- input.size() < wfilters.size()") {
    std::vector<double> input = {1, 2, 3};
    std::vector<double> wfilters = {1, 2, 3, 4, 5};
    std::string mode = "valid";
    std::vector<double> expectedOutput = {};
    std::vector<double> output = wconv1(input, wfilters, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("test valid mode -- input.size() > wfilters.size()") {
    std::vector<double> input = {1, 2, 3, 4, 5};
    std::vector<double> wfilters = {1, 2, 3};
    std::string mode = "valid";
    std::vector<double> expectedOutput = {10, 16, 22};
    std::vector<double> output = wconv1(input, wfilters, mode);
    REQUIRE(output == expectedOutput);
  }

  SECTION("test valid mode -- input.size() > wfilters.size()") {
    std::vector<double> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<double> wfilters = {1, -1, -3};
    std::string mode = "valid";
    std::vector<double> expectedOutput = {-2, -5, -8, -11, -14, -17, -20, -23};
    std::vector<double> output = wconv1(input, wfilters, mode);
    REQUIRE(output == expectedOutput);
  }
}

TEST_CASE("test downsample func") {
  SECTION("test downsample") {
    std::vector<double> coeff = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<double> expectedOutput = {2, 4, 6, 8, 10};
    int first = 2;
    int last = 5;
    std::vector<double> output = downsample(coeff, first, last);
    REQUIRE(output == expectedOutput);
  }

  SECTION("last < first") {
    std::vector<double> coeff = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int first = 5;
    int last = 2;
    REQUIRE_THROWS_AS(downsample(coeff, first, last),
                      const std::runtime_error &);
  }
}

TEST_CASE("test dwt func", "[dwt]") {
  std::string wavelet_name = "db5";
  std::string mode = "sym";
  int len = 32;
  std::vector<double> signal(len);
  for (int i = 0; i < len; ++i) {
    signal[i] = i + 1;
  }
  std::pair<std::vector<double>, std::vector<double>> coeffs =
      dwt(signal, wavelet_name, mode);
  std::vector<double> cA = coeffs.first;
  std::vector<double> cD = coeffs.second;
  SECTION("test 1 to 32 cA") {
    std::vector<double> expected_cA = {
        9.62269401153053, 6.81613380312931, 3.89701709021400, 1.35836381265697,
        3.10265447368332, 5.93108159842951, 8.75950872317570, 11.5879358479219,
        14.4163629726681, 17.2447900974143, 20.0732172221605, 22.9016443469067,
        25.7300714716528, 28.5584985963990, 31.3869257211452, 34.2153528458914,
        37.0463535467816, 39.8529137551828, 42.7720304680982, 45.3106837456552};
    REQUIRE(cA.size() == 20);
    for (size_t i = 0; i < cA.size(); i++) {
      REQUIRE(cA[i] == Approx(expected_cA[i]).epsilon(0.00001));
    }
  }

  SECTION("test 1 to 32 cD") {
    std::vector<double> expected_cD = {
        0.123522075866033,     -0.136067876251151,    -0.0100421274900581,
        0.0225879278489292,    -9.13522252635191e-12, -1.31803071061476e-11,
        -1.72244549352663e-11, -2.12688664423533e-11, -2.53139579610429e-11,
        -2.93581613013127e-11, -3.34031383282529e-11, -3.74465679818525e-11,
        -4.14917739922915e-11, -4.55365359135207e-11, -4.95818945796256e-11,
        -5.36248801440153e-11, -0.123522075932768,    0.136067876184417,
        0.0100421274233231,    -0.0225879279156650};
    REQUIRE(cD.size() == 20);
    for (size_t i = 0; i < cA.size(); i++) {
      REQUIRE(cD[i] == Approx(expected_cD[i]).epsilon(0.00001));
    }
  }

  SECTION("Sine wave dwt") {
    int numSamples = 100;        // 信号样本数量
    double amplitude = 1.0;      // 正弦信号振幅
    double frequency = 10.0;     // 正弦信号频率
    double noiseAmplitude = 0.2; // 随机噪声振幅
    std::vector<double> signal(numSamples);
    // 生成正弦信号
    for (int i = 0; i < numSamples; ++i) {
      double time = static_cast<double>(i) / numSamples;
      signal[i] = amplitude * std::sin(2 * M_PI * frequency * time);
    }

    std::pair<std::vector<double>, std::vector<double>> coeffs =
        dwt(signal, wavelet_name, mode);
    std::vector<double> cA = coeffs.first;
    std::vector<double> cD = coeffs.second;

    std::vector<double> expected_cA = {
        -0.639369213467517,  1.02055503874477,   1.19377131982088,
        -0.0358135086715916, 1.00336121585919,   1.25721868906570,
        -0.226356860170171,  -1.39711545443192,  -0.637107590322798,
        1.00336121585919,    1.25721868906570,   -0.226356860170171,
        -1.39711545443192,   -0.637107590322798, 1.00336121585919,
        1.25721868906570,    -0.226356860170171, -1.39711545443192,
        -0.637107590322798,  1.00336121585919,   1.25721868906570,
        -0.226356860170171,  -1.39711545443192,  -0.637107590322798,
        1.00336121585919,    1.25721868906570,   -0.226356860170170,
        -1.39711545443192,   -0.637107590322799, 1.00336121585919,
        1.25721868906570,    -0.226356860170172, -1.39711545443192,
        -0.637107590322799,  1.00336121585919,   1.25721868906570,
        -0.226356860170172,  -1.39711545443192,  -0.637107590322799,
        1.00336121585919,    1.25721868906570,   -0.226356860170168,
        -1.39711545443192,   -0.637107590322799, 1.00336121585919,
        1.25721868906570,    -0.226356860170168, -1.39711545443192,
        -0.637107590322799,  1.00336121585919,   1.25948031221042,
        -0.243550683055746,  -1.33366808518710,  -0.827650941821377};
    std::vector<double> expected_cD = {
        0.130867136517937,     -0.0949973393824321,   -0.0483977286213811,
        0.0348455749560919,    0.0360532440103088,    -3.55316892421639e-05,
        -0.0360744762852904,   -0.0222608795091048,   0.0223176434733281,
        0.0360532440103088,    -3.55316892423495e-05, -0.0360744762852904,
        -0.0222608795091049,   0.0223176434733279,    0.0360532440103089,
        -3.55316892425351e-05, -0.0360744762852904,   -0.0222608795091049,
        0.0223176434733277,    0.0360532440103089,    -3.55316892426652e-05,
        -0.0360744762852904,   -0.0222608795091049,   0.0223176434733275,
        0.0360532440103090,    -3.55316892428508e-05, -0.0360744762852905,
        -0.0222608795091043,   0.0223176434733300,    0.0360532440103082,
        -3.55316892427606e-05, -0.0360744762852905,   -0.0222608795091044,
        0.0223176434733298,    0.0360532440103083,    -3.55316892428907e-05,
        -0.0360744762852906,   -0.0222608795091056,   0.0223176434733244,
        0.0360532440103100,    -3.55316892436280e-05, -0.0360744762852905,
        -0.0222608795091056,   0.0223176434733243,    0.0360532440103101,
        -3.55316892438136e-05, -0.0360744762852906,   -0.0222608795091056,
        0.0223176434733241,    0.0360532440103102,    -0.108585024733852,
        0.0949761071074506,    0.0261013174230378,    -0.0486024077680532};

    // 检查长度
    REQUIRE(cA.size() == expected_cA.size());
    REQUIRE(cD.size() == expected_cD.size());

    // 逐一比较
    for (size_t i = 0; i < cA.size(); i++) {
      REQUIRE(cA[i] == Approx(expected_cA[i]).epsilon(0.00001));
      REQUIRE(cD[i] == Approx(expected_cD[i]).epsilon(0.00001));
    }
  }
}

TEST_CASE("test wavedec func") {
  std::string wavelet_name = "db5";
  std::string mode = "sym";

  SECTION("test 1") {
    std::vector<double> signal(32, 1.0);
    int level = 5;
    std::pair<std::vector<double>, std::vector<double>> wavedec_set =
        wavelet_decomposition(signal, level, wavelet_name);
    std::vector<double> expected_coeffs = {
        5.65685424949239,      5.65685424949239,      5.65685424949239,
        5.65685424949239,      5.65685424949239,      5.65685424949239,
        5.65685424949239,      5.65685424949239,      5.65685424949239,
        -8.08897473403165e-12, -8.08897473403165e-12, -8.08897473403165e-12,
        -8.08897473403165e-12, -8.08897473403165e-12, -8.08897473403165e-12,
        -8.08897473403165e-12, -8.08897473403165e-12, -8.08897473403165e-12,
        -5.71991602581166e-12, -5.71991602581166e-12, -5.71991602581166e-12,
        -5.71991602581166e-12, -5.71991602581166e-12, -5.71991602581166e-12,
        -5.71991602581166e-12, -5.71991602581166e-12, -5.71991602581166e-12,
        -5.71991602581166e-12, -4.04464706884574e-12, -4.04464706884574e-12,
        -4.04464706884574e-12, -4.04464706884574e-12, -4.04464706884574e-12,
        -4.04464706884574e-12, -4.04464706884574e-12, -4.04464706884574e-12,
        -4.04464706884574e-12, -4.04464706884574e-12, -4.04464706884574e-12,
        -2.85992714851726e-12, -2.85992714851726e-12, -2.85992714851726e-12,
        -2.85992714851726e-12, -2.85992714851726e-12, -2.85992714851726e-12,
        -2.85992714851726e-12, -2.85992714851726e-12, -2.85992714851726e-12,
        -2.85992714851726e-12, -2.85992714851726e-12, -2.85992714851726e-12,
        -2.85992714851726e-12, -2.85992714851726e-12, -2.02227861192950e-12,
        -2.02227861192950e-12, -2.02227861192950e-12, -2.02227861192950e-12,
        -2.02227861192950e-12, -2.02227861192950e-12, -2.02227861192950e-12,
        -2.02227861192950e-12, -2.02227861192950e-12, -2.02227861192950e-12,
        -2.02227861192950e-12, -2.02227861192950e-12, -2.02227861192950e-12,
        -2.02227861192950e-12, -2.02227861192950e-12, -2.02227861192950e-12,
        -2.02227861192950e-12, -2.02227861192950e-12, -2.02227861192950e-12,
        -2.02227861192950e-12};

    // 检查长度
    REQUIRE(wavedec_set.first.size() == expected_coeffs.size());

    // 逐一比较
    for (size_t i = 0; i < wavedec_set.first.size(); i++) {
      INFO("Checking index : " << i);
      REQUIRE(wavedec_set.first[i] ==
              Approx(expected_coeffs[i]).epsilon(0.00001));
    }
  }
  SECTION("test 1 to 32") {

    int len = 32;
    std::vector<double> signal(len);
    for (int i = 0; i < len; ++i) {
      signal[i] = i + 1;
    }

    int level = 5;
    std::pair<std::vector<double>, std::vector<double>> wavedec_set =
        wavelet_decomposition(signal, level, wavelet_name);

    std::vector<double> expected_coeffs = {
        25.9044992472043,      21.5921925861930,      23.1350706403862,
        24.4550924619225,      22.1529704476436,      28.0562363769563,
        4.25183004388005,      95.9773555423089,      188.354341567029,
        0.309020227131436,     2.00861535364172,      1.30282393694265,
        -9.88041656866324,     22.4088118951722,      -8.68364822431269,
        -17.8440975212317,     20.5297392588335,      -9.98921888735898,
        -0.582449650819383,    -7.67537918807974,     -3.84719425508619,
        -6.84409995075414,     1.90375692052575,      0.0856365924779228,
        -3.55648710304517,     3.17176027440347,      8.62931969579327,
        1.71775452051919,      -0.827757156484661,    3.42858628856041,
        -5.14940228208002,     4.87577894330846,      -1.89818894155896,
        0.260091553712442,     0.432371433000033,     -0.908107464747659,
        0.508720633450225,     0.213201554439236,     -0.198972862253252,
        -0.535483543725539,    0.390510273166383,     1.08527636390249,
        -0.673981195452495,    0.0885947754091511,    0.0351587391872139,
        -2.63925248008209e-11, -3.78350210955159e-11, 0.00464291058564161,
        0.0924045077388710,    -0.329368341312049,    0.237120247016297,
        0.0644692418934177,    -0.0693271404472271,   0.123522075866033,
        -0.136067876251151,    -0.0100421274900581,   0.0225879278489292,
        -9.13522252635191e-12, -1.31803071061476e-11, -1.72244549352663e-11,
        -2.12688664423533e-11, -2.53139579610429e-11, -2.93581613013127e-11,
        -3.34031383282529e-11, -3.74465679818525e-11, -4.14917739922915e-11,
        -4.55365359135207e-11, -4.95818945796256e-11, -5.36248801440153e-11,
        -0.123522075932768,    0.136067876184417,     0.0100421274233231,
        -0.0225879279156650};
    // 检查长度
    REQUIRE(wavedec_set.first.size() == expected_coeffs.size());

    // 逐一比较
    for (size_t i = 0; i < wavedec_set.first.size(); i++) {
      INFO("Checking index : " << i);
      REQUIRE(wavedec_set.first[i] ==
              Approx(expected_coeffs[i]).epsilon(0.00001));
    }
  }
}