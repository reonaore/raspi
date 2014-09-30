#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>


#include "gpio.h"
#define BUF 255 


int fd;
void* gpio_fd;


int gpio_init()
{

  fd = open("/dev/mem", O_RDWR | O_SYNC);
  if (fd == -1){
    printf("error: cannot open /dev/mem\n");
    return -1;
  }else printf("success!: open /dev/mem\n");
  
  gpio_fd = mmap(NULL,
		 BLOCK_SIZE,
		 PROT_READ | PROT_WRITE,
		 MAP_SHARED,
		 fd,
		 GPIO_BASE);
    if( gpio_fd == MAP_FAILED){
      printf("error: mmap failed\n");
      return -2;
    }else printf("success!: mmap\n");
    close(fd);

    gpio = (volatile unsigned *)gpio_fd;
    //printf("debub: mmap gpio: address = %p\n",gpio);
    return 0;
}

int gpio_set_mask(int pin)
{
  //check the pin numper
  //pin 0 ~ 26
  if(pin < 0 || pin > 26){
    printf("error: unable pin number; pin = %d\n",pin);
    return -1;
  }else
    return ~(0x07 << (pin % 10) * 3);
}

int gpio_set_mode(int pin, int mode)
{
  int mask = 0;  
  int index = -1;
  int i = 0;
  int offset = 0;

  
  if(pin < 0 || pin > 26){
    printf("error: unable pin number; pin = %d\n",pin);
    return -1;
  }

  for(i = 0; i <= 3; i++){
    if(pin >= (10 * i) && pin < (10 * (i+1))){
      index = i;
      //printf("success: gpio_set_mode: index = %d\n", index);
    }
  }

  if(index == -1){
    printf("error: gpio_set_mode: cannot assign the index\n");
    return -1;
  }
  offset = (pin - (index * 10)) * 3;
  mask = ~(0x07 << offset);
  //printf("debug: gpio_set_mode: mask = %d, offset = %d\n", mask, offset);
  gpio[index] = (mask && gpio[index]) | (mode << offset);
  //rt_printf("debug: gpio_set_mode: gpio[%d] = %d\n",index, gpio[index]);
  return 0;
}  

int gpio_output_set(int pin)
{
  gpio[7] = gpio[7] | 0x1 << pin;

  return 0;
}

int gpio_output_level(int pin, int level)
{
  if(level){
    gpio[13] = gpio[13] | 0x1 << pin;
  }else if (level == 0){
    gpio[13] = gpio[13] & ~(0x1 << pin);
  }else{
    printf("error: unable level = %d\n", level);
  }

  return 0;
}

int gpio_output_clear(int pin)
{
  gpio[10] = 0x1 << pin;

  return 0;
}

int gpio_set_mode_out(int pin)
{
  return gpio_set_mode(pin,GPIO_OUT);
}

int gpio_set_mode_in(int pin)
{
  return gpio_set_mode(pin,GPIO_IN);
}

int gpio_set_mode_alt_func0(int pin)
{
  return gpio_set_mode(pin,GPIO_ALT_FUNC0);
}

int gpio_set_mode_alt_func1(int pin)
{
  return gpio_set_mode(pin,GPIO_ALT_FUNC1);
}

int gpio_set_mode_alt_func2(int pin)
{
  return gpio_set_mode(pin,GPIO_ALT_FUNC2);
}

int gpio_set_mode_alt_func3(int pin)
{
  return gpio_set_mode(pin,GPIO_ALT_FUNC3);
}

int gpio_set_mode_alt_func4(int pin)
{
  return gpio_set_mode(pin,GPIO_ALT_FUNC4);
}

int gpio_set_mode_alt_func5(int pin)
{
  return gpio_set_mode(pin,GPIO_ALT_FUNC5);
}

int gpio_export(int pin)
{
  char buf[BUF];
  ssize_t byte_written;
  int fd = 0;

  fd = open("/sys/class/gpio/export",O_WRONLY);
  if(fd < 0){
    fprintf(stderr, "Failed to open export for writing!\n");
    return (-1);
  }

  byte_written = snprintf(buf, BUF, "%d", pin);
  write(fd, buf, byte_written);
  close(fd);
  return (0);
}

int gpio_unexport(int pin)
{
  char buf[BUF];
  ssize_t byte_written;
  int fd = 0;

  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if(fd < 0){
    fprintf(stderr, "Failed to open unexport for writing!\n");
    return (-1);
  }

  byte_written = snprintf(buf, BUF, "%d", pin);
  write(fd, buf, byte_written);
  close(fd);
  return (0);

}

int gpio_setup(int max_pin_number)
{
  int err;
  int i;
  for(i = 0; i < max_pin_number;i++){
    err = gpio_export(i);
    if(err < 0){
      fprintf(stderr, "gpio_setup is failed\n");
      return (-1);
    }
  }
    return (0);
}

int gpio_unset(int max_pin_number)
{
  int err;
  int i;
  for(i = 0; i < max_pin_number;i++){
    err = gpio_unexport(i);
    if(err == -1){
      fprintf(stderr, "gpio_setup is failed\n");
      return (-1);
    }
  }
    return (0);
}

int gpio_direction(int direction, int pin)
{
  char path[BUF];
  char dire_string[BUF];
  ssize_t byte_written;
  int fd;
  int err;

  snprintf(path, BUF, "/sys/class/gpio/gpio%d/direction", pin);
  fd = open(path, O_WRONLY);
  if(fd < 0){
    fprintf(stderr, "Failed to open gpio direction for written!\n");
    return (-1);
  }

  switch(direction){
  case IN:
    byte_written = snprintf(dire_string, BUF, "in");
    err = write(fd, "in", byte_written);
    if(err < 0){
      fprintf(stderr, "Failed to open export for writing!\n");
      return (-1);
    }

    break;

  case OUT:
    byte_written = snprintf(dire_string, BUF, "out");
    err = write(fd, "out", byte_written);
    if(err < 0){
      fprintf(stderr, "Failed to open export for writing!\n");
      return (-1);
    }

    break;

  default:
    fprintf(stderr, "Direction is failed\n");
    return (-1);
  }
  close(fd);
  return(0);
}

int gpio_read(int pin)
{
  char path[BUF];
  char value_str[3];
  int fd;
  int err;
  
  snprintf(path, BUF, "/sys/class/gpio/gpio%d/value",pin);
  fd = open(path,O_RDONLY);
  if(fd < 0){
    fprintf(stderr, "Failed to open gpio  value for reading!\n");
    return(-1);
  }
  err = read(fd, value_str, 3);
    if(err < 0){
      fprintf(stderr ,"Failed to read value!\n");
      return(-1);
    }
    
    close(fd);
    return(atoi(value_str));
}

int gpio_write(int dir_value, int pin)
{
  char value_str[BUF];
  char path[BUF];
  int fd;
  ssize_t byte_written;

  snprintf(path, BUF, "/sys/class/gpio/gpio%d/value", pin);
  fd = open(path, O_WRONLY);
  if(fd < 0){
    fprintf(stderr, "Failed to open gpio value for writing!\n");
    fprintf(stderr, "path = %s\n",path);
    return(-1);
  }
  switch(dir_value){
  case HIGH:
    byte_written = snprintf(value_str, BUF, "1");
    if(write(fd, value_str, byte_written) < 0){
      fprintf(stderr, "Failed to write value!\n");
      return(-1);
    }
    break;

  case LOW:
    byte_written = snprintf(value_str, BUF, "0");
    if(write(fd, value_str, byte_written) < 0){
      fprintf(stderr, "Failed to write value!\n");
      return(-1);
    }
    break;

  default:
    fprintf(stderr, "Enable direction value of output!\n");
    break;
  }
  close(fd);
  return(0);
}






