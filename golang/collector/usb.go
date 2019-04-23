package usbenv

import (
	"errors"
	"log"
	"time"
	"unsafe"

	"github.com/google/gousb"
)

type USB_env struct {
	ctx    *gousb.Context
	dev    *gousb.Device
	config *gousb.Config
	iface  *gousb.Interface
}

type EnvVal struct {
	Temperature uint32
	Humidity    uint32
	Pressure    uint32
}

var (
	vid         int   = 0x0403
	pid         int   = 0xc630
	cfg         int   = 0x01
	cmd_set_led uint8 = (3 << 5) | (0 << 3)
	cmd_get_ver uint8 = (4 << 5) | (0 << 3)
	cmd_get_env uint8 = (4 << 5) | (1 << 3)
)

func (this *USB_env) USB_init() error {
	var err error
	this.ctx = gousb.NewContext()
	this.ctx.Debug(0)

	this.dev, err = this.ctx.OpenDeviceWithVIDPID(gousb.ID(vid), gousb.ID(pid))
	if this.dev == nil || err != nil {
		return errors.New("Device not found")
	}

	this.dev.ControlTimeout = 5000 * time.Millisecond

	if err = this.dev.SetAutoDetach(true); err != nil {
		log.Println(err)
		return err
	}

	this.config, err = this.dev.Config(cfg)
	if err != nil {
		log.Println(err)
		return err
	}

	this.iface, err = this.config.Interface(0, 0)
	if err != nil {
		log.Println(err)
		return err
	}

	return err
}

func (this *USB_env) USB_close() {
	if this.iface != nil {
		this.iface.Close()
		this.iface = nil
	}
	if this.config != nil {
		this.config.Close()
		this.config = nil
	}
	if this.dev != nil {
		this.dev.Close()
		this.dev = nil
	}
	if this.ctx != nil {
		this.ctx.Close()
		this.ctx = nil
	}
}

func (this *USB_env) Set_LED(onoff uint16) (ret int, err error) {
	if this.dev == nil {
		return -1, errors.New("Device not available")
	}
	return this.dev.Control(gousb.ControlIn|gousb.ControlVendor|gousb.ControlDevice, cmd_set_led, onoff, 0x00, []byte{})
}

func (this *USB_env) Get_Environment() (*EnvVal, error) {
	if this.dev == nil {
		return nil, errors.New("Device not available")
	}
	var data = make([]byte, 256)
	_ret, err := this.dev.Control(gousb.ControlVendor|gousb.ControlDevice|gousb.ControlIn, cmd_get_env, 0x00, 0x00, data)
	if err != nil || _ret < 0 {
		log.Println(err)
		return nil, err
	}
	var penv *EnvVal = *(**EnvVal)(unsafe.Pointer(&data))
	return penv, err
}
