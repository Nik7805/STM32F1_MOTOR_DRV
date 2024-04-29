cmake_minimum_required(VERSION 3.17)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
 
set(TOOLCHAIN_PATH /usr/bin)
set(TOOLCHAIN_PREFIX arm-none-eabi-)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_AR ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}ar)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}objdump)
set(SIZE ${TOOLCHAIN_PATH}/${TOOLCHAIN_PREFIX}size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
 
# Add you project name
project(motor_drv C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
 
# Add carnel name
set(MCPU cortex-m3)
 
# Uncomment for hardware floating point
#add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
#add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
#add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
 
# Uncomment for software floating point
add_compile_options(-mfloat-abi=soft)
 
add_compile_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
 
# Uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")
 
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()
 
# Add Include directories
include_directories(
    #Includes from Makefile
        ${CMAKE_SOURCE_DIR}/Core/Inc 
        ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc 
        ${CMAKE_SOURCE_DIR}/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy 
        ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Device/ST/STM32F1xx/Include 
        ${CMAKE_SOURCE_DIR}/Drivers/CMSIS/Include
    #Includes from user code
        ${CMAKE_SOURCE_DIR}/UserCode/Inc
        )
 
# Add C defines
add_definitions(-DUSE_HAL_DRIVER -DSTM32F103xB)
 
# Add you source file
# file(GLOB_RECURSE SOURCES
#         "Core/Src/*.c"
#         "Drivers/STM32F1xx_HAL_Driver/Src/*.c"
#         "startup_stm32f103xb.s"
#         )
set(SOURCES 
#Sources from Makefile
Core/Src/main.c 
Core/Src/gpio.c 
Core/Src/tim.c 
Core/Src/stm32f1xx_it.c 
Core/Src/stm32f1xx_hal_msp.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c 
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_exti.c 
Core/Src/system_stm32f1xx.c  
startup_stm32f103xb.s
#Sources from user code
)
 
# Add lincer file
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103C8Tx_FLASH.ld)
 
# this options for C++
add_link_options(-specs=nosys.specs -lstdc++)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=${MCPU} -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})
 
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})
 
set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)
 
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")