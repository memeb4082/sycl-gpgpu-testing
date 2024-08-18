#include <sycl/sycl.hpp>
using namespace sycl;
int main() {
    queue q;
    std::cout << "Running on " << q.get_device().get_info<info::device::name>() << "\n";
    q.submit([&](handler& cg) {
        auto os = stream{1024, 1024, cg};
        cg.parallel_for(10, [=](id<1> myid) {
            os << "Hello World! My ID is " << myid << "\n";
        });
    });
}