package models

import (
	// "errors"
	"log"
	"time"

	"github.com/jinzhu/gorm"
)

type Temperature struct {
	Id        int64 `json:"id"         gorm:"column:id"         "ID"`
	Val       int32 `json:"val"        gorm:"column:val"        "温度值"`
	DevId     int64 `json:"devid"      gorm:"column:devid"      "设备Id"`
	CreatedAt int64 `json:"created_at" gorm:"column:created_at" "创建时间"`
	UpdatedAt int64 `json:"-"          gorm:"column:updated_at" "更新时间"`
}

func (this *Temperature) BeforeCreate(scope *gorm.Scope) error {
	if this.CreatedAt == 0 {
		this.CreatedAt = time.Now().Unix() * 1000
	}
	if this.UpdatedAt == 0 {
		this.UpdatedAt = time.Now().Unix() * 1000
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

func (this *Temperature) Search(start int64, end int64) (*[]Temperature, error) {
	t := []Temperature{}
	ret := db.Model(this).Where("created_at between ? AND ?", start, end).Find(&t)
	if ret.Error == nil {
		log.Println(ret.Error)
	}
	return &t, ret.Error
}
