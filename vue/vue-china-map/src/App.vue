<template>

  <div id="myMap">
  </div>
</template>

<script>
import echarts from "echarts";
import 'echarts/map/js/china.js'
import chinaJson from "./map/data/mapJson.json"

export default {
  name: 'App',
  data () {
    return {
      // 地图数据
      dataList:[],
      // 图标配置项
      option : {
        visualMap: {
          min: 1,
          max: 1000,
        },
        title: {
          show: true,
          text: '全国',
          subtext: 'hhhh',
          left:"center",
          top: 16
        },
        //提示框组件
        tooltip: {
            show: true,
            //触发类型：数据项图形触发
            trigger: 'item',
            padding: 10,
            borderWidth: 1,
            borderColor: '#409eff',
            backgroundColor: 'rgba(255,255,255,0.4)',
            textStyle: {
              color: '#000000',
              fontSize: 12
            }
        },
        // 地理坐标组件
        geo: {
          //使用 registerMap 注册的地图名称
          map: 'chinaMap'
        }
      }
    }
  },
  methods: {
    initMap () {
      let myMapChart = echarts.init(document.getElementById('myMap'))
      myMapChart.registerMap('chinaMap', chinaJson)
      myMapChart.setOption(this.option)
    }
  },
  mounted() {
    this.initMap()
    console.log("dfdf")
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
