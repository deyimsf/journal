<template>
    <div class="shop">
        <img :src="item.imgUrl" class="shop__img" />
        <div
            :class="{'shop__content':true, 'shop__content--bordered': hideBorder?false:true}"
        >
        <div class="shop__content__title">{{item.name}}</div>
        <div class="shop__content__tags">
            <span class="shop__content__tag">月售: {{item.sales}}</span>
            <span class="shop__content__tag">起送: {{item.expressLimit}}</span>
            <span class="shop__content__tag">基础运费: {{item.expressPrice}}</span>
        </div>
        <p class="shop__content__highlight">{{item.slogan}}</p>
        </div>
    </div>
</template>

<script>
export default {
  name: 'ShopInfo',
  props: ['item', 'hideBorder']
}
</script>

<style lang="scss" scoped>
@import '../style/viriables.scss';

.shop {
    display: flex;
    padding-top: .12rem;
    &__img{
        margin-right: .16rem;
        width: .56rem;
        height: .56rem;
    }
    &__content {
        flex:1;
        &--bordered {
            border-bottom: .001rem solid $content-bg-color;
        }
    &__title{
        line-height: .22rem;
        font-size: .16rem;
        font-size: $content-font-color;
    }
    &__tags {
        margin-top: .08rem;
        line-height: .18rem;
        font-size: .13rem;
        color: $content-font-color;
    }
    &__tag{
        margin-right: .16rem;
    }
    &__highlight{
        margin-top:.08rem 0 0 0;
        line-height:.18rem;
        font-size: .13rem;
        color: red;
        }
    }
}

</style>
