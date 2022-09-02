<template>
    <div>
    <div class="wrapper">
        <img class="wrapper__img" src="https://img30.360buyimg.com/mobilecms/jfs/t1/173730/23/1250/10393/60642179E8ac04847/266e4ae9b63903dc.png"/>
        <div class="wrapper__input">
            <input class="wrapper__input__content" placeholder="请输入用户名"
                v-model="username"
            />
        </div>
        <div class="wrapper__input">
            <input class="wrapper__input__content" placeholder="请输入密码" type="password"
            autocomplete="new-password"
            v-model="password"
        />
        </div>
        <div class="wrapper__input">
            <input class="wrapper__input__content" placeholder="确认密码" type="password"
                autocomplete="new-password"
                v-model="ensurement"
            />
        </div>
        <div class="wrapper__register-button" @click="handleRegister">注册</div>
        <div class="wrapper__register-link" @click="handleLoginClick">已有账号登录</div>

        <Toast v-if="show" :message="toastMessage"/>
    </div>
</div>
</template>

<script>
import { useRouter } from 'vue-router'
import { reactive, toRefs } from 'vue'
import { post } from '../../utils/request'
import Toast, { useToastEffect } from '../../components/Toast.vue'

// 处理注册相关逻辑
const userRegisterEffect = (showToast) => {
  const router = useRouter()
  const data = reactive({
    username: '',
    password: '',
    ensurement: ''
  })

  const handleRegister = async () => {
    try {
      const result = await post('/api/user/register', {
        username: data.mobile,
        password: data.password
      })

      if (result.errno === 0) {
        router.push({ name: 'Login' })
      } else {
        showToast('注册失败')
      }
    } catch (e) {
      showToast('注册异常')
    }
  }

  const { username, password, ensurement } = toRefs(data)

  return { username, password, ensurement, handleRegister }
}

// 处理登录跳转
const userLoginEffect = () => {
  const router = useRouter()

  const handleLoginClick = () => {
    router.push({ name: 'Login' })
  }

  return { handleLoginClick }
}

export default {
  name: 'Register',
  components: { Toast },
  setup () {
    const { show, toastMessage, showToast } = useToastEffect()
    const { username, password, ensurement, handleRegister } = userRegisterEffect(showToast)
    const { handleLoginClick } = userLoginEffect()

    return { username, password, ensurement, handleRegister, handleLoginClick, show, toastMessage }
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
    &__register-button {
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
    &__register-link {
        text-align: center;
        font-size: .14rem;
        color: #777;
    }
  }
</style>
