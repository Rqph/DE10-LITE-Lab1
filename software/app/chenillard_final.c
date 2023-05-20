#include <system.h>
#include <altera_avalon_pio_regs.h>
#include <sys/alt_stdio.h>
#include <sys/alt_irq.h>
#include <alt_types.h>
#include <io.h>
#include <unistd.h>


int flag_start = 0;
int flag_stop = 0;
int speed=0;

static void handle_button_interrupts(void* context, alt_u32 id){
	/* Reset the edge capture register. */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0x03);
	
	alt_printf("IRQ Button \n");
	if (flag_start==1) {
		flag_stop =1;
		flag_start = 0;
	}
	else {
		flag_start =1;
		flag_stop = 0;
	}
	
}
static void init_button_pio(){
	/* Enable all 4 button interrupts. */
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTONS_BASE, 0x03);
	//alt_printf("MASK \n");
	/* Reset the edge capture register. */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0x03);
	//alt_printf("EDGE CAP \n");
	/* Register the ISR. */
	alt_irq_register(BUTTONS_IRQ,NULL,(void*)handle_button_interrupts);
	//alt_printf("irq \n");	
}

static void handle_switch_interrupts(void* context, alt_u32 id){
	/* Reset the edge capture register. */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCHS_BASE, 0x7F);
	
	alt_printf("IRQ Switch \n");
	int sw_value = IORD_ALTERA_AVALON_PIO_DATA(SWITCHS_BASE);
	alt_printf("sw_value = %x\n",sw_value);
	
	switch (sw_value){
		case 0x201 :
			speed= 100;
			alt_printf("speed = %x\n",speed);
			break;
		case 0x202 :
			speed= 200;
			alt_printf("speed = %x\n",speed);
			break;
		case 0x204 :
			speed= 300;
			alt_printf("speed = %x\n",speed);
			break;
		case 0x208 :
			speed= 400;
			alt_printf("speed = %x\n",speed);
			break;
		case 0x210 :
			speed= 500;
			alt_printf("speed = %x\n",speed);
			break;
		case 0x220 :
			speed= 600;
			alt_printf("speed = %x\n",speed);
			break;
		case 0x240 :
			speed= 700;
			alt_printf("speed = %x\n",speed);
			break;
		default :
			speed= 100;
	}	
	
}
static void init_switch_pio(){
	/* Enable all 4 button interrupts. */
	IOWR_ALTERA_AVALON_PIO_IRQ_MASK(SWITCHS_BASE, 0x7F);
	//alt_printf("MASK \n");
	/* Reset the edge capture register. */
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(SWITCHS_BASE, 0x7F);
	//alt_printf("EDGE CAP \n");
	/* Register the ISR. */
	alt_irq_register(SWITCHS_IRQ,NULL,(void*)handle_switch_interrupts);
	//alt_printf("irq \n");	
}


int main(){
	//alt_printf("debut main \n");
	init_button_pio();
	init_switch_pio();
	int led = 0x01;
	long time = 0;
	IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,led);
	
	while(1){
			if(flag_start==1){
				while (flag_stop == 0){
					time = speed*100;
					for (int i = 0; i < 7; i++)
					{
						//alt_printf("i: %x\n",i);
						led = led<<1;
						IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,led);
						//alt_printf("led<<1: %x\n",led);
						usleep(time);
					}
					//alt_printf("i- \n");
					for (int i = 7; i > 0 ; i--)
					{
						//alt_printf("i: %x\n",i);
						led = led>>1;
						IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE,led);
						//alt_printf("led<<1: %x\n",led);
						usleep(time);
					}
				}
			}
	}
		
	return 0;		
}
