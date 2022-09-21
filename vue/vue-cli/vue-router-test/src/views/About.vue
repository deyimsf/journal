<template>
  <div class="about">
    <h1>This is an about page</h1>
    <h2>{{ name }}</h2>
    
    <h2>{{ nameObj.name }}</h2>
  </div>
</template>

<script>
import { reactive, ref } from 'vue'
export default {
  name: 'About',
  setup  () {
    const name = ref('zhangsan')
    const nameObj = reactive({
      name: 'lisi'
    })

    return { name, nameObj }
  }
}
</script>