import { createStore } from 'vuex'
import axios from 'axios';

export default createStore({
  state: {name:'dell'},

  mutations: {
    change456(state,value){
      state.name = value;
    }
  },
  actions: {
    change123(store){
      axios.get('http://localhost:8080/8baf71bfa9b1ab1a28d9.hot-update.json')
      .then((response)=>{
         console.log(response.data);
         store.commit("change456",response.data.h);
       });


       setTimeout(()=>{
        store.commit("change456","hhhhhh");
      },2000);
    }
  },
  modules: {
  }
})
