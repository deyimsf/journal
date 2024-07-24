import asyncio
import random
import time


# 定义一个协程 调度器
async def cron_scheduler():
    page = 1
    
    while True:
        url  = '{}/{}'.format('https://www.baidu.com',page)
        # 创建协程
        job = cron_job(url)
        
        # 等待job协程执行完毕 (这里会变成顺序执行，同时运行中的job只有一个。可以用  asyncio.create_task(job)方法，把协程直接注册到事件循环中)
        # await job
        
        # 注册到事件循环
        task = asyncio.create_task(job)  # 注册到事件循环
        await asyncio.sleep(0) # 主动让出线程，让其他协程有机会执行
        
        page = page + 1
        
        

### 一个协程 用来访问一个url
async def cron_job(url):
    n = random.randint(1,2)
    
    # asyncio.sleep是标有async的异步协程，需要用await等待完成
    # 如果用 time.sleep(n) 就不需要用await，因为他不是协程，他是同步方法
    await asyncio.sleep(n)
    print('下载结束:', url)
    

# 创建协程
scheduler = cron_scheduler()


#把协程放到事件循环中执行

#############  第一种方式  ###############
# 获取事件循环
event_loop = asyncio.get_event_loop()
# 简写成放到时间循环中
result = event_loop.run_until_complete(scheduler)


#############  第二种方式  ###############
# 把协程放到时间循环中
# asyncio.run(scheduler)


