#include "Utilities.h"

namespace Tools {
double GetStepPerYear(const IR::Frequency& freq) {
  switch (freq) {
    case IR::Frequency::Yearly: {
      return 1;
    } break;
    case IR::Frequency::SemiAnnually: {
      return 0.5;
    } break;
    case IR::Frequency::Quarterly: {
      return 0.25;
    } break;
    case IR::Frequency::Monthly: {
      return 1. / 12;
    } break;
    default:
      throw std::invalid_argument(
          "Does not recognize frequency other than yearly, semi-annually, and "
          "quarterly");
  }
  return 0;
}

double GetNumPerYear(const IR::Frequency& freq) {
  auto step = GetStepPerYear(freq);
  return 1 / step;
}

namespace Csv {
void ToFile(const std::string& filename,
            const std::vector<std::vector<std::string>>& data,
            const std::string& separator, bool transpose,
            std::string& errormessage, bool append) {
  errormessage = "";
  if (data.size() == 0) {
    errormessage = "Input data is empty";
  }

  if (append) {
    std::ofstream outputfile(filename, std::ios_base::app);
    if (transpose)  // must be same columns!
    {
      for (int j = 0; j < static_cast<int>(data[0].size()); j++) {
        for (int i = 0; i < static_cast<int>(data.size()) - 1; i++) {
          outputfile << (data[i][j]);
          outputfile << separator;
        }
        outputfile << (data[data.size() - 1][j]);
        if (j < static_cast<int>(data[0].size()) - 1) outputfile << "\n";
      }
    } else {
      for (int i = 0; i < static_cast<int>(data.size()); i++) {
        for (int j = 0; j < static_cast<int>(data[i].size()) - 1; j++) {
          outputfile << (data[i][j]);
          outputfile << separator;
        }
        outputfile << (data[i][data[i].size() - 1]);
        if (i < static_cast<int>(data.size()) - 1) outputfile << "\n";
      }
    }
    outputfile << "\n";
    outputfile.close();
  } else {
    std::ofstream outputfile(filename);

    if (transpose)  // must be same columns!
    {
      for (int j = 0; j < static_cast<int>(data[0].size()); j++) {
        for (int i = 0; i < static_cast<int>(data.size()) - 1; i++) {
          outputfile << (data[i][j]);
          outputfile << separator;
        }
        outputfile << (data[data.size() - 1][j]);
        if (j < static_cast<int>(data[0].size()) - 1) outputfile << "\n";
      }
    } else {
      for (int i = 0; i < static_cast<int>(data.size()); i++) {
        for (int j = 0; j < static_cast<int>(data[i].size()) - 1; j++) {
          outputfile << (data[i][j]);
          outputfile << separator;
        }
        outputfile << (data[i][data[i].size() - 1]);
        if (i < static_cast<int>(data.size()) - 1) outputfile << "\n";
      }
    }
    outputfile.close();
  }
  return;
}

void ToFile(const std::string& fileName,
            const std::map<std::string, std::vector<double>>& data,
            const std::string& seperator) {
  if (data.empty()) {
    return;
  }
  std::vector<std::vector<std::string>> outputContent(
      data.size(), std::vector<std::string>(data.begin()->second.size() + 1));
  std::vector<std::string> headers;
  for (auto it : data) {
    headers.push_back(it.first);
  }
  for (std::size_t i = 0; i < headers.size(); i++) {
    outputContent[i][0] = headers[i];
    for (std::size_t j = 0; j < data.at(headers[i]).size(); j++) {
      outputContent[i][j + 1] = std::to_string(data.at(headers[i])[j]);
    }
  }
  std::string errormessage;
  ToFile(fileName, outputContent, seperator, true, errormessage);
  if (errormessage != "") {
    throw errormessage;
  }
}
}  // namespace Csv
}  // namespace Tools