#include "44b.h"
#include "gpio.h"

/* Port B interface implementation */

int portB_conf(int pin, enum port_mode mode)
{
	int ret = 0;
	if (pin < 0 || pin > 10)
		return -1;

	if (mode == SIGOUT)
		rPCONB |= (0x1 << pin);

		// COMPLETAR si es necesario
	else if (mode == OUTPUT)
		rPCONB &= ~(0x1 << pin);
	else
		ret = -1;

	return ret;
}

int portB_write(int pin, enum digital val)
{
	if (pin < 0 || pin > 10)
		return -1;

	if (val < 0 || val > 1)
		return -1;


	if (val)
		// COMPLETAR: poner en rPDATB el bit indicado por pin a 1
		rPDATB |= (0x1 << pin);

	else
		// COMPLETAR: poner en rPDATB el bit indicado por pin a 0
		rPDATB &= ~(0x1 << pin);
	return 0;
}

/* Port G interface implementation */

int portG_conf(int pin, enum port_mode mode)
{
	int pos  = pin*2;

	if (pin < 0 || pin > 7)
		return -1;


	switch (mode) {
		case INPUT:
			rPCONG &= ~(0x3 << pos);
			// COMPLETAR: poner en rPCONG 00 a partir de la posición pos para
			// configurar como pin de entrada el pin indicado por el parámetro pin
			break;
		case OUTPUT:
			rPCONG &= ~(0x3 << pos);
			rPCONG |= (0x1 << pos);
			// COMPLETAR: poner en rPCONG 01 a partir de la posición pos para
			// configurar como pin de salida el pin indicado por el parámetro pin
			break;
		case SIGOUT:
			rPCONG &= ~(0x3 << pos);
			rPCONG |= (0x2 << pos);
			// COMPLETAR: poner en rPCONG 10 a partir de la posición pos para
			// que salga la señal interna correspondiente por el pin indicado
			// por el parámetro piSn
			break;
		case EINT:
			rPCONG |= (0x3 << pos);
			// COMPLETAR: poner en rPCONG 11 a partir de la posición pos para
			// habilitar la generación de interrupciones externas por el pin
			// indicado por el parámetro pin
			break;
		default:
			return -1;
	}

	return 0;
}

int portG_eint_trig(int pin, enum trigger trig)
{
	int pos = pin*4;

	if (pin < 0 || pin > 7)
		return -1;

switch (trig) {
		case LLOW:
			// COMPLETAR: poner en rEXTINT a partir de la posición pos tres bits
			// a 000, para configurar interrupciones externas por nivel bajo
			rEXTINT &= ~(0x7 << pos);
			rEXTINT |= (0x0 << pos);
			break;
		case LHIGH:
			rEXTINT &= ~(0x7 << pos);
			rEXTINT |= (0x1 << pos);
			// COMPLETAR: poner en rEXTINT a partir de la posición pos tres bits
			// a 001, para configurar interrupciones externas por nivel alto
			break;
		case FALLING:
			rEXTINT &= ~(0x7 << pos);
			rEXTINT |= (0x2 << pos);
			// COMPLETAR: poner en rEXTINT a partir de la posición pos tres bits
			// a 010, para configurar interrupciones externas por flanco de
			// bajada
			break;
		case RISING:
			rEXTINT &= ~(0x7 << pos);
			rEXTINT |= (0x4 << pos);
			// COMPLETAR: poner en rEXTINT a partir de la posición pos tres bits
			// a 100, para configurar interrupciones externas por flanco de
			// subida
			break;
		case EDGE:
			rEXTINT &= ~(0x7 << pos);
			rEXTINT |= (0x6 << pos);
			// COMPLETAR: poner en rEXTINT a partir de la posición pos tres bits
			// a 110, para configurar interrupciones externas por cualquier
			// flanco
			break;
		default:
			return -1;
	}
	return 0;
}

int portG_write(int pin, enum digital val)
{
	int pos = pin*2;

	if (pin < 0 || pin > 7)
		return -1; // indica error

	if (val < 0 || val > 1)
		return -1; // indica error

	if ((rPCONG & (0x3 << pos)) != (0x1 << pos))
		return -1; // indica error

	if (val)
		rPDATG|= ~(0x1<<pin);
		// COMPLETAR: poner en rPDATG el bit indicado por pin a 1
	else
		rPDATG &= ~(0x1<<pin);
		// COMPLETAR: poner en rPDATG el bit indicado por pin a 0

	return 0;
}

int portG_read(int pin, enum digital* val)
{
	int pos = pin*2;

	if (pin < 0 || pin > 7)
		return -1; // indica error

	if (rPCONG & (0x3 << pos))
		return -1; // indica error

	if((rPDATG & (0x1 << pin)))
		*val = HIGH;
	else
		*val = LOW;

	return 0;}

int portG_conf_pup(int pin, enum enable st)
{
	if (pin < 0 || pin > 7)
		return -1; // indica error

	if (st != ENABLE && st != DISABLE)
		return -1; // indica error

	if (st == ENABLE)
		rPUPG &= ~(0x1 << pin);
		// COMPLETAR: poner el pin de rPUPG indicado por el parametro pin al valor adecuado,
		// para activar la resistencia de pull-up
	else
		rPUPG |= (0x1 << pin);
		// COMPLETAR: poner el pin de rPUPG indicado por el parametro pin al valor adecuado,
		// para desactivar la resistencia de pull-up

	return 0;
}

