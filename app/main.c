#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <usb.h>

/* vendor and product id */
#define USB_VID  0x0403
#define USB_PID  0xc630

/* command format:
 * 7 6 5 4   3 2 1 0
 * |-+-| |-+-| |-+-|
 *   |     |     +------- number of bytes in transfer
 *   |     +------------- target
 *   +------------------- command
 *
 */

#define UE_ECHO           (0<<5)
#define UE_CMD            (1<<5)
#define UE_DATA           (2<<5)
#define UE_SET            (3<<5)
#define UE_GET            (4<<5)

/* target is value to set */
#define UE_SET_LED   (UE_SET | (0<<3))

/* target is value to get */
#define UE_GET_FWVER      (UE_GET | (0<<3))
#define UE_GET_ENV        (UE_GET | (1<<3))

typedef struct _env {
    unsigned int temperature;
    unsigned int humidity;
    unsigned int pressure;
} env;

usb_dev_handle      *handle = NULL;

void ue_echo(void) {
	int i, nBytes, errors=0;
	unsigned short val, ret;
	int ECHO_NUM = 100;

	for(i=0;i<ECHO_NUM;i++) {
		val = rand() & 0xffff;
		nBytes = usb_control_msg(handle,
				USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				UE_ECHO, val, 0, (char*)&ret, sizeof(ret), 1000);
		if(nBytes < 0) {
			fprintf(stderr, "USB request failed!");
			return;
		}
		if(val != ret)
			errors++;
	}
	if(errors)
		fprintf(stderr, "ERROR: %d out of %d echo transfers failed!\n",
				errors, ECHO_NUM);
	else
		printf("Echo test successful!\n");
}

/* get lcd2usb interface firmware version */
void ue_get_version(void) {
	unsigned char       buffer[64];
	int                 nBytes;
	/* send control request and accept return value */
	nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			UE_GET_FWVER, 0, 0, (char *)buffer, sizeof(buffer), 1000);
	if(nBytes < 0) {
		fprintf(stderr, "USB request failed!\n");
		return;
	}
	printf("Firmware version %d.%d\n", buffer[0], buffer[1]);
}

int ue_get_environment(void) {
	unsigned char       buffer[64];
	int                 nBytes;
	/* send control request and accept return value */
	nBytes = usb_control_msg(handle, USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			UE_GET_ENV, 0, 0, (char *)buffer, sizeof(buffer), 1000);
	if(nBytes < 0) {
		fprintf(stderr, "USB request failed!\n");
		return -1;
	}
	env *penv = (env *)buffer;
	printf("temperature=%d humidity=%d pressure=%d\n", penv->temperature, penv->humidity, penv->pressure);
	return 0;
}

void ue_set_led(int onoff)
{
	int ret = usb_control_msg(handle, USB_TYPE_VENDOR, UE_SET_LED, onoff, 0, NULL, 0, 1000);
	if(ret < 0) {
		fprintf(stderr, "USB request failed!\n");
	}
}

int main(int argc, char *argv[])
{
	struct usb_bus      *bus;
	struct usb_device   *dev;

	usb_init();
	usb_find_busses();
	usb_find_devices();

	for(bus = usb_get_busses(); bus; bus = bus->next) {
		for(dev = bus->devices; dev; dev = dev->next) {
			if((dev->descriptor.idVendor == USB_VID) && (dev->descriptor.idProduct == USB_PID)) {
				printf("Found USB device on bus %s device %s.\n", bus->dirname, dev->filename);
				/* open device */
				if(!(handle = usb_open(dev)))
					fprintf(stderr, "Error: Cannot open USB device: %s\n", usb_strerror());
				break;
			}
		}
	}

	if(!handle) {
		fprintf(stderr, "Error: Could not find USB device\n");
		exit(-1);
	}

    ue_get_version();

    int i = 0;
    for(i=0; i< 1000; i++) {
		ue_set_led(0);
		sleep(1);
		ue_set_led(1);
		sleep(1);
		ue_get_environment();
    }

	usb_close(handle);

	return 0;
}
