<template>
    <div class="docker">
      <!--
        BEM css 命名规则
        block__element(元素)--Modifier(状态)
      -->
      <div v-for="(item, index) in dockerList"
        :class="{'docker__item':true,'docker__item--active':index === 0}"
        :key="item.icon"
      >
        <router-link :to='item.to'>
          <div class="iconfont" v-html="item.icon"/>
          <div class="docker__title">{{item.text}}</div>
      </router-link>
      </div>

    </div>
</template>

<script>
export default {
  name: 'Docker',
  setup () {
    const dockerList = [
      { icon: '&#xe64d;', text: '首页', to: { name: 'Home' } },
      { icon: '&#xe7e5;', text: '购物车', to: { name: 'CartList' } },
      { icon: '&#xe62d;', text: '订单', to: { name: 'Home' } },
      { icon: '&#xe78b;', text: '我的', to: { name: 'Home' } }
    ]
    return { dockerList }
  }
}
</script>

<!--
  scoped 表示该样式只对该页面起作用
-->
<style lang="scss" scoped>
  @import '../../style/viriables.scss';
  .wrapper {
    background: red;
  }

 .docker {
    // 字体颜色
    color: $content-font-color;
    display: flex;
    position: absolute;
    /*
      box-sizing: border-box就是将border和padding数值包含在width和height之内

      box-sizing属性的取值可以为content-box或border-box
        > content-box：当定义width和height时，它的参数值不包括border和padding。
        > border-box：当定义width和height时，border和padding的参数值被包含在width和height之内。
    */
    box-sizing: border-box;
    padding: 0 0.18rem;
    left: 0;
    bottom: 0;
    width: 100%;
    height: 0.49rem;
    border-top:0.03rem solid $content-bg-color;
    // 这里&表示.docker
    &__item{
      flex: 1;
      text-align: center;
      a {
        color: $content-fontcolor;
        text-decoration: none;
      }
      .iconfont{
        font-size: 0.18rem;
        margin: 0.07rem 0 0.02rem 0;
      }
      // 这里&表示 .docker__item
      &--active{
        a {
          color:#1FA4FC;
        }
      }
    }
    // 这里&表示.docker
    &__title{
      font-size: .2rem;
      transform: scale(0.5,0.5);
      transform-origin: center top;
    }
  }
</style>
