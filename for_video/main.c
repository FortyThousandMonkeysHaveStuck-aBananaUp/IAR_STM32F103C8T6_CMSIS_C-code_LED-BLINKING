#include "stm32f103xb.h"

void System_clock_config(void);
void GPIO_PA10_Config(void);
void USER_delay(unsigned int wait);

int main()
{
  /*config*/
  //clock tree
  System_clock_config();
  GPIO_PA10_Config();
  //config the PA10 pin
  
  while(1)
  {
  //set PA10
  GPIOA->BSRR |= GPIO_BSRR_BS10;
  //1: Set the corresponding ODRx bit
  //wait
  USER_delay(6000000);
    
    
  //reset PA10
  GPIOA->BSRR |= GPIO_BSRR_BR10;
  //1: Reset the corresponding ODRx bit
  //wait 
  USER_delay(6000000);
  }
  
  return 0;
}

void System_clock_config(void)
{
  //1 Flash latency
  FLASH->ACR |= FLASH_ACR_LATENCY_1; //FLASH->ACR = FLASH->ACR | FLASH_ACR_LATENCY_1;
  //010 Two wait states, if 48 MHz < SYSCLK  ≤ 72 MHz
  
  //2 enable HSE and wait for HSE to become ready
  RCC->CR |= RCC_CR_HSEON;
  while(!(RCC->CR & RCC_CR_HSERDY));
  //1: HSE oscillator ON
  
  //3.1 PLL multiptlication factor
  RCC->CFGR |= RCC_CFGR_PLLMULL9;
  //0111: PLL input clock x 9
  
  //3.2 PLL entry source clock
  RCC->CFGR |= RCC_CFGR_PLLSRC;
  //1: Clock from PREDIV1 selected as PLL input clock
  
  //4 enable PLL and wait for it to become ready
  RCC->CR |= RCC_CR_PLLON;
  //1: PLL ON
  while(!(RCC->CR & RCC_CR_PLLRDY));
  
  //5 set the prescalers AHB APB1 APB2
  //AHB         /1
  //do nothing
  
  //APB1        /2
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
  //100: HCLK divided by 2
  
  //APB2        /1
  //do nothing
  
  //6 SW status
  RCC->CFGR |= RCC_CFGR_SW_PLL;
  //10: PLL selected as system clock
  while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));
  
}

void GPIO_PA10_Config(void)
{
  //1 GPIOA clock
  RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  //1:I/O port A clock enabled
  
  //2 reset PA pins
  GPIOA->BRR = 0x0000ffff;
  //1: Reset the corresponding ODRx bit
  
  //3 set the mode
  GPIOA->CRH |= GPIO_CRH_MODE10_Msk;
  //11: Output mode, max speed 50 MHz.
  
  //4 configure the PA10 pin
  GPIOA->CRH &= ~GPIO_CRH_CNF10_Msk; 
  //00: General purpose output push-pull
}

void USER_delay(unsigned int wait)
{
  while(--wait);
}