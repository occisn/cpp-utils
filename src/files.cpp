#include <stdexcept>
#include <string>
#include <fstream>

/**
 * Counts the number of non-empty data lines in a file.
 *
 * This function is used for memory pre-allocation to improve performance
 * by avoiding multiple vector reallocations during file reading.
 * Empty lines are ignored, and the header line is excluded from the count
 * if skipHeader is true.
 *
 * @param filename Path to the file to count lines in
 * @param skipHeader If true, subtracts 1 from the count to exclude the header line
 * @return size_t Number of data lines that will be read (excluding header if specified)
 * @throws std::runtime_error if the file cannot be opened
 *
 * Example:
 *   A file with 1 header line + 1000 data lines + 5 empty lines:
 *   countLines("data.csv", true)  returns 1000
 *   countLines("data.csv", false) returns 1001
 *
 * (v1, available in occisn/cpp-utils GitHub repository, 2025-12-16)
 */
size_t count_lines(const std::string &filename, bool skipHeader)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  size_t count = 0;
  std::string line;
  while (std::getline(file, line)) {
    if (!line.empty())
      count++;
  }

  if (skipHeader && count > 0)
    count--;
  return count;
}
