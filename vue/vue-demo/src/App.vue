<div>{{ dataA }}</div>
<button type="button" v-on:click="test()">button</button>

<template>
  <div>
    <button type="button" v-on:click="connectWallet">链接小狐狸</button><br>
  </div>
  <div>
    <label>购买金额</label> <input v-model="contractBuyValue" type="text" placeholder="请输入金额"/>
    <button v-on:click="contractBuy">购买</button>
  </div>

  <div>
    <label>地址:</label>
    <input type="text" v-model="accountAddress" placeholder="请输入地址"/>
    <button @click="getEthBalance">查询该地址余额</button>
  </div>
  <br>
  <div>
    <input type="text" v-model="addValue" placeholder="请输入整数"/>
    <button @click="contractAdd">调用合约的加法</button>
  </div>
</template>

<script>
import Web3 from 'web3'
var web3 = new Web3(window.ethereum);
var currentConnectAddress = null; // 当前链接的钱包地址

//要调用的合约abi
let contractAbi = [{"inputs":[{"internalType":"uint256","name":"num","type":"uint256"}],"name":"add","outputs":[{"internalType":"uint256","name":"","type":"uint256"}],"stateMutability":"nonpayable","type":"function"},{"inputs":[{"internalType":"uint256","name":"count","type":"uint256"}],"name":"buy","outputs":[],"stateMutability":"payable","type":"function"},{"inputs":[],"stateMutability":"nonpayable","type":"constructor"},{"anonymous":false,"inputs":[{"indexed":false,"internalType":"string","name":"str","type":"string"},{"indexed":false,"internalType":"address","name":"addr","type":"address"}],"name":"logAddr","type":"event"},{"anonymous":false,"inputs":[{"indexed":false,"internalType":"string","name":"str","type":"string"},{"indexed":false,"internalType":"uint256","name":"num","type":"uint256"}],"name":"logNum","type":"event"},{"stateMutability":"payable","type":"fallback"},{"stateMutability":"payable","type":"receive"}]
//要调用的合约地址
let contractAddress = '0x626cBcaAD4a88017489155126565bE0939bb5A37';
//要调用的合约
let contract = new web3.eth.Contract(contractAbi,contractAddress);


// ----------------调用合约的buy方法------------
function contractBuy() {
  let buyValue = this.contractBuyValue;
  if(buyValue < 0){
    alert("请输出正整数")
  }

  // 转换单位
  buyValue = web3.utils.toWei(buyValue,'ether');

  // 调用合约方法，买东西
  contract.methods.buy(buyValue).send(
      {from:'0xdb8963dd8a26f3cd2bbed4735404caddcc7628d5', value:buyValue}
  ).then(result => {
    console.log(result);
  }).catch(
      err => alert(err)
  )
}


// ------------调用合约的加法---------
function contractAdd(){
  let num = this.addValue;
  if(num < 0 || num == undefined){
    alert("请输出正整数")
    return;
  }

  // 调用合约方法,但是call不能更改合约状态 --TODO
  contract.methods.add(num).call({from:'0xdb8963dd8a26f3cd2bbed4735404caddcc7628d5',value:0}).then(result => {
    alert(result)
    console.log(result);
  })
}

// ------------连接钱包--------------
async function connectWallet (){
   if(window.ethereum ==='undefined'){
     alert("请安装metamask");
     return;
   }

  // window.ethereum.enable();
  await window.ethereum.request({method:'eth_requestAccounts'}).then(
      accounts => {
        currentConnectAddress = accounts[0];
        console.log(accounts[0])
      }
  )

  alert("当前地址:["+currentConnectAddress+"]")
}

// ------------------查询地址余额-------
function getEthBalance(){
  let addr = this.accountAddress;
  if (addr == null || addr === 'undefined') {
    alert("地址不能是空");
    return;
  }

  if(!web3.utils.isAddress(addr)){
    alert("非法的地址");
    return;
  }

  // 查询余额
  web3.eth.getBalance(addr).then(
     function (balance) { // 余额
       let balanceEth = web3.utils.fromWei(balance,'ether');
        alert("地址: "+addr+"\n余额: "+balanceEth+"eth")
      }
  ).catch( // 发生异常
      err => alert("发生异常: "+err)
  )
}



export default {
  name: 'App',
  methods:{
    connectWallet, getEthBalance,contractAdd,contractBuy
  },
  data(){
     return {
       currentConnectAddress:'-----',
     }
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}
</style>
