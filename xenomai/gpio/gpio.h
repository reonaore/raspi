#define IN 0
#define OUT 1
#define LOW 0
#define HIGH 1

//Define mode
#define GPIO_IN 0x0
#define GPIO_OUT 0x1
#define GPIO_ALT_FUNC0 0x4
#define GPIO_ALT_FUNC1 0x5
#define GPIO_ALT_FUNC2 0x6
#define GPIO_ALT_FUNC3 0x7
#define GPIO_ALT_FUNC4 0x3
#define GPIO_ALT_FUNC5 0x2

//GPIO memory mapping
#define BCM2708_PERI_BASE 0x20000000
#define GPIO_BASE (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */
#define PAGE_SIZE (4*1024)
#define BLOCK_SIZE (4*1024)

/* use memory mapped io */
int gpio_init();
int gpio_set_mask(int pin);
int gpio_set_mode(int pin, int mode);
int gpio_output_set(int pin);
int gpio_output_level(int pin, int level);
int gpio_output_clear(int pin);
int gpio_set_mode_out(int pin);
int gpio_set_mode_in(int pin);
int gpio_set_mode_alt_func0(int pin);
int gpio_set_mode_alt_func1(int pin);
int gpio_set_mode_alt_func2(int pin);
int gpio_set_mode_alt_func3(int pin);
int gpio_set_mode_alt_func4(int pin);
int gpio_set_mode_alt_func5(int pin);

/* use /sys/class/gpio */
int gpio_setup(int max_pin_number);
int gpio_export(int pin);
int gpio_unexport(int pin);
int gpio_direction(int direction,int pin);
int gpio_write(int dir_value, int pin);
int gpio_read(int pin);
int gpio_unset(int pin);

volatile unsigned *gpio;

