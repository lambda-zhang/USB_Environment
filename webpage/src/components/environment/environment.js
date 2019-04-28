import echarts from 'echarts'

import {getEnvironmentHour, getEnvironmentDay, getEnvironmentMonth} from '../../models/environment.js'
import {color1, color2, lineItemStyle, isupdated, areaStyleMem, areaStyleSwap, tooltipPercent, tooltipDefault} from '../../consts/echarts.js'

export default {
  name: 'cpumem',
  data () {
    return {
      timer: null
    }
  },
  created () {
  },
  mounted(){
    this.daychart = echarts.init(this.$refs.day)
    this.hourchart = echarts.init(this.$refs.hour)
    this.monthchart = echarts.init(this.$refs.month)
    window.addEventListener("resize", this.daychart.resize)
    window.addEventListener("resize", this.hourchart.resize)
    window.addEventListener("resize", this.monthchart.resize)
    this.daychart.showLoading({text: '加载中...'})
    this.hourchart.showLoading({text: '加载中...'})
    this.monthchart.showLoading({text: '加载中...'})

    var that = this
    that.drawLineHour()
    that.drawLineDay()
    that.drawLineMonth()
    this.timer = setInterval(function () {
      that.drawLineHour()
      that.drawLineDay()
      that.drawLineMonth()
    }, 60*1000)
  },
  beforeDestroy () {
    window.removeEventListener("resize", this.daychart.resize)
    window.removeEventListener("resize", this.hourchart.resize)
    window.removeEventListener("resize", this.monthchart.resize)
    this.daychart.clear()
    this.hourchart.clear()
    this.monthchart.clear()
    this.daychart.dispose()
    this.hourchart.dispose()
    this.monthchart.dispose()

    this.clearInterval(this.timer)
  },
  computed: {
    listencpumeminfo() {
      return this.$store.state.cpumeminfo.latestx
    }
  },
  watch:{
    listencpumeminfo:function(newd, old){
      if (newd <= old) {
        return
      }
      var _cpumem = this.$store.state.cpumeminfo
      this.drawLineCpu(_cpumem)
    }
  },
  methods: {
    drawLineHour(){
      getEnvironmentHour(this, function(opt, err) {
        opt.title.text = '小时'
        this.daychart.hideLoading()
        this.daychart.setOption(opt)
      });
    },
    drawLineDay(){
      getEnvironmentDay(this, function(opt, err) {
        opt.title.text = '天'
        this.hourchart.hideLoading()
        this.hourchart.setOption(opt)
      });
    },
    drawLineMonth(){
      getEnvironmentMonth(this, function(opt, err) {
        opt.title.text = '月'
        this.monthchart.hideLoading()
        this.monthchart.setOption(opt)
      });
    }
  }
}
