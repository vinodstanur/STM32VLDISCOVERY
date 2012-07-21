#include <libopencm3/stm32/f1/rcc.h>
#include <libopencm3/stm32/f1/gpio.h>
#include <libopencm3/stm32/timer.h>

/* Set STM32 to 24 MHz. */
void clock_setup(void)
{
	//rcc_clock_setup_in_hse_8mhz_out_24mhz();
	rcc_clock_setup_in_hse_8mhz_out_24mhz();
}

void gpio_setup(void)
{
	/* Enable GPIOC clock. */
	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPCEN);
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

void timer2_init(void)
{
	rcc_peripheral_enable_clock(&RCC_APB1ENR, RCC_APB1ENR_TIM2EN);
/*we can also use the inbuild fuctions declared in timer.h, but any way
 I am just directly configuring few registers
 */
	//RCC_CFGR |= RCC_CFGR_SW_SYSCLKSEL_PLLCLK|RCC_CFGR_PLLMUL_PLL_CLK_MUL3|RCC_CFGR_PLLSRC_HSE_CLK;
	//RCC_CR |= RCC_CR_HSEON|RCC_CR_PLLON;
	TIM2_CR1 |= TIM_CR1_CEN;
	TIM2_PSC = 1000; //setting prescaler to 1000
	TIM2_ARR = 24000; //auto reload value adjusted for 1 second
}

void led_heartbeat(void)
{
	volatile int i;
	GPIOC_ODR = GPIO8;
	i=200000; 
	while(i--);
	GPIOC_ODR = GPIO9;
	i=200000;
	while(i--);
	GPIOC_ODR = 0;
}

int main(void)
{
	clock_setup();
	gpio_setup();
	timer2_init();
	while (1) {
		while(!(TIM2_SR & (TIM_SR_UIF)));//polling for update interrupt flag
		TIM2_SR &= ~TIM_SR_UIF; //clearing update interrupt flag
		led_heartbeat(); //a short duration(<<1s) blink on two leds
	}

	return 0;
}
