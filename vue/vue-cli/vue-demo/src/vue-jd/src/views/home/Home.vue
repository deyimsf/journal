<!-- eslint-disable indent -->
<template>
    <!--test MyStore     {{store}} -->
    <div class="wrapper">
      <StaticPart />
      <Nearby />
    </div>

    <Docker :currentIndex="0"/>
</template>

<script>
// test MyStore
import { store } from '../../utils/MyStore'

import StaticPart from '../../views/home/StaticPart.vue'
import Nearby from '../../views/home/Nearby.vue'
import Docker from '../../components/Docker.vue'

export default {
  name: 'Home',
  components: { StaticPart, Nearby, Docker },
  setup () {
    return { store }
  }
}
</script>

<style lang="scss" scoped>
  .wrapper {
      // 纵向溢出?
      overflow-y: auto;
      position:absolute;
      left:0;
      top:0;
      bottom:.5rem;
      right: 0;
      padding: 0 0.18rem .1rem .18rem;
      // background: rgb(205, 205, 211);
  }

</style>
