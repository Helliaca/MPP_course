#include "init_taste1.h"

void init_tasten() {

	GPIO_DeInit(GPIOC);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;

	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN;       //GPIO Input Mode
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;   //Medium speed
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //PushPull
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_NOPULL;   //PullUp

	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

uint8_t k1_d = 0, k1_u = 0, k2_d = 0, k2_u = 0;

uint8_t key1_down() { return k1_d; }

uint8_t key2_down() { return k2_d; }

uint8_t key1_up() { return k1_u; }

uint8_t key2_up() { return k2_u; }

uint8_t get_key1() {
	return !GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
}

uint8_t get_key2() {
	return GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5);
}

void update_keys() {
  static uint8_t k1_was_pressed = 0;
  static uint8_t k2_was_pressed = 0;

  k1_d = k1_was_pressed < get_key1();
  k2_d = k2_was_pressed < get_key2();

  k1_u = k1_was_pressed > get_key1();
  k2_u = k2_was_pressed > get_key2();

  k1_was_pressed = get_key1();
  k2_was_pressed = get_key2();
}
