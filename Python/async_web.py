from concurrent.futures import ThreadPoolExecutor
from fastapi import FastAPI,Request,Response
import uvicorn #web服务器，类似tomcat
import asyncio #python中异步io工具
import time


# 创建web应用
app = FastAPI() #类似servlet,springmvc等框架

# 创建一个线程池: 对于不可异步的io操作,可以将任务放入到线程池中操作(这样多核cpu可以被利用上)
threadPool = ThreadPoolExecutor(max_workers=200)

# 定义一个get类型 http接口,入口是 /ver?
@app.get('/abc/ver1')
async def ver2(req: Request): #这是一个协程
    # 获取参数
    msg = req.query_params.get('msg')

    # 获取主线程中的事件异步io
    loop = asyncio.get_event_loop()

    # 定义一个任务
    task = {
        'msg': msg, #从客户端接收的参数
        'event': asyncio.Event(), #用它来实现自定义wait通知
        'result': None #处理好的结果放这里
    }
    # 处理任务：把入参转字符转换成大写字符
    # 任务中不要放同步io操作(如果执行同步io，则会快速消耗掉线程池中的线程，并降低并发性)
    def handle_task():
        print('task received:', task['msg'])
        # 转换大写
        task['result'] = task['msg'].upper()
        # 模拟任务处理时间
        time.sleep(2)

        # 任务处理完成后，将事件唤醒并通知主线程
        def async_callback():
            print('task ends notified:', task['result'])
            task['event'].set() #唤醒事件
        # 用线程安全的方式，将事件注册到主线程
        loop.call_soon_threadsafe(async_callback)

    # 提交任务并等待结果
    threadPool.submit(handle_task)
    await task['event'].wait()

    # 任务处理完毕，给客户端返回结果
    return Response(task['result'])


# 另一种方式
@app.get('/abc/ver2')
async def ver2(req: Request):
    # 获取参数
    msg = req.query_params.get('msg')
    # 获取事件循环
    loop = asyncio.get_event_loop()

    # 任务
    task = {
        'msg': msg
    }

    # 任务中不要放同步io操作(如果执行同步io，则会快速消耗掉线程池中的线程，并降低并发性)
    def handle_task():
        print('task received:', task['msg'])
        result = task['msg'].upper()
        time.sleep(2)

        # 这里不用asyncio.Event()来唤醒事件了，直接返回
        return result

    # 用异步方法把任务提交到线程池
    result = await loop.run_in_executor(threadPool, handle_task)
    print('task ends notified:',result,asyncio.get_event_loop())
    # 返回结果
    return Response(result)


# 第三种方式: 协程里面放线程池(线程池中不放同步io)，假设所有的io操作都是可以异步的
@app.get('/abc/ver3')
async def ver3(req: Request):
    # 获取参数
    msg = req.query_params.get('msg')
    # 获取事件循环
    loop = asyncio.get_event_loop()

    # 任务
    task = {
        'msg': msg
    }

    # 假设这个task中的代码是可异步的io操作
    async def handle_task():
        # TODO 结果出来前，可以在这里把计算性任务放到线程池中批量执行
        # 用异步方法把任务提交到线程池
        # result = await loop.run_in_executor(threadPool, 一个方法)

        print('task received:', task['msg'])
        result = task['msg'].upper()
        await asyncio.sleep(2)

        # TODO 结果出来后，可以在这里把计算性任务放到线程池中批量执行
        # 用异步方法把任务提交到线程池
        # result = await loop.run_in_executor(threadPool, 一个方法)

        # 这里不用asyncio.Event()来唤醒事件了，直接返回
        return result

    # 用异步方法把任务提交到线程池
    result = await handle_task

    print('task ends notified:', result, asyncio.get_event_loop())
    # 返回结果
    return Response(result)


if __name__ == '__main__':
    # 启动web服务，在8080端口上
    uvicorn.run(app, host='127.0.0.1', port=8080)
