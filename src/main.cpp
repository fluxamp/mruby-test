
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#include <libopencmsis/core_cm3.h>

#include <stdio.h>
#include <mruby.h>
#include <mruby/compile.h>

#include <FreeRTOS.h> 
#include <task.h> 

// needed by FreeRTOS
uint32_t SystemCoreClock = 16000000UL;

void clock_setup(void)
{
	// enable systick timer for FreeRTOS
	SysTick_Config(SystemCoreClock / 1000);
}

void gpio_setup(void)
{
	// setup pins for LED
	rcc_periph_clock_enable(RCC_GPIOB);
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);

	// setup pins for USART
	rcc_periph_clock_enable(RCC_USART3);
	rcc_periph_clock_enable(RCC_GPIOD);
	gpio_mode_setup(GPIOD, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8);
	gpio_set_af(GPIOD, GPIO_AF7, GPIO8);
}

void usart_setup(void)
{
	// setup USART3
	usart_set_baudrate(USART3, 115200);
	usart_set_databits(USART3, 8);
	usart_set_stopbits(USART3, USART_STOPBITS_1);
	usart_set_mode(USART3, USART_MODE_TX);
	usart_set_parity(USART3, USART_PARITY_NONE);
	usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);
	usart_enable(USART3);
}

// define stdio output
extern "C" {
	int _write(int file, char* ptr, int len)
	{
		int sent = 0;

		for(sent=0; sent < len && (*ptr); ++sent)
		{
			usart_send_blocking(USART3, *ptr);
			if(*ptr == '\n')
			{
				usart_send_blocking(USART3, '\r');
			}
			ptr++;
		}

		return sent;
	}
}

void led_task (void* parameter)
{
	while (true)
	{
		gpio_toggle(GPIOB, GPIO14);	/* LED on/off */
		vTaskDelay(1000);
	}
}

void print_task (void* parameter)
{
	mrb_state *mrb = mrb_open();
	if (!mrb) { /* handle error */ }
	puts("Executing Ruby code from C!");
	mrb_load_string(mrb, "p 'hello world!'");
	mrb_close(mrb);

	char c = 0;
	while(true)
	{
		usart_send_blocking(USART3, '0' + c++);
		if(c >= 10) c = 0;

		vTaskDelay(1000);
	}
}

int main(void)
{
	clock_setup();
	gpio_setup();
	usart_setup();

	if (xTaskCreate(led_task, "leds", 32, NULL, 4, NULL) != pdPASS)
	{ 
		// TODO: proper error reporting/handling
		while(1);
	}
	if (xTaskCreate(print_task, "printing", 2048, NULL, 4, NULL) != pdPASS)
	{ 
		// TODO: proper error reporting/handling
		while(1);
	}

	vTaskStartScheduler();

	return 0;
}
