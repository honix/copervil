#include <iostream>
#include <cstdlib>

extern "C"
{
#include "core/node_api.h"

    void cpp_test_init(struct node *node)
    {
        std::cout << "CPP INIT!" << std::endl;

        init_pins(node, true, 0, 0);
    }

    void cpp_test_deinit(struct node *node)
    {
        std::cout << "CPP DEINIT!" << std::endl;
    }

    void cpp_test(struct node *node)
    {
        std::cout << "CPP HELLO!" << std::endl;
    }

    void register_library()
    {
        register_function((struct function_note){
            "cpp_test", cpp_test_init, cpp_test, cpp_test_deinit});
    }
}