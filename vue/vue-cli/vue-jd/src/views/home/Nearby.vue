<template>
    <div class="nearby">
        <h3 class="nearby__title">附近店铺</h3>
        <RouterLink v-for="item in nearbyList" :key="item._id"
              :to="`/shop/${item._id}`"
        >
          <ShopInfo :item="item"/>
        </RouterLink>
    </div>
</template>

<script>
import { ref } from 'vue'
import { get } from '../../utils/request'
import ShopInfo from '../../components/ShopInfo.vue'

const userNearbyListEffect = () => {
  const nearbyList = ref([])
  const getNearbyList = async () => {
    const result = await get('/api/shop/hot-list')
    if (result.errno === 0 && result.data.length) {
      nearbyList.value = result.data
    }
    console.log(result)
  }
  return { nearbyList, getNearbyList }
}

export default {
  name: 'Nearby',
  components: { ShopInfo },
  setup () {
    const { nearbyList, getNearbyList } = userNearbyListEffect()
    getNearbyList()

    return { nearbyList }
  }
}
</script>

<style lang="scss">
  @import '../../style/viriables.scss';

  .nearby {
    &__title {
      margin: .16rem 0 .02rem 0;
      font-size: .18rem;
      font-weight: normal;
      color: $content-font-color;
    }
  }

  a {
    text-decoration: none;
  }
  </style>
