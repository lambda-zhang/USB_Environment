package models

import (
	// "errors"
	"fmt"
	"log"
	"time"

	"github.com/jinzhu/gorm"
)

type Temperature struct {
	Id        int64     `json:"id"         gorm:"column:id"         "ID"`
	Val       int32     `json:"val"        gorm:"column:val"        "温度值"`
	DevId     int64     `json:"devid"      gorm:"column:devid"      "设备Id"`
	CreatedAt time.Time `json:"-"          gorm:"column:created_at" "创建时间"`
	UpdatedAt time.Time `json:"-"          gorm:"column:updated_at" "更新时间"`
}

func (this *Temperature) BeforeCreate(scope *gorm.Scope) error {
	if this.CreatedAt.IsZero() {
		this.CreatedAt = time.Now()
	}
	if this.UpdatedAt.IsZero() {
		this.UpdatedAt = time.Now()
	}
	return nil
}

func (this *Temperature) Save() error {
	this.Id = 0
	ret := db.Model(this).Create(this)
	if ret.Error != nil {
		log.Println(ret.Error)
	}
	return ret.Error
}

type Result struct {
	Avg   string
	Houre string
}

func (this *Temperature) SearchOneDay(year int, mounth int, day int) (*[]Result, error) {
	var ress []Result
	selector1 := "SELECT AVG(val) as avg, DATE_FORMAT(created_at,'%k') as houre from temperature where DATE_FORMAT(created_at,'%Y-%m-%d')=? GROUP BY houre"
	selector := fmt.Sprintf("%4.4d-%2.2d-%2.2d", year, mounth, day)
	ret := db.Raw(selector1, selector).Scan(&ress)
	if ret.Error != nil {
		log.Println(ret.Error)
	}
	return &ress, ret.Error
}

type Humidity struct {
	Id        int64     `json:"id"         gorm:"column:id"         "ID"`
	Val       int32     `json:"val"        gorm:"column:val"        "湿度值"`
	DevId     int64     `json:"devid"      gorm:"column:devid"      "设备Id"`
	CreatedAt time.Time `json:"-"          gorm:"column:created_at" "创建时间"`
	UpdatedAt time.Time `json:"-"          gorm:"column:updated_at" "更新时间"`
}

func (this *Humidity) BeforeCreate(scope *gorm.Scope) error {
	if this.CreatedAt.IsZero() {
		this.CreatedAt = time.Now()
	}
	if this.UpdatedAt.IsZero() {
		this.UpdatedAt = time.Now()
	}
	return nil
}

func (this *Humidity) Save() error {
	this.Id = 0
	ret := db.Model(this).Create(this)
	if ret.Error != nil {
		log.Println(ret.Error)
	}
	return ret.Error
}

type Pressure struct {
	Id        int64     `json:"id"         gorm:"column:id"         "ID"`
	Val       int32     `json:"val"        gorm:"column:val"        "气压值"`
	DevId     int64     `json:"devid"      gorm:"column:devid"      "设备Id"`
	CreatedAt time.Time `json:"-"          gorm:"column:created_at" "创建时间"`
	UpdatedAt time.Time `json:"-"          gorm:"column:updated_at" "更新时间"`
}

func (this *Pressure) BeforeCreate(scope *gorm.Scope) error {
	if this.CreatedAt.IsZero() {
		this.CreatedAt = time.Now()
	}
	if this.UpdatedAt.IsZero() {
		this.UpdatedAt = time.Now()
	}
	return nil
}

func (this *Pressure) Save() error {
	this.Id = 0
	ret := db.Model(this).Create(this)
	if ret.Error != nil {
		log.Println(ret.Error)
	}
	return ret.Error
}
