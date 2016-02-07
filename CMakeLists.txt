cmake_minimum_required(VERSION 3.2)
project(ser2neo C CXX ASM)

set(LPCOPEN third_party/lpcopen)

add_library(
  lpcopen
  ${LPCOPEN}/lpc_chip_8xx/src/acmp_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/chip_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/clock_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/crc_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/gpio_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/i2c_common_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/i2cm_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/i2cs_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/iap.c
  ${LPCOPEN}/lpc_chip_8xx/src/iocon_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/irc_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/pinint_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/pmu_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/ring_buffer.c
  ${LPCOPEN}/lpc_chip_8xx/src/sct_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/sct_pwm_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/spi_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/spim_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/spis_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/stopwatch_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/swm_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/syscon_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/sysinit_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/uart_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/wkt_8xx.c
  ${LPCOPEN}/lpc_chip_8xx/src/wwdt_8xx.c
  )

set(MBED_LPC81X third_party/mbed/libraries/mbed/targets/cmsis/TARGET_NXP/TARGET_LPC81X)

set(STARTUP ${MBED_LPC81X}/TOOLCHAIN_GCC_ARM/startup_LPC81X.S)
set(LDSCRIPT ${MBED_LPC81X}/TARGET_LPC810/TOOLCHAIN_GCC_ARM/LPC810.ld)

include_directories(
  ${LPCOPEN}
  ${LPCOPEN}/lpc_chip_8xx/inc
  .
  )

add_executable(
  ser2neo
  ser2neo.cc
  serial.cc
  neopixel.cc
  retarget.cc
  ${STARTUP}
  )

target_link_libraries(ser2neo lpcopen)

set(EXTRA_FLAGS "-Wall -Os -flto --specs=nano.specs --specs=nosys.specs -DCORE_M0PLUS -Wl,-T${CMAKE_SOURCE_DIR}/${LDSCRIPT},-gc-sections -fno-builtin-printf -fno-builtin-putchar -fno-builtin-puts -Wl,-Map,ser2neo.map,--cref")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${RUNTIME_FLAGS} ${EXTRA_FLAGS} -Wno-incompatible-pointer-types")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${RUNTIME_FLAGS} ${EXTRA_FLAGS} -fno-exceptions -fno-rtti -std=gnu++14 -save-temps=obj")
