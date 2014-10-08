package com.nio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

/**
 * java nio客户端例子
 * @author MASF
 *
 */
public class NioClient {

	public static void main(String args[]) {
		String server_ip = "127.0.0.1";
		int server_port = 8080;
		
		//创建IO多路复用器
		Selector selector = null;
		try {
			selector = Selector.open();
		} catch (IOException e1) {
			e1.printStackTrace();
			throw new RuntimeException("创建IO多路复用器异常",e1);
		}
		
		SocketChannel socket = null;
		try {
			//创建一个socket
			socket = SocketChannel.open();
			//设置为非阻塞模式
			//和Selector一起使用则通道必须设置为非阻塞
			//该设置在使用Selector之前调用就可以
			socket.configureBlocking(false);
		} catch (IOException e) {
			e.printStackTrace();
			throw new RuntimeException("创建SocketChannel异常",e);
		}
	
		//向服务端发起链接 
		try {
			if(socket.connect(new InetSocketAddress(server_ip, server_port))){
				//如果链接成功,则向selector注册相关事件
				socket.register(selector, SelectionKey.OP_READ|SelectionKey.OP_WRITE);
			}else{
				//非阻塞模式下，链接还为成功，注册链接事件等待链接成功
				socket.register(selector, SelectionKey.OP_CONNECT);
			}
		} catch (IOException e) {
			e.printStackTrace();
			throw new RuntimeException("发起链接异常",e);
		}
		
		//Selector维护了三种选择键集:
		//1.注册的键集
		//2.已选择键集,就是已经就绪的通道
		//3.已取消的键集
		//开始轮训事件
		while(true){
			try {
				int t = selector.select();  //零是什么意思
				System.out.println("t:"+t);
			} catch (IOException e) {
				throw new RuntimeException("轮训事件时异常",e);
			}
			
			//取出已就绪的键集
			Set<SelectionKey> keys = selector.selectedKeys();
			Iterator<SelectionKey> iterator = keys.iterator();
			while(iterator.hasNext()){
				SelectionKey key = iterator.next();
				//从已就绪集合中移除该键,另外两个集合不受影响
				iterator.remove();
				
				//处理链接成功事件
				if(key.isConnectable()){
					System.out.println("ccccc");
					SocketChannel sc = (SocketChannel) key.channel();
					try {
						//完成连接过程
						//如果该通道是非阻塞模式且在调用connect()方法后返回的是false,
						//则后续必须用finishConnect()方法来完成连接
						if(sc.finishConnect()){
							sc.register(selector,SelectionKey.OP_READ|SelectionKey.OP_WRITE);
							//key.interestOps(SelectionKey.OP_READ|SelectionKey.OP_WRITE);
						}
					}catch (Exception e) {
						throw new RuntimeException("SocketChannel注册事件时异常",e);
					}
				//处理可读事件
				}else if(key.isReadable()){
					SocketChannel channel =	(SocketChannel) key.channel();
					try {						
						ByteBuffer buffer = ByteBuffer.allocate(1024);
						int len = channel.read(buffer);
						//如果该通道已到达流的末尾，则返回 -1
						if(len < 0 ){
							int interest = key.interestOps();
							//取消读事件
							key.interestOps(interest ^ SelectionKey.OP_READ);
							//关闭通道
							channel.close();
						}else{
							buffer.flip();
							byte[] bytes = new byte[buffer.remaining()];
							buffer.get(bytes);
							System.out.println("read ----> "+new String(bytes));
						}
					} catch (IOException e) {
						try {
							//该方法和key.cancel()都会取消该键
							//并且在下次调用select()方法时将通道将从选择器的注册键集中移除
							channel.close();
						} catch (IOException e1) {
							e1.printStackTrace();
						}
					}
					//处理可写事件
				}else if (key.isWritable()) {
					SocketChannel channel =	(SocketChannel) key.channel();
					try {
						ByteBuffer buffer = ByteBuffer.wrap("Hello\r\n\r\n".getBytes());
						channel.write(buffer);
						int interest = key.interestOps();
						//取消写事件
						key.interestOps(interest ^ SelectionKey.OP_WRITE);
					} catch (IOException e) {
						try {
							channel.close();
						} catch (IOException e1) {
							e1.printStackTrace();
						}
					}
				
				}
			}			
		}
	}
	

}
