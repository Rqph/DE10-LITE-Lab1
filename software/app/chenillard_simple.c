#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "sys/alt_stdio.h"
#include "alt_types.h"
#include "io.h"
#include "unistd.h"

int main(){
	int led = 0x01;
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,led);
	//alt_printf("main 1\n");
	while(1){
		//alt_printf("i+ \n");
		for (int i = 0; i < 7; i++)
		{
			//alt_printf("i: %x\n",i);
			led = led<<1;
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,led);
			//alt_printf("led<<1: %x\n",led);
			usleep(100000);
        }
		//alt_printf("i- \n");
        for (int i = 7; i > 0 ; i--)
		{
			//alt_printf("i: %x\n",i);
			led = led>>1;
			IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,led);
			//alt_printf("led<<1: %x\n",led);
			usleep(100000);
		}
	}
		
	return 0;		
}
