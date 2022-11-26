<template>
<div class="wrapper">
  <!-- <a href="/#/shop/1" style="height:100px; background:&red">
    aaaddfdddddddddd
    ddddddd
  </a> -->

    <div class="search">
        <div class="search__back iconfont"
        @click="handleBackClick"
        >&#xe662;</div>
        <div class="search__content">
            <span class="search__content__icon iconfont">&#xe6e1;</span>
            <input class="search__content__input"
                placeholder="请输入商品名称"
            />
        </div>
    </div>

    <ShopInfo :item="item" :hideBorder="true"/>
    <Content :shopName="item.name"/>
    <Cart />
</div>
</template>

<script>
import { reactive, toRefs } from 'vue'
import { useRouter, useRoute } from 'vue-router'
import { get } from '../../utils/request'
import ShopInfo from '../../components/ShopInfo.vue'
import Content from './Content.vue'
import Cart from './Cart.vue'

// 获取当前店铺信息
const userShopInfoEffect = () => {
  const route = useRoute()
  const data = reactive({ item: {} })
  const getItemData = async () => {
    const result = await get(`/api/shop/${route.params.id}`)
    if (result.errno === 0 && result.data) {
      data.item = result.data
    }
    console.log(result)
  }

  return { data, getItemData }
}

const useBackRouterEffect = () => {
  const router = useRouter()
  const handleBackClick = () => {
    router.back()
  }

  return handleBackClick
}

export default {
  name: 'Shop',
  components: { ShopInfo, Content, Cart },
  setup () {
    // route.params.id 中的id是在路由侧定义的(path: '/shop/:id'),
    // 如果路由侧定义的/:abc,则就应该用params.abc
    const route = useRoute()
    console.log('route.name->' + route.name, 'route.params->' + route.params.id)

    const { data, getItemData } = userShopInfoEffect()
    const handleBackClick = useBackRouterEffect()

    // 从服务端获取数据
    getItemData()

    const { item } = toRefs(data)
    return { item, handleBackClick }
  }

}
</script>

<style lang="scss" scoped>
.wrapper {
    padding: 0 .18rem;
}
.search {
    display: flex;
    height: .32rem;
    margin: .2rem 0 .04rem 0;
    line-height: .32rem;
    background: none;
    &__back {
        width: .3rem;
        font-size: .33rem;
        color: #b6b6b6;
        padding-left: .01rem;
    }
    &__content {
        display: flex;
        flex: 1;
        line-height: .32rem;
        background: #f5f5f5;
        border-radius: .16rem;
        &__icon {
            width: .44rem;
            text-align: center;
        }
        &__input {
            display: block;
            width: 100%;
            padding-right: .2rem;
            border: none;
            outline: none;
            background: none;
            height: .32rem;
            font-size: .14rem;
            color: #333;
            &::placeholder {
                color: #333;
            }
        }
    }
}
</style>
