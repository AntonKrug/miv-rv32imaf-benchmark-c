/*******************************************************************************
 * (c) Copyright 2016-2017 Microsemi SoC Products Group. All rights reserved.
 * v1.0 2017/12/19 anton.krug@microsemi.com
 * Using UART at 115200 baudrate with MiV Core running at 50MHz
 */

#include <stdio.h>
#include "riscv_hal.h"
#include "hw_platform.h"

#include "mandelbrot.h"

volatile uint32_t tick_current;

void SysTick_Handler(void) {
	tick_current++;
}

uint32_t test_raytracer() {
  uint32_t expectedDebug   = 709576;
  uint32_t expectedRelease = 709576;
  uint32_t actual;
  uint32_t start;
  uint32_t end;
  uint32_t pass;


  start = tick_current;
  actual = allMandebrotSets();
  end = tick_current;

  if ( (expectedDebug != actual) &&  (expectedRelease != actual) ) {
	pass = 0;
  }
  else {
	pass = 1;
  }

  printf("Mandelbrot checksum(%d)= %s, time=%d(ms)\n", actual, (pass) ? "PASS" : "FAIL", end - start);
  return end - start;
}


int main() {
  uint32_t tick_total = 0;

  printf("\nFloating C benchmark started\n");

  SysTick_Config(SYS_CLK_FREQ/1000);
  tick_total += test_raytracer();
  printf("Finished in final time %d(ms). Lower number is better.\n", tick_total);

  return 0;
}
