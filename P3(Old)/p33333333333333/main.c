#include <stdio.h>
#include "44b.h"
#include "button.h"
#include "leds.h"
#include "utils.h"
#include "D8Led.h"
#include "intcontroller.h"
#include "timer.h"
#include "gpio.h"
#include "keyboard.h"

struct RLstat {
	int moving;
	int speed;
	int direction;
	int position;
};

static struct RLstat RL = {
	.moving = 0,
	.speed = 5,
	.direction = 0,
	.position = 0,
};
int cont;
int cont2;
int two;
int mode;
int ocupado;
void timer_ISR(void) __attribute__ ((interrupt ("IRQ")));
void timer1_ISR(void) __attribute__ ((interrupt ("IRQ")));

void button_ISR(void) __attribute__ ((interrupt ("IRQ")));

void keyboard_ISR(void) __attribute__ ((interrupt ("IRQ")));

void timer_ISR(void)
{
	//COMPLETAR: tomar el código de avance de posición del led rotante de la práctica anterior
	if (mode == 1){
		D8Led_digit(cont);
		ic_cleanflag(INT_TIMER0);
		cont++;
		if(cont == 11){
			D8Led_segment(0);
			tmr_stop(TIMER0);
			ocupado = 0;
		}
	}
	else{
		D8Led_digit(cont);
		ic_cleanflag(INT_TIMER0);
		cont++;
		if(cont == 17){
			D8Led_segment(0);
			tmr_stop(TIMER0);
			ocupado = 0;
		}
	}
	ic_cleanflag(INT_TIMER0);
}
void timer1_ISR(void)
{
	//COMPLETAR: tomar el código de avance de posición del led rotante de la práctica anterior
//	if (RL.moving) {
		//if(RL.iter == 0) {
			if(RL.direction==0){
				if(RL.position == 5){
					two--;
					RL.position = 0;}
				else
					RL.position++;
			}
			else{
				if(RL.position == 0){
					RL.position = 5;

				}
				else
					RL.position--;
			}
			D8Led_segment(RL.position);

		//	RL.iter = RL.speed;
		//}
		//else RL.iter--;
//	}

	D8Led_segment(RL.position);
	if(two == 0){
		RL.moving = 0;
		tmr_stop(TIMER1);
		ocupado = 0;

	}
	ic_cleanflag(INT_TIMER1);

}

void button_ISR(void)
{
	unsigned int whicheint = rEXTINTPND;
	unsigned int buttons = (whicheint >> 2) & 0x3;

	//COMPLETAR: usar el código de la primera parte parte de atención a los
	//pulsadores
	if (buttons & BUT1) {
		if (ocupado == 0){
			ocupado = 1;
			if(cont2 == 0){
			cont2 = 1;
			two = 2;
			mode = 1;
			RL.moving = 1;
			tmr_start(TIMER1);

			}
		else{
			ocupado = 1;
			cont2 = 0;
			cont = 12;
			mode = 0;
			tmr_start(TIMER0);

		}
		}
	}
	if (buttons & BUT2) {
		if (ocupado == 0){
			ocupado = 1;
			cont = 0;
			tmr_start(TIMER0);
		}
						/*if (RL.moving){
				RL.moving = 0;
				tmr_stop(TIMER0);
			}
			else {
				RL.moving = 1;
				tmr_update(TIMER0);
				tmr_start(TIMER0);
			}*/
		}
	// eliminamos rebotes
	Delay(2000);
	// borramos el flag en extintpnd
	//rEXTINTPND = //COMPLETAR: debemos borrar las peticiones de interrupción en
		         //EXTINTPND escribiendo un 1 en los flags que queremos borrar (los
				 //correspondientes a los pulsadores pulsados)
	if (rEXTINTPND & 0x4){
			rEXTINTPND |= (0x1 << 2);
		}
		else if(rEXTINTPND & 0x8){
			rEXTINTPND |= (0x1 << 3);
		}
		ic_cleanflag(INT_EINT4567);
	//Borrar el flag de interrupción en el controlador de interrupciones
}

void keyboard_ISR(void)
{
	int key;

	/* Eliminar rebotes de presión */
	Delay(200);

	/* Escaneo de tecla */
	key = kb_scan();

	if (key != -1) {
		/* Visualizacion en el display */
		//COMPLETAR: mostrar la tecla en el display utilizando el interfaz
		//definido en D8Led.h
		D8Led_digit(key);

		switch (key) {
			case 0:
				//COMPLETAR: poner en timer0 divisor 1/8 y contador 62500
				tmr_set_divider(TIMER0, D1_8);
				tmr_set_count(TIMER0, 62500,62495);
				break;
			case 1:
				//COMPLETAR: poner en timer0 timer divisor 1/8 y contador 31250
				tmr_set_divider(TIMER0, D1_8);
				tmr_set_count(TIMER0, 31250,31245);
				break;
			case 2:
				//COMPLETAR: poner en timer0 timer divisor 1/8 y contador 15625
				tmr_set_divider(TIMER0, D1_8);
				tmr_set_count(TIMER0, 15625,15620);
				break;
			case 3:
				tmr_set_divider(TIMER0, D1_4);
				tmr_set_count(TIMER0, 15625,15620);
				//COMPLETAR: poner en timer0 timer divisor 1/4 y contador 15625
				break;
			default:
				break;
		}

		/* Esperar a que la tecla se suelte, consultando el registro de datos */
		//while (/*COMPLETAR: true si está pulsada la tecla (leer del registro rPDATG)*/);
		while(rPDATG == ENABLE);
	}

    /* Eliminar rebotes de depresión */
    Delay(200);

    /* Borrar interrupciones pendientes */
	//COMPLETAR
	//borrar la interrupción por la línea EINT1 en el registro rI_ISPC
    ic_cleanflag(INT_EINT1);
}

int setup(void)
{
	leds_init();
	D8Led_init();
	D8Led_segment(RL.position);

	/* Port G: configuración para generación de interrupciones externas,
	 *         botones y teclado
	 **/

	//COMPLETAR: utilizando el interfaz para el puerto G definido en gpio.h
	//configurar los pines 1, 6 y 7 del puerto G para poder generar interrupciones
	//externas por flanco de bajada por ellos y activar las correspondientes
	//resistencias de pull-up.
		portG_conf(1, EINT);
		portG_conf(6, EINT);
		portG_conf(7, EINT);
		portG_eint_trig(1, FALLING);
		portG_eint_trig(6, FALLING);
		portG_eint_trig(7, FALLING);
		portG_conf_pup(1, ENABLE);
		portG_conf_pup(6, ENABLE);
		portG_conf_pup(7, ENABLE);

	/********************************************************************/

	/* Configuración del timer */
	cont = 0;
	ocupado = 0;
	cont2 = 0;
	//Quedas configurado
	tmr_set_prescaler(0,255);
	tmr_set_divider(0,D1_8);
	tmr_set_count(TIMER0,62500,62495);
	tmr_update(TIMER0);
	tmr_set_mode(0,RELOAD);
	tmr_stop(TIMER0);

	tmr_set_count(TIMER1,62500,62495);
	tmr_update(TIMER1);
	tmr_set_mode(1,RELOAD);
	tmr_stop(TIMER1);



	//COMPLETAR: tomar el código de la segunda parte


	/***************************/

	// Registramos las ISRs
	//pISR_TIMER0   = //COMPLETAR: registrar la RTI del timer
	pISR_TIMER0 = (unsigned) timer_ISR;
	pISR_TIMER1 = (unsigned) timer1_ISR;
	pISR_EINT4567 = (unsigned) button_ISR;
	pISR_EINT1 = (unsigned) keyboard_ISR;

	//pISR_EINT4567 = //COMPLETAR: registrar la RTI de los botones
	//pISR_EINT1    = //COMPLETAR: registrar la RTI del teclado

	ic_init();
		//COMPLETAR: utilizando el interfaz definido en intcontroller.h
		//		habilitar la l�nea IRQ en modo no vectorizado
		//		deshabilitar la l�nea FIQ
		//		configurar la l�nea INT_TIMER0 en modo IRQ
		//		configurar la l�nea INT_EINT4567 en modo IRQ
		//		habilitar la l�nea INT_TIMER0
		//		habilitar la l�nea INT_EINT4567
		ic_conf_irq(ENABLE, VEC);
		ic_conf_fiq(DISABLE);
		ic_conf_line(INT_TIMER0, IRQ);
		ic_conf_line(INT_TIMER1, IRQ);
		ic_conf_line(INT_EINT4567, IRQ);
		ic_enable(INT_TIMER0);
		ic_enable(INT_TIMER1);
		ic_enable(INT_EINT4567);
		ic_enable(INT_EINT1);
	/***************************************************/

	Delay(0);
	return 0;
}

int loop(void)
{
	return 0;
}


int main(void)
{
	setup();

	while (1) {
		loop();
	}
}
