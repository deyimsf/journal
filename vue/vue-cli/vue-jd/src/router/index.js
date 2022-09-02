import { createRouter, createWebHashHistory } from 'vue-router'

const routes = [
  {
    path: '/',
    name: 'Home',
    component: () => import('../views/home/Home')
  },
  {
    path: '/cartList',
    name: 'CartList',
    component: () => import('../views/cartlist/CartList.vue')
  },
  {
    path: '/orderConfirmation/:id',
    name: 'OrderConfirmation',
    component: () => import(/* webpackChunkName: "OrderConfirmation" */ '../views/orderconfirmation/OrderConfirmation.vue')
  },
  {
    path: '/orderList',
    name: 'OrderList',
    component: () => import(/* webpackChunkName: "OrderList" */ '../views/orderlist/OrderList.vue')
  },
  {
    path: '/shop/:id',
    name: 'Shop',
    component: () => import(/* webpackChunkName: "shop_123123" */ '../views/shop/Shop')
  },
  {
    path: '/register',
    name: 'Register',
    component: () => import(/* webpackChunkName: "register_123123" */ '../views/register/Register'),
    beforeEnter (to, from, next) {
      const isLogin = localStorage.isLogin
      if (isLogin) {
        next({ name: 'Home' })
      } else {
        next()
      }
    }
  },
  {
    path: '/login',
    name: 'Login',
    component: () => import(/* webpackChunkName: "login_123123" */ '../views/login/Login'),
    beforeEnter (to, from, next) {
      const isLogin = localStorage.isLogin
      if (isLogin) {
        // 如果已经登陆了就直接去Home页
        next({ name: 'Home' })
      } else {
        next()
      }
    }
  }
]

const router = createRouter({
  history: createWebHashHistory(),
  routes
})

// 每次切换路由都会执行
router.beforeEach((to, from, next) => {
  const isLogin = localStorage.isLogin
  const { name } = to
  const isLoginOrRegister = (name === 'Login' || name === 'Register')
  if (isLogin || isLoginOrRegister) {
    next()
  } else {
    next({ name: 'Login' })
  }
})

export default router
