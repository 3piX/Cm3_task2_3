#include "stm32f10x_lib.h"
#include "sys_init.h"
#include "ADC.h"


#define PIN_ZIGBEE_TX	GPIO_Pin_12
#define PIN_ZIGBEE_RX	GPIO_Pin_2

#define PIN_LED_AUX				GPIO_Pin_12
#define PIN_LED_MANAGE			GPIO_Pin_13
#define PIN_LED_PROGRAM			GPIO_Pin_14
#define PIN_LED_PLAY			GPIO_Pin_15


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void RCC_Configuration(void)
{
	ErrorStatus HSEStartUpStatus;
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if(HSEStartUpStatus == SUCCESS)
	{
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		/* Flash 2 wait state */
		FLASH_SetLatency(FLASH_Latency_2);

		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);

		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);

		/* PCLK1 = HCLK/2 */
		RCC_PCLK1Config(RCC_HCLK_Div2);

		/* PLLCLK = 8MHz * 9 = 72 MHz */
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

		/* Enable PLL */
		RCC_PLLCmd(ENABLE);

		/* Wait till PLL is ready */
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

		/* Wait till PLL is used as system clock source */
		while(RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}

	/* Enable peripheral clocks --------------------------------------------------*/

	/* Enable USART1 and GPIOB clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC	 | RCC_APB2Periph_ADC1, ENABLE);


	/* Enable USART3 clocks */
	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_USART3 | RCC_APB1Periph_TIM2, ENABLE);

	PWR_BackupAccessCmd(ENABLE);
}

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	#ifdef  VECT_TAB_RAM
		// Set the Vector Table base location at 0x20000000
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
	#else  // VECT_TAB_FLASH
		// Set the Vector Table base location at 0x08003000
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x3000);
	#endif

	// Configure the NVIC Preemption Priority Bits
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// Enable the USART1 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the TIM2 Interrupt
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the ADC Interrupt
	//NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQChannel;
	//NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	//NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);


	// PORTA CONFIG
	GPIO_InitStructure.GPIO_Pin = 	 ADC_1_PIN_SIG_MOT1P | ADC_1_PIN_SIG_MOT1M  | ADC_2_PIN_SIG_MOT1P | ADC_2_PIN_SIG_MOT1M | ADC_3_PIN_SIG_MOT1P | ADC_3_PIN_SIG_MOT1M;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// PORTB CONFIG
	GPIO_InitStructure.GPIO_Pin = PIN_LED_AUX | PIN_LED_MANAGE | PIN_LED_PROGRAM | PIN_LED_PLAY	| PIN_ENABLE_TXD | PIN_ENABLE_RXD | ADC_6_PIN_SIG_MOT1P | ADC_6_PIN_SIG_MOT1M;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DXL_RXD | PIN_PC_RXD;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_DXL_TXD | PIN_PC_TXD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig( GPIO_Remap_USART1, ENABLE);
	GPIO_PinRemapConfig( GPIO_Remap_SWJ_Disable, ENABLE);

	GPIO_ResetBits(PORT_ENABLE_TXD, PIN_ENABLE_TXD);	// TX Disable
	GPIO_SetBits(PORT_ENABLE_RXD, PIN_ENABLE_RXD);	// RX Enable

		// PORTC CONFIG
	GPIO_InitStructure.GPIO_Pin =   PIN_ADC_SELECT0 | PIN_ADC_SELECT1 |  PIN_LED_POWER | ADC_4_PIN_SIG_MOT1P | ADC_4_PIN_SIG_MOT1M | ADC_5_PIN_SIG_MOT1P | ADC_5_PIN_SIG_MOT1M;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// set Analog input of ADC
	GPIO_InitStructure.GPIO_Pin =  PIN_ADC0 | PIN_VDD_VOLT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(PORT_ADC0, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  PIN_ADC1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(PORT_ADC1, &GPIO_InitStructure);

	// PORTC - Zigbee
	GPIO_InitStructure.GPIO_Pin =  PIN_ZIGBEE_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// PORTD CONFIG (Zigbee)
	GPIO_InitStructure.GPIO_Pin = PIN_ZIGBEE_RX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);


}


void SysTick_Configuration(void)
{
	  /* SysTick end of count event each 1us with input clock equal to 9MHz (HCLK/8, default) */
	  SysTick_SetReload(9);

	  /* Enable SysTick interrupt */
	  SysTick_ITConfig(ENABLE);
}

void Init_Timer2()
{
	TIM2->CR1 = 0x000; // Upcounting mode + no buffer on reload register, Counter diable
	TIM2->PSC = 0x0F5F; // Prescaler
	TIM2->ARR = 0xFFFF; // Reload register

	TIM2->CCMR1 = 0x0000; // NO output compare and input capture!!!
	//TIM2->DIER = 0x0002; // CC1IE UIE
	TIM2->CCR1 = 0xFF00; // CCR1 value = ~10 times overflow pr rotation of IR sensor
	TIM2->CNT = 0x0000;
	//TIM2->CR1 = TIM_CR1_CEN; // ENABLE TIMER!
}


