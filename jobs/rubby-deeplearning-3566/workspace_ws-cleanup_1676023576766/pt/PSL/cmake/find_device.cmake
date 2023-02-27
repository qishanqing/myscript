# src file
if(USE_GPU)
    add_definitions(-DUSE_GPU)
    list(APPEND SRC_DIR ${ROOT_DIR}/src/module/gpu)
elseif(USE_NPU)
    add_definitions(-DUSE_NPU)
    list(APPEND SRC_DIR ${ROOT_DIR}/src/module/npu ${ROOT_DIR}/src/module/npu/vnn)
else()
    message(FATAL_ERROR "please set the device USE_GPU or USE_NPU")
endif()

# lib
set(NPU_LIBRARIES -lOpenVX -lOpenVXU -lArchModelSw -lCLC -lGAL -lNNArchPerf -lVSC -lovxlib)

if (USE_NPU)
    include_directories(/usr/share/npu/sdk/include/)
    set(CNN_LIBRARY ${NPU_LIBRARIES})
elseif(USE_GPU)
    include_directories(${MNN_DIR}/include)
    set(CNN_LIBRARY ${MNN_DIR}/lib/libMNN.so)
endif ()