<template>
    <div class="position">
        <span class="iconfont position__icon">&#xe61f;</span>
     北京理工大学国防科技园2号楼10层北京理工大学国防科技园2号楼10层北京理工大学国防科技园2号楼10层北京理工大学国防科技园2号楼10层
        <span class="iconfont position__notice">&#xe7c4;</span>
      </div>

      <div class="search">
        <span class="iconfont ">&#xe6e1;</span>
        <input class="search__text" type="text" value="山姆会员商品优惠商品"/>
      </div>

      <div class="banner">
        <img class="banner__img" src="https://m15.360buyimg.com/mobilecms/s1528x388_jfs/t1/219374/37/9895/379975/61d409aaE455d3578/168ff5869a75742f.jpg" />
      </div>

      <div class="icons">
          <div class="icons__item"
              v-for="item in iconsList"
              :key="item"
          >
            <img class="icons__item__img" :src="item.imgUrl"/>
            <p class="icons__item__desc">{{item.desc}}</p>
          </div>
      </div>

      <div class="gap"></div>
</template>

<script>
export default {
  name: 'StaticPart',
  setup () {
    const iconsList = [
      { desc: '酒水饮料', imgUrl: 'https://img30.360buyimg.com/mobilecms/jfs/t1/173730/23/1250/10393/60642179E8ac04847/266e4ae9b63903dc.png' },
      { desc: '生鲜果蔬', imgUrl: 'https://img30.360buyimg.com/mobilecms/jfs/t1/85658/4/23437/11783/6231502bEb825de77/2cc97d34b154acc7.png' },
      { desc: '送药上门', imgUrl: 'https://img30.360buyimg.com/mobilecms/jfs/t1/181667/35/6075/10202/60ade8d6E2067b9da/3a48a522ea152272.png' },
      { desc: '母婴', imgUrl: 'https://img30.360buyimg.com/mobilecms/jfs/t1/183326/17/25803/9501/62a2fe29E9c19f9a1/9fb31fefde8adc5a.png' },
      { desc: '个洗家清', imgUrl: 'https://img30.360buyimg.com/mobilecms/jfs/t1/175687/6/5265/8891/607cebf4E00fa2549/bffe07ca1dff3a62.png' },
      { desc: '粮油副食', imgUrl: 'https://img30.360buyimg.com/mobilecms/jfs/t1/188383/30/7677/9386/60c308b2E2b4b259f/45d803a575430c43.png' },
      { desc: '乳制品', imgUrl: 'https://img30.360buyimg.com/mobilecms/jfs/t1/170117/14/17884/8232/6073c058E860b7549/4608bb5de6206c3a.png' },
      { desc: '手机家电', imgUrl: 'https://img30.360buyimg.com/mobilecms/jfs/t1/159406/27/1106/9009/5fefa7e5E63f2e1c7/060bb24dfd00503d.png' },
      { desc: '超市', imgUrl: 'https://img30.360buyimg.com/mobilecms/jfs/t1/152758/37/12989/8884/5fefa67cE1244a6ad/6495bed0f22bbde8.png' },
      { desc: '美妆护肤', imgUrl: 'https://img30.360buyimg.com/mobilecms/jfs/t1/184985/2/8411/9355/60c185fdE41f7b5d3/2225851aa2899ede.png' }
    ]

    return { iconsList }
  }
}
</script>

<style lang="scss">
  @import '../../style/viriables.scss';
  @import '../../style/mixins.scss';

  .position{
    // // 不换行
    // white-space: nowrap;
    // // 太长就隐藏
    // overflow: hidden;
    // text-overflow: ellipsis;
    // ellipsis这是自己定义的一个mixin,封装了上面3行css
    @include ellipsis;
    position: relative;
    padding: 0.16rem .24rem 0.16rem 0;
    // background: #e8eaeb;
    line-height: 0.22rem;
    font-size: .16rem;
    // 字体颜色,用样式变量赋值
    color: $content-font-color;
    .position__icon {
      position: relative;
      top: .01rem;
      font-size: .2rem;
    }
    .position__notice{
      position: absolute;
      right: 0;
      top:0.17rem;
      font-size: 0.22rem;
    }
  }

  .search {
    margin-bottom: .12rem;
    line-height: .32rem;
    background: #f5f5f5;
    color: #b7b7b7;
    border-radius: .16rem;
    .iconfont{
      position: relative;
      display: inline-block;
      font-size: .2rem;
      top: 0.02rem;
      padding: 0 .06rem 0 .13rem;
    }
    &__text {
      display: inline-block;
      font-size: .14rem;
      border: 0;
      background: #f5f5f5;
      color: #b7b7b7;
      width: 80%;
      height: 100%;
    }
  }

  .banner {
    // height: 0;
    // overflow: hidden;
    // padding-bottom: 39.4%;
    &__img {
      width: 100%;
      border-radius:0.15rem;
    }
  }

  .icons {
    display: flex;
    flex-wrap: wrap;
    &__item {
      width: 20%;
      margin-top: .16rem;
      &__img {
        display: block;
        width:.4rem;
        height: .4rem;
        margin: 0 auto;
      }
      &__desc {
        text-align: center;
        margin: 0.06rem 0 .16rem 0;
        color: $content-font-color;
      }
    }
  }

  .gap {
    margin: 0 -0.18rem;
    height: .1rem;
    background: $content-bg-color;
  }

</style>
