package main

import (
	"github.com/google/gousb"
	"log"
	"unsafe"
)

type USB_env struct {
	ctx   *gousb.Context
	dev   *gousb.Device
	iface *gousb.Interface
}

type env struct {
	temperature uint16
	humidity    uint16
	pressure    uint16
}

var (
	vid         int   = 0x0403
	pid         int   = 0xc630
	cfg         int   = 0x01
	cmd_set_led uint8 = (3 << 5) | (0 << 3)
	cmd_get_ver uint8 = (4 << 5) | (0 << 3)
	cmd_get_env uint8 = (4 << 5) | (1 << 3)
)

func USB_init(usb_env *USB_env) error {
	var err error
	usb_env.ctx = gousb.NewContext()
	usb_env.ctx.Debug(0)

	usb_env.dev, err = usb_env.ctx.OpenDeviceWithVIDPID(gousb.ID(vid), gousb.ID(pid))
	if usb_env.dev == nil || err != nil {
		log.Println("device not Found")
		return err
	}

	if err := usb_env.dev.SetAutoDetach(true); err != nil {
		usb_env.dev.Close()
		log.Println("Failed to set auto-detach: %v", err)
		return err
	}

	config, err := usb_env.dev.Config(cfg)
	if err != nil {
		usb_env.dev.Close()
		log.Println("Failed to configure device: %v", err)
		return err
	}

	usb_env.iface, err = config.Interface(0, 0)
	if err != nil {
		usb_env.dev.Close()
		log.Println("Failed to claim iface-%v: %v", 0, err)
		return err
	}

	return err
}

func USB_close(usb_env *USB_env) {
	usb_env.dev.Close()
}

func Set_LED(usb_env *USB_env, onoff uint16) (ret int, err error) {
	return usb_env.dev.Control(gousb.ControlIn|gousb.ControlVendor|gousb.ControlDevice, cmd_set_led, onoff, 0x00, []byte{})
}

func Get_Environment(usb_env *USB_env) (*env, error) {
	var data = make([]byte, 256)
	_ret, err := usb_env.dev.Control(gousb.ControlVendor|gousb.ControlDevice|gousb.ControlIn, cmd_get_env, 0x00, 0x00, data)
	if err != nil || _ret < 0 {
		return nil, err
	}
	var penv *env = *(**env)(unsafe.Pointer(&data))
	return penv, err
}

func main() {
	var err error
	var usb_env USB_env

	if err := USB_init(&usb_env); err != nil {
		log.Println(err)
		USB_close(&usb_env)
		return
	}

	Set_LED(&usb_env, 1)

	_env, err := Get_Environment(&usb_env)
	if err == nil || _env != nil {
		log.Println(_env)
	}

	USB_close(&usb_env)
	return
}
