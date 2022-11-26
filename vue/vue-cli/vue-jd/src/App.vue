<template>
  <router-view />
</template>

<script>
// import { bbb } from './utils/MyStore1'
// bbb()
import Myclass from './utils/MyClass'

const myss = new Myclass()
console.log(myss.name)

export default {
  name: 'App'
}
</script>
