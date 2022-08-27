import { createStore } from 'vuex'

// VueX 数据管理框架
// VueX 创建了一个全局唯一的仓库,用来存放全局的数据
export default createStore({
  state: {
    name:'dell'
  },
  // mutation里只允许写同步代码,不能写异步代码;   将来同步代码封装可以都放到这里 
  mutations: {
    // 4.通过this.commit("change456");会触发该方法
    change456(){
      // 5.在mutaion中修改数据
      this.state.name = 'lee';
    }
  },
  // 这里可以写异步代码;  将来异步代码封装可以都放到这里
  actions: {
    // 2.通过this.$store.dispatch('change123');会触发该方法
    change123(){
      // 3.提交一个commit,触发一个 mutation(mutations中的一个方法)
      this.commit("change456");
    }
  },
  modules: {
  }
})
