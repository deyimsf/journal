<template>
    <div class="content">
        <div class="category">
            <div
                :class="{'category__item':true, 'category__item--active': currentTab === item.tab}"
                v-for="item in categories"
                :key="item.name"
                @click="handleTabClick(item.tab)"
            >{{item.name}}</div>
        </div>

        <div class="product">
            <div class="product__item"
                v-for="(item) in list"
                :key="item._id"
            >
                <img class="product__item__img" src="https://img30.360buyimg.com/vendersettle/s120x120_jfs/t1/104763/10/17095/195874/5e83ecc2Ec39ed4d9/aaf4060a10ac1cc5.png" />
                <div class="product__item__detail">
                    <h4 class="product__item__title">{{item.name}}</h4>
                    <p class="product__item__sales">月售 {{item.sales}}</p>
                    <p class="product__item__price">
                        <span class="product__item__yen">&yen;</span>{{item.price}}
                        <span class="product__item__origin">&yen;{{item.oldPrice}}</span>
                    </p>
                </div>

                <div class="product__number">
                    <span class="product__number__minus"
                        @click="changeCartItem(shopId, item._id, item, -1, shopName)"
                    >-</span>
                    {{ getProductCartCount(shopId, item._id) }}
                    <span class="product__number__plus"
                        @click="changeCartItem(shopId, item._id, item, 1, shopName)"
                    >+</span>

                </div>
            </div>
        </div>
    </div>
</template>

<script>
import { get } from '../../utils/request'
import { reactive, toRefs, ref, watchEffect } from 'vue'
import { useRoute } from 'vue-router'
import { useStore } from 'vuex'
import { useCommonCartEffect } from '../../effects/cartEffects'
// import store from '../../store'

const categories = [
  { name: '全部商品', tab: 'all' },
  { name: '秒杀', tab: 'seckill' },
  { name: '新鲜水果', tab: 'fruit' }]

// tab切换逻辑
const useTabEffect = () => {
  const currentTab = ref(categories[0].tab)
  const handleTabClick = (tab) => {
    // watchEffect()通过getContentData()依赖currentTab,所以当currentTab值变动时getContentData()方法会被调用
    currentTab.value = tab
  }

  return { handleTabClick, currentTab }
}

// 列表内容相关逻辑
const userCurrentListEffect = (currentTab, shopId) => {
  const content = reactive({ list: [] })

  const getContentData = async (tab) => {
    const result = await get(`/api/shop/${shopId}/products`, { tab: currentTab.value })
    if (result.errno === 0 && result.data.length) {
      content.list = result.data
    }

    console.log(result)
  }

  watchEffect(() => {
    getContentData()
  })
  const { list } = toRefs(content)
  return { list }
}

const useCartEffect = () => {
  const store = useStore()
  const { cartList, changeCartItemInfo } = useCommonCartEffect()

  const changeShopName = (shopId, shopName) => {
    store.commit('changeShopName', { shopId, shopName })
  }

  const changeCartItem = (shopId, productId, item, num, shopName) => {
    changeCartItemInfo(shopId, productId, item, num)
    changeShopName(shopId, shopName)
  }

  const getProductCartCount = (shopId, productId) => {
    return cartList?.[shopId]?.productList?.[productId]?.count || 0
  }

  return { cartList, changeCartItem, getProductCartCount }
}

export default {
  name: 'Content',
  props: ['shopName'],
  setup () {
    const route = useRoute()
    const shopId = route.params.id

    const { currentTab, handleTabClick } = useTabEffect()
    const { list } = userCurrentListEffect(currentTab)
    const { changeCartItem, cartList, getProductCartCount } = useCartEffect()

    return {
      categories,
      currentTab,
      handleTabClick,
      list,
      cartList,
      shopId,
      changeCartItem,
      getProductCartCount
    }
  }

}
</script>

<style lang="scss" scoped>
@import '../../style/mixins.scss';
.content{
    display: flex;
    position: absolute;
    left: 0;
    right: 0;
    top: 1.5rem;
    bottom: .5rem;
}

.category {
    overflow-y: scroll;
    height: 100%;
    width: .76rem;
    background: #f5f5f5;
    &__item {
        line-height: .4rem;
        text-align: center;
        font-size: .14rem;
        color: #333;
        &--active {
            background: #FFF;
        }
    }
}
.product {
    overflow-y: scroll;
    flex: 1;
    &__item {
        position: relative;
        display: flex;
        padding: .12rem 0;
        margin: 0 .16rem;
        border-bottom: .01rem solid #F1F1F1;
        &__detail {
            overflow: hidden;
        }
    &__img {
      width: .68rem;
      height: .68rem;
      margin-right: .16rem;
    }
    &__title {
      margin: 0;
      line-height: .2rem;
      font-size: .14rem;
      color: #333;
      @include ellipsis;
    }
    &__sales {
      margin: .06rem 0;
      font-size: .12rem;
      color: #333;
    }
    &__price {
      margin: 0;
      line-height: .2rem;
      font-size: .14rem;
      color: #E93B3B;
    }
    &__yen {
      font-size: .12rem;
    }
    &__origin {
      margin-left: .06rem;
      line-height: .2rem;
      font-size: .12rem;
      color: #999;
      text-decoration: line-through;
    }

    .product__number {
        position: absolute;
        right: .18rem;
        bottom: .12rem;
        font-size: .16rem;
        &__minus, &__plus {
            display: inline-block;
            width: .2rem;
            height: .2rem;
            border-radius: 50%;
            font-size: .2rem;
            text-align: center;
            padding-left: .01rem;
            padding-right: .01rem;
        }
        &__minus {
            border: .01rem solid #666;
            color: #666;
            margin-right: .07rem;
            padding-bottom: .02rem;
        }
        &__plus {
            background: #0091FF;
            color: #FFF;
            margin-left: .07rem;
            padding-bottom: .05rem;
        }
    }
  }
}
</style>
