package setinterval

import (
	"fmt"
	"runtime"

	cron "github.com/robfig/cron"
)

func New(period_sec int, callback func()) *cron.Cron {
	ostype := runtime.GOOS
	if ostype != "linux" {
		panic("support linux only fornow")
		return nil
	}
	if period_sec < 1 {
		panic("period must >= 1second")
		return nil
	}

	spec := fmt.Sprintf("@every %ds", period_sec)
	c := cron.New()
	c.AddFunc(spec, func() {
		callback()
	})
	return c
}
