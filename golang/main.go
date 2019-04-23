package main

import (
	"log"

	c "./collector"
	s "./cron"
)

var period_sec int = 5
var usbenv c.USB_env

func callback() {
	err := usbenv.USB_init()
	if err == nil {
		ret, err2 := usbenv.Get_Environment()
		if err2 == nil {
			log.Printf("Temperature=%d Pressure=%d Humidity=%d\n", ret.Temperature, ret.Pressure, ret.Humidity)
		}
	}
	usbenv.USB_close()
}

func main() {
	_setinterval := s.New(period_sec, callback)
	_setinterval.Start()

	select {}
	return
}
