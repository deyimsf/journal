<template>
    <div class="mask" v-if="showCart && calculations.total > 0"
        @click="handleCartShowChange"
    ></div>
    <div class="cart">
        <div class="product" v-if="showCart && calculations.total > 0">
            <div class="product__header">
                <div class="product__header__all"
                    @click="setCartItemsChecked(shopId)"
                >
                    <span class="product__header__icon iconfont"
                        v-html="(calculations.allChecked ? '&#xe70f;': '&#xe66c;')"
                    ></span>
                    全选
                </div>
                <div class="product__header__clear">
                    <span @click="clearCartProducts(shopId)"
                        class="product__header__clear__btn"
                    >清空购物车</span>
                </div>
            </div>
            <template
                v-for="(item, index) in productList"
                :key="index"
            >
                <div class="product__item" v-if="item.count > 0">
                    <div class="product__item__checked iconfont"
                        v-html="(item.check ? '&#xe70f;': '&#xe66c;')"
                        @click="changeCartItemChecked(shopId, item._id)"
                    >
                </div>

                    <img class="product__item__img" src="https://img30.360buyimg.com/vendersettle/s120x120_jfs/t1/104763/10/17095/195874/5e83ecc2Ec39ed4d9/aaf4060a10ac1cc5.png" />
                    <div class="product__item__detail">
                        <h4 class="product__item__title">{{item.name}}</h4>
                        <p class="product__item__price">
                            <span class="product__item__yen">&yen;</span>{{item.price}}
                            <span class="product__item__origin">&yen;{{item.oldPrice}}</span>
                        </p>
                    </div>

                    <div class="product__number">
                        <span class="product__number__minus"
                            @click="changeCartItemInfo(shopId, item._id, item, -1)"
                        >-</span>
                        {{ item.count || 0}}
                        <span class="product__number__plus"
                            @click="changeCartItemInfo(shopId, item._id, item, 1)"
                        >+</span>

                    </div>
                </div>
            </template>
        </div>

        <div class="check">
            <div class="check__icon">
                <img src="https://img30.360buyimg.com/vendersettle/s120x120_jfs/t1/104763/10/17095/195874/5e83ecc2Ec39ed4d9/aaf4060a10ac1cc5.png"
                    class="check__icon__img"
                    @click="handleCartShowChange"
                />
                <div class="check__icon__tag">{{ calculations.total }}</div>
            </div>
            <div class="check__info">
                 总计: <span class="check__info__price">&yen; {{calculations.price}}</span>
            </div>
            <div class="check__btn" v-show="calculations.total > 0">
              <router-link :to="`/orderConfirmation/${shopId}`">
                去结算
              </router-link>
            </div>
        </div>
    </div>>
</template>

<script>
import { useStore } from 'vuex'
import { useRoute } from 'vue-router'
import { useCommonCartEffect } from '../../effects/cartEffects'
import { ref } from 'vue'

const useCartEffect = (shopId) => {
  const { productList, changeCartItemInfo, calculations } = useCommonCartEffect(shopId)

  const store = useStore()

  const changeCartItemChecked = (shopId, productId) => {
    store.commit('changeCartItemChecked', { shopId, productId })
  }

  const clearCartProducts = (shopId) => {
    store.commit('clearCartProducts', { shopId })
  }

  const setCartItemsChecked = (shopId) => {
    store.commit('setCartItemsChecked', { shopId })
  }

  return {
    calculations,
    productList,
    changeCartItemInfo,
    changeCartItemChecked,
    clearCartProducts,
    setCartItemsChecked
  }
}

const toggleCartEffect = () => {
  const showCart = ref(false)
  const handleCartShowChange = () => {
    showCart.value = !showCart.value
  }
  return { showCart, handleCartShowChange }
}

export default {
  name: 'Cart',
  setup () {
    const route = useRoute()
    const shopId = route.params.id

    const {
      calculations, productList,
      changeCartItemInfo, changeCartItemChecked, clearCartProducts,
      setCartItemsChecked
    } = useCartEffect(shopId)

    const { showCart, handleCartShowChange } = toggleCartEffect()

    return {
      calculations,
      shopId,
      productList,
      changeCartItemInfo,
      changeCartItemChecked,
      clearCartProducts,
      setCartItemsChecked,
      showCart,
      handleCartShowChange
    }
  }
}
</script>

<style lang="scss" scoped>
@import '../../style/viriables.scss';
@import '../../style/mixins.scss';

.mask {
    position: fixed;
    left: 0;
    right: 0;
    bottom: 0;
    top: 0;
    background: rgba(0,0,0,.5);
    z-index: 1;
}

.cart {
  position: absolute;
  left: 0;
  right: 0;
  bottom: 0;
  z-index: 2;
  background: #FFF;
}

.product {
    overflow-y: scroll;
    flex: 1;
    background: #fff;
    &__header {
        display: flex;
        line-height: .52rem;
        height: .52rem;
        border-bottom: .01rem solid #F1F1F1;
        &__all {
            width: .64rem;
            margin-left: .18rem;
        }
        &__icon {
            display: inline-block;
            line-height: .2rem;
            color: #0091FF;
            font-size: .2rem;
        }
        &__clear {
            flex: 1;
            margin-right: .16rem;
            text-align: right;
            color: #333;
            &__btn {
                display: inline-block;
            }
        }
    }
    &__item {
        position: relative;
        display: flex;
        padding: .12rem 0;
        margin: 0 .16rem;
        border-bottom: .01rem solid #F1F1F1;
        &__detail {
            overflow: hidden;
        }
        &__checked {
            line-height: .5rem;
            margin-right: .2rem;
            color: #0091FF;
            font-size: .2rem;
        }
    &__img {
      width: .46rem;
      height: .46rem;
      margin-right: .16rem;
    }
    &__title {
      margin: 0;
      line-height: .2rem;
      font-size: .14rem;
      color: #333;
      @include ellipsis;
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
        bottom: .25rem;
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

.check {
  display: flex;
  height: .49rem;
  border-top: .01rem solid $content-bgColor;
  line-height: .49rem;
  &__icon {
    position: relative;
    width: .84rem;
    &__img {
      margin: .12rem .19rem;
      width: .38rem;
      height: .36rem;
    }
    &__tag {
      position: absolute;
      left: .43rem;
      top: .04rem;
      padding: 0 .04rem;
      min-width: .2rem;
      height: .2rem;
      line-height: .2rem;
      background-color: $hightlight-fontColor;
      border-radius: .1rem;
      font-size: .12rem;
      text-align: center;
      color: #fff;
      transform: scale(.5);
      // 以左侧中间作为旋转点
      transform-origin: left center;
    }
  }
  &__info {
    flex: 1;
    color: $content-fontcolor;
    font-size: .12rem;
    &__price {
      margin: .06rem 0 0 0;
      line-height: .49rem;
      color: $hightlight-fontColor;
      font-size: .18rem;
    }
  }
  &__btn {
    width: .98rem;
    background-color: #4FB0F9;
    text-align: center;
    color: #FFF;
    font-size: .14rem;
    a {
      color: #fff;
      text-decoration: none;
    }
  }
}
</style>
