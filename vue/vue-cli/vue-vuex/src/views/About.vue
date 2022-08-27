<template>
  <div class="about">
    <h1 @click="handleClick">This is an about page</h1>
    <h1> {{myName}} -- {{aa}}</h1>
  </div>
</template>


<script>
export default {
  name: 'About',
  data(){
    return {aa: this.$store.state.name}
  },
  computed:{
    myName(){
      return this.$store.state.name;
    }
  },
  methods:{
    handleClick(){
      // 1.想改变数据,vueX要求第一步必须派发(触发)一个action(动作)
      // 2.通过this.$store.dispatch('change123');会触发store.actions中的change123方法
      // 3.change123方法中,再提交一个commit: 
      //      this.commit("change456"); 
      //    触发一个 mutation(mutations中的一个方法)
      // 4.在mutaions的change456方法中才能修改数据
      this.$store.dispatch('change123'); // change123是store.actions中的一个方法
     
      // 不需要异步也可以这么写
      this.$store.state.name = 'fdsfd';

    }
  }
}
</script>
