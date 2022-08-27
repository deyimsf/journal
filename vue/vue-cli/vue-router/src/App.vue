<template>
  <div id="nav">
    <!-- router-link是跳转路由的标签 -->
    <router-link to="/">Home</router-link> |
    <router-link to="/about">About</router-link>|
    <router-link to="/Login">Login</router-link>

  </div>

  <!-- router-view是展示路由内容的标签 -->
  <router-view/>
</template>

<style>
</style>
