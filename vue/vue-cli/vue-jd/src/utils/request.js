import axios from 'axios'

const instance = axios.create({
  baseURL: 'https://www.fastmock.site/mock/4f1dcdae75d82ea90b3cdd39d1a831e9/api',
  timeout: 10000
})

export const get = (url, params = {}) => {
  return new Promise((resolve, reject) => {
    instance.get(url, { params }).then((response) => {
      // 只返回接口数据,忽略axios给封装的数据
      resolve(response.data)
    }).catch((err) => {
      reject(err)
    })
  })
}

export const post = (url, data = {}) => {
  return new Promise((resolve, reject) => {
    axios.post(url, data, {
      baseURL: 'https://www.fastmock.site/mock/4f1dcdae75d82ea90b3cdd39d1a831e9/api',
      Headers: {
        'Content-Type': 'application/json'
      }
    }).then((response) => {
      // 只返回接口数据,忽略axios给封装的数据
      resolve(response.data)
    }).catch((err) => {
      reject(err)
    })
  })
}
