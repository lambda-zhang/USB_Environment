package models

import (
	// "errors"
	"fmt"
	"log"
	"time"

	"github.com/jinzhu/gorm"
)

type Result2 struct {
	T   string
	H   string
	P   string
	Ctx string
}

func _search2(ts time.Time, _type string) (*[]Result2, error) {
	var ress []Result2
	selector1 := ""
	selector := fmt.Sprintf("%4.4d-%2.2d-%2.2d %2.2d:%2.2d:%2.2d", ts.Year(), ts.Month(), ts.Day(), ts.Hour(), ts.Minute(), ts.Second())
	selector0 := "SELECT ROUND(AVG(temperature), 0) as t, ROUND(AVG(humidity), 0) as h, ROUND(AVG(Pressure), 0) as p, "

	if _type == "hour" {
		selector1 = selector0 + "DATE_FORMAT(created_at,'%i') as ctx, created_at " +
			"from environment where  DATE_SUB(?, INTERVAL 1 HOUR) <= created_at " +
			"GROUP BY ctx ORDER BY created_at ASC"
	} else if _type == "day" {
		selector1 = selector0 + "DATE_FORMAT(created_at,'%H') as ctx, created_at " +
			"from environment where  DATE_SUB(?, INTERVAL 1 DAY) <= created_at " +
			"GROUP BY ctx ORDER BY created_at ASC"
	} else if _type == "month" {
		selector1 = selector0 + "DATE_FORMAT(created_at,'%d') as ctx, created_at " +
			"from environment where  DATE_SUB(?, INTERVAL 1 MONTH) <= created_at " +
			"GROUP BY ctx ORDER BY created_at ASC"
	} else if _type == "year" {
		selector1 = selector0 + "DATE_FORMAT(created_at,'%m') as ctx, created_at " +
			"from environment where  DATE_SUB(?, INTERVAL 1 YEAR) <= created_at " +
			"GROUP BY ctx ORDER BY created_at ASC"
	} else {
		return &ress, nil
	}

	ret := db.Raw(selector1, selector).Scan(&ress)
	if ret.Error != nil {
		log.Println(ret.Error)
	}
	return &ress, ret.Error
}

// Environment 环境数值，包括温度湿度气压
type Environment struct {
	ID          int64     `json:"id"          gorm:"column:id"`          //"id"
	Temperature int32     `json:"temperature" gorm:"column:temperature"` //"温度值"
	Humidity    int32     `json:"humidity"    gorm:"column:humidity"`    //"湿度值"
	Pressure    int32     `json:"Pressure"    gorm:"column:Pressure"`    //"气压值"
	DevID       int64     `json:"devid"       gorm:"column:devid"`       //"设备Id"
	CreatedAt   time.Time `json:"-"           gorm:"column:created_at"`  //"创建时间"
	UpdatedAt   time.Time `json:"-"           gorm:"column:updated_at"`  //"更新时间"
}

// BeforeCreate 插入的时候出发
func (env *Environment) BeforeCreate(scope *gorm.Scope) error {
	if env.CreatedAt.IsZero() {
		env.CreatedAt = time.Now()
	}
	if env.UpdatedAt.IsZero() {
		env.UpdatedAt = time.Now()
	}
	return nil
}

// Save 保存环境数值
func (env *Environment) Save() error {
	env.ID = 0
	ret := db.Model(env).Create(env)
	if ret.Error != nil {
		log.Println(ret.Error)
	}
	return ret.Error
}

func (this *Environment) Search(ts time.Time, _type string) (*[]Result2, error) {
	return _search2(ts, _type)
}
