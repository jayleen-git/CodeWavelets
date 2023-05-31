#ifndef dwt_h
#define dwt_h

#include <string>
#include <vector>

std::vector<double> wextend(const std::vector<double> &input, int extendLen,
                            const std::string &mode);

std::vector<double> wconv1(const std::vector<double> &input,
                           const std::vector<double> &wfilters,
                           const std::string &mode);

std::vector<double> downsample(const std::vector<double> coeffs,
                               const size_t first, const size_t last);

std::pair<std::vector<double>, std::vector<double>>
dwt(const std::vector<double> &signal, const std::string wavelet_name,
    const std::string mode);

std::pair<std::vector<double>, std::vector<double>>
wavelet_decomposition(const std::vector<double> &signal, const size_t level,
                      const std::string wavelet_type);

#endif /* dwt_h */