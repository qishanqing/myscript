//////////////////////////////////////////////////////////////////////
///  @file     main.cpp
///  @brief    unit test for module
///  Details.
///
///  @author   sunhao
///  @version  1.1.0
///  @date     2021.06.15
///
///  revision statement:
//////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "types.h"

// use global variable for every test case
InputParam inputParam;

GTEST_API_ int main(int argc, char **argv)
{
    std::cout << "running test for deeplearning." << std::endl;

    testing::InitGoogleTest(&argc, argv);
    // TODO : check the module which need param in the gtest_filter
    if (true)
    {
        inputParam.configPath = argv[1];
        inputParam.imagePath = argv[2];
        inputParam.annoPath = argv[3];
    }
    else
    {
        // Help()
    }

//    testing::GTEST_FLAG(filter) = "InstanceTest.Init";

    return RUN_ALL_TESTS();
}

