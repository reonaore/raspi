#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <signal.h>
#include <math.h>

#include <native/task.h>
#include <native/timer.h>
#include <rtdm/rtdm.h>
#include <rtdk.h>

#include "gpio.h"

#define TIME_MS 1000000 //1 msec
#define TIME_S 1000000000 // 1 sec
#define MAX_PIN 24
#define PIN 23 

#define BCM2708_PERI_BASE        0x20000000
#define GPIO_BASE                (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

#define NUM 5000

RT_TASK gpio_test;

int rpi_gpio(void *cookie)
{
  unsigned int flag = 1;
  int err;
  double load[2000] = {0.0};
  unsigned int counter = 0;
 
  for(counter = 0; counter < 2000; counter++){
    load[counter] = sin((double)255);
  }

  rt_task_set_periodic(NULL, TM_NOW, TIME_MS);
  err = gpio_init();
  if(err == -1){
    return -1;
  }
  err = gpio_set_mode(PIN, GPIO_OUT);
  while(1){
    rt_task_wait_period(NULL);
    for(counter = 0; counter < NUM; counter++){
      load[0] = load[0] * load[0];
      load[0] = load[0] / 3.141593;
    }

    if(flag){
      err = gpio_output_clear(PIN);
      flag = 0;
    }else if(flag == 0){
      err = gpio_output_set(PIN);
      flag = 1;
    }
    if(err == -1){
      printf("error: gpio out or in \n");
      return -1;
    }
  }
  return 0;
}

int main(int argc, char* argv[])
{
  int err;
  
  //Perform auto-init of rt_printf buffers if the task doesn't do so
  rt_print_auto_init(1);

  // Avoid memory swapping for this program
  mlockall(MCL_CURRENT| MCL_FUTURE);

  err = rt_task_create(&gpio_test, "raspi gpio test", 0, 99, 0);
  if(!err){
    printf("rt_task_create success!\n");
    err = rt_task_start(&gpio_test, &rpi_gpio, NULL);
    if(!err)
      printf("rt_task start!\n");
  }else{
    fprintf(stderr, "Failed to creat rt_task!\n");
    return -1;
  } 
  printf("if you want to stop this rt_task, please push any key...\n");
  getchar();
  printf("task finished\n");
  return 0;
}



