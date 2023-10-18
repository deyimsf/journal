import time
import math
import os.path

import requests
import json
from bs4 import BeautifulSoup

headers = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/108.0.0.0 Safari/537.36",
    "Referer": "https://bj.meituan.com/meishi/193935766/",
    "Cookie": "uuid=d5d3d8c40dc1430da32d.1671158144.1.0.0; _lxsdk_cuid=18367c0ee54c8-0c0203decdec86-26021c51-384000-18367c0ee54c8; ci=1; __mta=146062219.1671158570467.1671158570467.1671158570467.1; IJSESSIONID=node0nn8cxg1o4oaqgiq5ad4nd0z548906390; iuuid=6A9ADA013A282510EDC8C6261DBC7066CA405BE8AB1E65B2EADD7627157DFD48; cityname=%E5%8C%97%E4%BA%AC; _lxsdk=6A9ADA013A282510EDC8C6261DBC7066CA405BE8AB1E65B2EADD7627157DFD48; client-id=82e73353-56fd-4d28-b394-1d75bddb8218; mtcdn=K; userTicket=fyrpLBuWkbdTbcBcqmjWeAylgcVLYcCYumfqqhYk; u=6483480; n=dboylx; m=q666q%40163.com; lt=AgF0IVThKKxGCkZrGPOPnFYVEWjOtWjP4KvMNFva7PBZdAxgUpIKt46R0MMN9GHPKnufr2qP5qViMAAAAACfFQAASMGLH2UWB87qR9Bddl8ApzsYR5gOZ4NRojds_tMIKCnW-vmvV7tIEG6KAmCwp9Tk; mt_c_token=AgF0IVThKKxGCkZrGPOPnFYVEWjOtWjP4KvMNFva7PBZdAxgUpIKt46R0MMN9GHPKnufr2qP5qViMAAAAACfFQAASMGLH2UWB87qR9Bddl8ApzsYR5gOZ4NRojds_tMIKCnW-vmvV7tIEG6KAmCwp9Tk; token=AgF0IVThKKxGCkZrGPOPnFYVEWjOtWjP4KvMNFva7PBZdAxgUpIKt46R0MMN9GHPKnufr2qP5qViMAAAAACfFQAASMGLH2UWB87qR9Bddl8ApzsYR5gOZ4NRojds_tMIKCnW-vmvV7tIEG6KAmCwp9Tk; token2=AgF0IVThKKxGCkZrGPOPnFYVEWjOtWjP4KvMNFva7PBZdAxgUpIKt46R0MMN9GHPKnufr2qP5qViMAAAAACfFQAASMGLH2UWB87qR9Bddl8ApzsYR5gOZ4NRojds_tMIKCnW-vmvV7tIEG6KAmCwp9Tk; unc=dboylx; WEBDFPID=zw891488uz9950v9y0xvxz277x6x54x181481w8188w97958zxyyu825-1986518658993-1671158658836GOSESKGfd79fef3d01d5e9aadc18ccd4d0c95071536; _hc.v=df200bd0-582e-2c76-8dd4-0d26bfa8d8f5.1671158659; isid=AgF0IVThKKxGCkZrGPOPnFYVEWjOtWjP4KvMNFva7PBZdAxgUpIKt46R0MMN9GHPKnufr2qP5qViMAAAAACfFQAASMGLH2UWB87qR9Bddl8ApzsYR5gOZ4NRojds_tMIKCnW-vmvV7tIEG6KAmCwp9Tk; oops=AgF0IVThKKxGCkZrGPOPnFYVEWjOtWjP4KvMNFva7PBZdAxgUpIKt46R0MMN9GHPKnufr2qP5qViMAAAAACfFQAASMGLH2UWB87qR9Bddl8ApzsYR5gOZ4NRojds_tMIKCnW-vmvV7tIEG6KAmCwp9Tk; logintype=normal; lat=40.015304; lng=116.271054; firstTime=1671160254266; _lx_utm=utm_source%3DBaidu%26utm_medium%3Dorganic; _lxsdk_s=1851920a961-5a8-3f4-503%7C%7C1",
    "Accept-Language": "zh,en;q=0.9,zh-CN;q=0.8",
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9"
}

def getDetail(url):
    print(url)
    time.sleep(2.4)

    res = requests.get(url, headers=headers)
    res = res.text
    res = res[res.find("window._appState ="):]
    res = res[res.find("{"): res.find(";</script>")]
    print(res)
    obj = json.loads(res)
    f = open("shop/" + obj["poiId"], "w", encoding='utf-8')
    str = json.dump(obj["detailInfo"], f, ensure_ascii=False)


def getEatchDetail(arr):
    for i in arr:
        if not os.path.exists("shop/" + str(i)):
            url = "https://www.meituan.com/meishi/"+str(i)+"/"
            getDetail(url)

def getList(url):
    print(url)
    res = requests.get(url, headers=headers)
    res = res.text
    res = res[res.find("window._appState ="):]
    res = res[res.find("{"): res.find(";</script>")]
    obj = json.loads(res)

    print(obj["poiLists"])
    total = obj["poiLists"]["totalCounts"]
    list = obj["poiLists"]["poiInfos"]
    arr = []
    for item in list :
        arr.append(item["poiId"])

    return (arr, total)


def getAllPage(url):
    (arr,total) = getList(url)
    getEatchDetail(arr)

    for i in range(2, math.ceil(total/15)):
        (r,_) = getList(url+"pn"+ str(i)+"/")
        arr.extend(r)
        getEatchDetail(r)
    return arr

# getDetail('https://www.meituan.com/meishi/6824520/')


arr = getAllPage("https://bj.meituan.com/meishi/c63/")
print(arr)