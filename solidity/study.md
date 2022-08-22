#合约
 pragma solidity >=0.7.0;
 
 contract Constract {
    uint256 num;

    funcation setNum(uint256 number) public {
	num = number;
    }
 }


#状态变量
 类似其他语言中的成员变量
 contract AAA {
   string name;

 }

#局部变量
 也叫本地变量,函数中/出入参/等


#方法
 funcaiton getAAA() public returns(uint256){
	return 256;
 }

#构造函数
 constructor(){
   // dosomething
 }
 构造函数也可以加入参

#异常(高版本被禁用,换成require()/assert()/revert())
 可以使用throw抛出异常，但是不支持捕获，抛出后会回滚之前所有操作
 function test() public{
   uint256 num;
   num = 10;
   throw;
 }

#修饰符
#public修饰符
 谁都可以访问，public修饰的状态变量编译器直接提供getter方法
 例如:string public name; 编译器自动给添加一个name()方法返回该属性值

#internal修饰符
 1.修饰的状态变量外部和子合约都可以访问，状态变量默认就是internal修饰
 2.修饰函数，则只能合约内部访问,不能继承

#private修饰符
 1.修饰的变量和函数，只能在合约内部访问，子合约和外部都无法直接访问

#external修饰符
 1.不能修饰状态变量
 2.函数的调用方式有两种，一种是内部调用;另一种是外部调用，需要创建一个EVM call
 3.external修饰的函数，内部不能调用，如果要调用则需要使用this关键字(这种会通过创建一个EVM call来调用函数);这种函数只能外部调用。

#constant修饰符
 1.修饰状态变量，则表示这是一个常量
 2.修饰函数，则表示该函数不会修改以太坊状态(被view替代)

#view修饰符
 1.使用该修饰符修饰的函数，不能更改状态变量(不能修改以太区块链状态)
 uint256 number;
 funcation setAbc(uint256 num) public view{
 	number = num;//不会修改状态变量的值
 }

#pure修饰符
 1.修饰函数，表示该函数不能读取状态变量值(当然更不能修改了)


#----------------------数据类型--------------------------
#三种引用类型
 1.mapping
 2.struct
 3.数组(string/动态字节数组)

#bool类型
 1.关键字 bool
 2.就两个值 true/false
 
 bool flag = true;

#Integers类型
 1.有符号,范围从[int8~int256]
 2.无符号,范围从[uint8~uint256]
 
 注意:整型字面量做除法不截断，但整型变量会截断
 uint256 aaa = 5/2 + 5/2; //结果是5，不会截断
 
 uint8 a = 5;
 uint8 b = 2;
 a/b + a/b; //结果是4，会被截断

#枚举类型
 enum Level {A,B,C}
 
 function getLevel(Level level) returns(string){
:	if(level == Level.A){
		return "great";
	}
 }

#mapping类型
 1.定义方式 mapping(keyType => valueType)
 2.mapping一般只能是状态变量，可以用作局部变量，但只能引用状态变量,而且存储位置为storage
 3.不能迭代

 mapping(uint256=>string) public abc;

 function bbb() public {
    mapping(uint256=>string) storage abcRef = abc;
    abcRef[1] = "abc";
 }

#结构体struct
 1.不能作为出入参，不能作为返回值

 contract structPerson{
   struct Person{
 	string name;
	uint8 age;
   }

   constructor(){
       Person public tom;
       Person public alice;
   }

   function tomName() public view returns(string){
       return tom.name;
   }
}
 
#固定大小字节数组
 1.范围 bytes1 bytes2    bytes32
 2.只能读不能修改
 
 bytes2 public stringBytes = "ab";
 uint8 aaaa = stringBytes.length;

#动态数组
 1.
 
 bytes public abcBytes = new bytes(2);
 function a() public{
    bytes1 abcd  = "a";
    abcBytes.push(abcd);
 }

#动态长度数组
 uint256[] public array = [1,2,3,4,5];
 function b() public{
    array.push(3);
    array[1] = 2;
    array.length;
 }


#地址类型
 function addressDemo(address payable addr) public{
    address(addr).balance;  // 查看地址余额
    addr.send(1 ether);  // 发送以太币，发生错误返回false
    addr.transfer(1 ether); // 发送以太币，发生错误抛异常，回滚之前所有操作
 }

#调用合约函数方法区别call、callcode、delegatecall 
 // TODO
 1. addr.call(函数名,参数).value([附加以太币]).gas([指定gas])
 2. delegatecall() 不支持.value() 










