#include <sycl/sycl.hpp>


int main() {

    // Create a buffer of 4 ints to be used inside kernel code
    sycl::buffer<int, 1> Buffer{4};
    
    // Create sycl queue
    sycl::queue Queue{};

    // Size of index space for kernel
    sycl::range<1> NumOfWorkItems{Buffer.size()};

    // Submit command group(work) to queue
    Queue.submit([&](sycl::handler &cgh) {
        auto Accessor = Buffer.get_access<sycl::access::mode::write>(cgh);
        cgh.parallel_for<class FillBuffer>(
            NumOfWorkItems, [=](sycl::id<1> WIid) {
                // Fill buffer with indices
                Accessor[WIid] = static_cast<int>(WIid.get(0));
            });
    });

    // Getting readonly access to the buffer on the host
    // Implicit barrier waiting for queue to complete the work
    auto HostAccessor = Buffer.get_host_access();

    // Check results
    bool MisMatchFound{false};
    for (size_t I{0}; I < Buffer.size(); ++I) {
        if (HostAccessor[I] != I) {
            std::cout << "The result is incorrect for element: " << I
            << ", expected: " << I << ", got " << HostAccessor[I] <<
            std::endl;
            MisMatchFound = true;
        }
    }
    if (!MisMatchFound) {
        std::cout << "The results are correct!" << std::endl;
    }
    return MisMatchFound;
}