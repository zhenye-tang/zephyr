/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>
#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   1000

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static int sdram_test(void)
{
	int res = 0;
	char *p = (char *)0xc0000000;
	char *q = (char *)0xc0000000 + 512;
	memset(p, 'a', 512);
	memset(q, 'a', 512);
	if(!(res = memcmp(q, p, 512)))
		printk("sdram test ok!!!\n");
	else
	 	printk("sdram test error!!!\n");

	return res;
}

int main(void)
{
	int ret;
	bool led_state = true;

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
	printk("hello zephyr.\n");
	sdram_test();
	while (1) {
		ret = gpio_pin_toggle_dt(&led);
		if (ret < 0) {
			return 0;
		}
		led_state = !led_state;
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
