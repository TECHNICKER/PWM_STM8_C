#include "stm8s.h"

extern int prev_states[3] = {0, 0, 0,}; //pole pro uchování předchozích (previous) stavů tlačítek 
extern int curr_states[3] = {0, 0, 0,}; //pole pro uchování současných (current) stavů tlačítek
uint16_t timer_h = 5000;                //výchozí a eventuelně měnící se hodnota Th, která přímo ovlivňuje výsldné DCL a jas

uint16_t time_frame = 10000;            //pevně stanovená délka jedné periody v rámci které regulujeme Th

int was_pressed(int button_number);     //deklarace funkce pro ověřování jestli bylo tlačítko zmáčknuto (was pressed)
void delay(int delay_time);             //deklarace funkce pro zpoždění (delay)

void main(void)
{
    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);              //dělič 1 => 16MHz                 
    GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_OUT_PP_LOW_SLOW);

    while(1)
    {
        if (was_pressed(1) == 1)        //ptáme se zda bylo zmáčknuto tlačítko číslo 1
        {
            if (timer_h > 9000)         //hlídání horní hranice Th, aby Th nikdy nepřekročilo délku samotného cyklu
            {timer_h = 10000;}          
            else
            {timer_h += 1000;}             
        }

        if (was_pressed(2) == 1)        //ptáme se zda bylo zmáčknuto tlačítko číslo 2
        {
            if (timer_h < 1000)         //hlídání dolní hranice Th, aby Th nikdy nebylo menší než 0, resp. negativní
            {timer_h = 0;}              
            else
            {timer_h -= 1000;}
        }
        
        GPIO_WriteHigh(GPIOB, GPIO_PIN_0);  //rozsvícení LED
        
        delay(timer_h);                     //počkání o Th => LEDky budou svítit po dobu Th

        GPIO_WriteLow(GPIOB, GPIO_PIN_0);   //zhasnutí LED

        delay(time_frame - timer_h);        //počkání o zbytek cyklu, aby byl kompletní (délka cyklu - Th)
    }
}

int was_pressed(int button_number)
{
    GPIO_Init(GPIOG, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOG, GPIO_PIN_5, GPIO_MODE_IN_FL_NO_IT);

    if (GPIO_ReadInputPin(GPIOG, GPIO_PIN_4) == RESET) //pokud na tlačítku čteme změnu stavu (zkrat)
    {curr_states[1] = 1;}                              //nastavíme současný stav na '1'
    else
    {curr_states[1] = 0;}                              //jinak nastavíme současný čas na '0'

    if (GPIO_ReadInputPin(GPIOG, GPIO_PIN_5) == RESET) //to stejné pouze pro tlačítko 2
    {curr_states[2] = 1;} 
    else
    {curr_states[2] = 0;}


    if (prev_states[button_number] == 0 && curr_states[button_number] == 1) //reagujeme pouza na stisk tlačítka
    {
        prev_states[button_number] = curr_states[button_number];            //pokud je toto zaznamenáno, funkce vrátí '1'
        return 1;
    }
    else
    {
        prev_states[button_number] = curr_states[button_number];      //pokud stisknutí nenastane, funkce vrátí '0'
        return 0;
    }
}

void delay(int delay_time)
{
    for (uint16_t i = 0; i < delay_time; i++); //limit zpoždění(počtu průběhů foru) je největší 16-bitové číslo
}











