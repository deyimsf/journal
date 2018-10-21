#vagrant用法,需要先安装VirtualBox
* 下载VirtualBox: https://www.virtualbox.org/wiki/Downloads
* 下载vagrant: https://www.vagrantup.com/downloads.html

```shell
 // 增加一个box,通过http或本地。完成后镜像被放到/Users/mashunfeng/.vagrant.d/boxes 目录下
 $ vagrant box add <name> http://xxx.box|./xxx.box   //增加一个box,通过http或本地 
 $ vagrant global-status //查看所有box状态,用返回的ID可以ssh上这个虚拟机 
 
 $ vagrant init <name>    //刚开始安装后,初始化<name>的环境 
 $ vagrant up <空|ID>     //启动

 $ vagrant ssh <ID>   //ssh上这个虚拟机 
   
 $ vagrant halt <?>     //关闭虚拟机 
 $ vagrant reload <?>   //重启虚拟机 
 $ vagrant status <?>   //查看虚拟机状态 
 $ vagrant destroy <?>  //销毁当前虚拟机 

 $ vagrant box list     //查看当前安装的box
 $ vagrant box remove   //删除一个box
```

vagrant box资源
http://www.vagrantbox.es/



#打包一个box
两种方式,一种是在任意目录输入如下命令 
```shell
 $ vagrant package --base [NAME] --output my.box
 [NAME]: virtualbox的名字或uuid(可以通过vboxmanage list vms获取)
```

另一种,进入init virtualbox是的目录,然后执行该命令 
```shell
 $ cd /My/work/virtualbox/
 $ vagrant package --output my.box
```


#在本机直接用ssh登陆vagrant
```shell
 $ssh vagrant@localhost -p 2222  
```

因为本地做了一个2222到22的映射所以可以,和使用vagrant ssh <ID>一样


#在本机用scp和vagrant链接
```shell
 $scp -P 2222 file root@localhost:/my/file
```


#端口映射
把宿主计算机的端口映射到虚拟机的端口上,访问宿主机端口时,请求会被转发到虚拟机的端口上。

将宿主机端口8080映射到虚拟机的80端口上
```shell
  config.vm.forwarded_port 80, 8080
```

默认只转发TCP包,如果需要额外增加UDP则需要添加如下语句
```shell
  config.vm.forwarded_port 80, 8080, protocol: "udp"
```

注意要关闭虚拟机对端口的限制。


#私有网络设置
只有宿主主机可以使用这个ip访问虚拟机,多个虚拟机设定在同一个网段也可以互相访问,
同时虚拟机也可以访问外网。

语法:
```shell
  config.vm.network "private_network", ip:"192.168.33.10"
```
在宿主机访问192.168.33.10时,实际访问的就是虚拟主机。


#公有网络设置
虚拟机和宿主机具有同样的待遇
```shell
  config.vm.network "public_network"
```

指定ip
```shell
  config.vm.network "public_network", ip:"192.168.1.11"
```

设置桥接的网卡
```shell
  config.vm.network "public_network", :bridge => 'en1: Wi-Fi(AirPort)'
```



