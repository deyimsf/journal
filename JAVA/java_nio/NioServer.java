package com.nio;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.channels.SelectionKey;
import java.nio.channels.Selector;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Iterator;
import java.util.Set;

/**
 * java nio 服务端例子
 * @author MASF
 *
 */
public class NioServer {
	
	public static void main(String args[]) throws IOException{
		int local_ip = 8080;
	
		//IO多路复用器
		Selector selector = Selector.open(); 
		ServerSocketChannel serverChannel = ServerSocketChannel.open();
		serverChannel.configureBlocking(false);
		serverChannel.socket().bind(new InetSocketAddress(local_ip),1024);  //1024是可接收得客户端的积压长度
		serverChannel.register(selector, SelectionKey.OP_ACCEPT);
		System.out.println("The time server is start in port :"+ local_ip);
		
		//Selector维护了三种选择键集:
		//1.注册的键集
		//2.已选择键集,就是已经就绪的通道
		//3.已取消的键集
		//开始接受客户端
		while(true){
			int t = selector.select();
			Set<SelectionKey> selectionKeys = selector.selectedKeys();
			Iterator<SelectionKey> iterator = selectionKeys.iterator();
			while(iterator.hasNext()){
				System.out.println(selectionKeys);

				//就绪的key，该key和通道关联
				SelectionKey key=iterator.next(); 
				//将该key从已选择集合中删除
				iterator.remove();
				System.out.println(selectionKeys);
				if(!key.isValid()){
					System.out.println(key+":无效"); 
					continue;
				}
				
				//有客户端接入
				if(key.isAcceptable()){
					ServerSocketChannel ssc=(ServerSocketChannel)key.channel();
					SocketChannel sc=ssc.accept();
					sc.configureBlocking(false);
					//为客户端的socket注册相关事件
					sc.register(selector, SelectionKey.OP_CONNECT|SelectionKey.OP_READ|SelectionKey.OP_WRITE);
				
					System.out.println("isAcceptable");
								
				   //处理可读事件
				}else if(key.isReadable()){
					SocketChannel sc=(SocketChannel)key.channel();
					//TODO  do something
					System.out.println("isReadable");
				
					//处理可写事件
				}else if(key.isWritable()){
					SocketChannel sc=(SocketChannel)key.channel();
					//TODO do something
					System.out.println("isWritable");
					
				}
			}
		}	
	}
}
