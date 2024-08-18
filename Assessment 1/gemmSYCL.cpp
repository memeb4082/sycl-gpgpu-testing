#include "header.h"

using namespace sycl;
namespace fs = std::filesystem;

std::vector<std::vector<double>> readMatrix(std::string path, int size)
{
  std::vector<std::vector<double>> output;
  std::ifstream file(path);
  std::string line, cell;
  if (!file.is_open())
  {
    std::cerr << "Error opening the file " << std::endl;
  }
  while (std::getline(file, line))
  {
    std::vector<double> row;
    std::stringstream lineStream(line);

    while (std::getline(lineStream, cell, ','))
    {
      row.push_back(std::stod(cell));
    }
    output.push_back(row);
  }
  file.close();
  return output;
}

bool assertMultiply(std::vector<std::vector<double>> C, int size)
{
  fs::path folder = ".test/size_" + std::to_string(size);
  auto matrixC = readMatrix(folder / "matrixC.csv", size);
  for (int i = 0; i < size; ++i)
  {
    for (int j = 0; j < size; ++j)
    {
      if (C[i][j] != matrixC[i][j])
      {
        std::cerr << "Error: C[" << i << "][" << j << "] = " << C[i][j] << " but expected " << matrixC[i][j] << std::endl;
        return false;
      }
    }
  }
  return true;
}
int main()
{
  sizeVect;
  for (int i = 0; i < sizes.size(); i++)
  {
    int size = sizes[i];
    fs::path folder = ".test/size_" + std::to_string(size);
    auto matrixA = readMatrix(folder / "matrixA.csv", size);
    auto matrixB = readMatrix(folder / "matrixB.csv", size);
    auto matrixC = readMatrix(folder / "matrixC.csv", size);
    queue q(property::queue::enable_profiling{});
    // Create buffers for matrices A, B, and C
    buffer A(matrixA);
    buffer B(matrixB);
    buffer<float, 2> C(range<2>(size, size));
    // Submit the kernel to the queue
    event e = q.submit([&](handler &h)
             {
      accessor a(A, h, read_only);
      accessor b(B, h, read_only);
      accessor c(C, h, write_only);

      h.parallel_for(range<2>(size, size), [=](id<2> idx) {
        float sum = 0;
        for (int k = 0; k < size; ++k)
        {
          sum += a[idx[0]][k] * b[k][idx[1]];
        }
        c[idx] = sum;
      }); });
    q.wait();
    host_accessor result(C, read_only);
    auto start = e.get_profiling_info<info::event_profiling::command_start>();
    auto end = e.get_profiling_info<info::event_profiling::command_end>();
    double duration_ms = (end - start) * 1e-6;
    std::cout << "Time taken for size " << size << ": " << duration_ms << "ms" << std::endl;
  }
}
