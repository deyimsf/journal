<template>
    <div>
    <div class="wrapper">
        <img class="wrapper__img" src="https://img30.360buyimg.com/mobilecms/jfs/t1/173730/23/1250/10393/60642179E8ac04847/266e4ae9b63903dc.png"/>
        <div class="wrapper__input">
            <input
              class="wrapper__input__content"
              placeholder="请输入手机号"
              v-model="mobile"
            />
        </div>
        <div class="wrapper__input">
            <input class="wrapper__input__content"
              placeholder="请输入密码" type="password"
              v-model="password"
              autocomplete="new-password"
            />
        </div>
        <div class="wrapper__login-button" @click="handleLogin">登录</div>
        <div class="wrapper__login-link" @click="handleRegister">立即注册</div>
        <Toast v-if="show" :message="toastMessage"/>

        <!--test MyStore-->
        {{ store.name }}
    </div>
</div>
</template>

<script>
import { useRouter } from 'vue-router'
import { post } from '../../utils/request'
import { reactive, toRefs } from 'vue'
import Toast, { useToastEffect } from '../../components/Toast.vue'

// test MyStore
import { store } from '../../utils/MyStore'

// 处理登录逻辑
const userLoginEffect = (showToast) => {
  const router = useRouter()
  const data = reactive({
    mobile: '',
    password: ''
  })

  const handleLogin = async () => {
    try {
      const result = await post('/api/user/login', {
        username: data.mobile,
        password: data.password
      })

      if (result.errno === 0) {
        localStorage.isLogin = true
        // 登录后路由到Home
        router.push({ name: 'Home' })
      } else {
        showToast('登录失败')
      }
    } catch (e) {
      showToast('请求失败')
    }
  }

  const { username, password } = toRefs(data)

  return { username, password, handleLogin }
}

// 处理注册跳转
const userRegisterEffect = () => {
  const router = useRouter()

  const handleRegister = () => {
    router.push({ name: 'Register' })
  }

  return { handleRegister }
}

export default {
  name: 'Login',
  components: { Toast },
  setup () {
    // test MyStore
    store.name = 'lee'

    const { show, toastMessage, showToast } = useToastEffect()
    const { username, password, handleLogin } = userLoginEffect(showToast)
    const { handleRegister } = userRegisterEffect()

    return { handleLogin, handleRegister, username, password, store, show, toastMessage }
  }
}
</script>

<style lang="scss" scoped>
  .wrapper {
    position: absolute;
    top: 50%;
    left: 0;
    right: 0;
    transform: translateY(-50%);
    &__img {
        display: block;
        margin: 0 auto 0.4rem auto;
        width: .66rem;
        height: .66rem;
    }
    &__input {
        height: .48rem;
        margin: 0 .4rem .16rem .4rem;
        padding: 0 .16rem;
        background: #F9F9F9;
        border: .01rem solid rgba(0,0,0,0.10);
        border-radius: .06rem;
        border-radius: .06rem;
        &__content {
            line-height: .48rem;
            border: none;
            outline: none;
            width: 100%;
            background: none;
            font-size: .16rem;
            color: rgba(0,0,0,0.50);
            &::placeholder{
                color: rgba(0,0,0,0.50)
            }
        }
    }
    &__login-button {
        margin: .32rem .4rem .16rem .4rem;
        line-height: .48rem;
        background: #0091FF;
        box-shadow: 0 .04rem 0.08rem 0 rgba(0,145,255,0.32);
        border-radius: 0.04rem;
        border-radius: 0.04rem;
        color: #fff;
        font-size: .16rem;
        text-align: center;
    }
    &__login-link {
        text-align: center;
        font-size: .14rem;
        color: #777;
    }
  }
</style>
