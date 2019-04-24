package main

import (
	"log"

	c "./collector"
	s "./cron"
	m "./models"
)

var period_sec int = 5
var usbenv c.USB_env

func callback() {
	err := usbenv.USB_init()
	if err == nil {
		ret, err2 := usbenv.Get_Environment()
		if err2 == nil {
			log.Printf("Temperature=%d Pressure=%d Humidity=%d\n", ret.Temperature, ret.Pressure, ret.Humidity)
			t := m.Temperature{Val: ret.Temperature, DevId: 0}
			h := m.Humidity{Val: ret.Humidity, DevId: 0}
			p := m.Pressure{Val: ret.Pressure, DevId: 0}
			t.Save()
			h.Save()
			p.Save()

			ret, _ := t.SearchOneDay(2019, 4, 24)
			log.Println(ret)
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
