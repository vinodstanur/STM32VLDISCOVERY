#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>

/* Set STM32 to 24 MHz. */
void clock_setup(void)
{
	rcc_clock_setup_in_hse_8mhz_out_24mhz();

	/* Enable GPIOC clock. */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);
}

void gpio_setup(void)
{
	/* Set GPIO 8/9 (in GPIO port C) to 'output push-pull'. */
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO8);
	gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_PUSHPULL, GPIO9);
}

void user_button_enable(void)
{
	/*Enable GPIOA clock */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPAEN);
	/*set GPIOA0 as input open-drain */
	gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO0);
}

void delay(volatile u32 t)
{
	t *= 1000;
	while (t--) ;
}

int main(void)
{
	int i;

	clock_setup();
	gpio_setup();
	user_button_enable();
	GPIOC_ODR = GPIO9;
	while (1) {
		if (GPIOA_IDR & GPIO0) {
			delay(1);
			while (GPIOA_IDR & GPIO0) ;
			delay(600);
		}
		GPIOC_ODR ^= (GPIO8 | GPIO9);	/* Toggle LEDs. */
	}

	return 0;
}
